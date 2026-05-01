#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tee_client_api.h>

#include "ta_skeleton.h"

static void usage(const char *argv0) {
    fprintf(stderr,
            "usage: %s version | random <bytes> | counter-read | counter-write <value>\n",
            argv0);
}

static TEEC_Result open_session(TEEC_Context *ctx, TEEC_Session *sess,
                                uint32_t *origin) {
    TEEC_UUID uuid = TA_SKELETON_UUID;
    TEEC_Result res = TEEC_InitializeContext(NULL, ctx);
    if (res != TEEC_SUCCESS) {
        return res;
    }

    res = TEEC_OpenSession(ctx, sess, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, origin);
    if (res != TEEC_SUCCESS) {
        TEEC_FinalizeContext(ctx);
    }
    return res;
}

static void print_hex(const uint8_t *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02x", buf[i]);
    }
    putchar('\n');
}

int main(int argc, char **argv) {
    TEEC_Context ctx;
    TEEC_Session sess;
    uint32_t origin = 0;
    TEEC_Result res;

    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }

    res = open_session(&ctx, &sess, &origin);
    if (res != TEEC_SUCCESS) {
        fprintf(stderr, "failed to open OP-TEE session: 0x%x origin=0x%x\n", res, origin);
        return 1;
    }

    if (strcmp(argv[1], "version") == 0) {
        char version[TA_SKELETON_VERSION_MAX] = {0};
        TEEC_Operation op = {0};
        op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE,
                                         TEEC_NONE, TEEC_NONE);
        op.params[0].tmpref.buffer = version;
        op.params[0].tmpref.size = sizeof(version);
        res = TEEC_InvokeCommand(&sess, TA_SKELETON_CMD_GET_VERSION, &op, &origin);
        if (res == TEEC_SUCCESS) {
            printf("%s\n", version);
        }
    } else if (strcmp(argv[1], "random") == 0 && argc == 3) {
        long requested = strtol(argv[2], NULL, 10);
        uint8_t buffer[256];
        TEEC_Operation op = {0};

        if (requested <= 0 || requested > (long)sizeof(buffer)) {
            fprintf(stderr, "random length must be between 1 and %zu\n", sizeof(buffer));
            res = TEEC_ERROR_BAD_PARAMETERS;
        } else {
            op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE,
                                             TEEC_NONE, TEEC_NONE);
            op.params[0].tmpref.buffer = buffer;
            op.params[0].tmpref.size = (size_t)requested;
            res = TEEC_InvokeCommand(&sess, TA_SKELETON_CMD_GET_RANDOM, &op, &origin);
            if (res == TEEC_SUCCESS) {
                print_hex(buffer, (size_t)requested);
            }
        }
    } else if (strcmp(argv[1], "counter-read") == 0) {
        uint32_t value = 0;
        TEEC_Operation op = {0};
        op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_OUTPUT, TEEC_NONE,
                                         TEEC_NONE, TEEC_NONE);
        res = TEEC_InvokeCommand(&sess, TA_SKELETON_CMD_COUNTER_READ, &op, &origin);
        if (res == TEEC_SUCCESS) {
            value = op.params[0].value.a;
            printf("%" PRIu32 "\n", value);
        }
    } else if (strcmp(argv[1], "counter-write") == 0 && argc == 3) {
        unsigned long value = strtoul(argv[2], NULL, 10);
        TEEC_Operation op = {0};
        op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT, TEEC_NONE,
                                         TEEC_NONE, TEEC_NONE);
        op.params[0].value.a = (uint32_t)value;
        res = TEEC_InvokeCommand(&sess, TA_SKELETON_CMD_COUNTER_WRITE, &op, &origin);
        if (res == TEEC_SUCCESS) {
            printf("ok\n");
        }
    } else {
        usage(argv[0]);
        res = TEEC_ERROR_BAD_PARAMETERS;
    }

    if (res != TEEC_SUCCESS) {
        fprintf(stderr, "command failed: 0x%x origin=0x%x\n", res, origin);
    }

    TEEC_CloseSession(&sess);
    TEEC_FinalizeContext(&ctx);
    return res == TEEC_SUCCESS ? 0 : 1;
}

