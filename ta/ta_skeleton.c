#include <stdint.h>
#include <string.h>

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include <ta_skeleton.h>

#define COUNTER_OBJECT_ID "counter.value"

static TEE_Result write_counter(uint32_t value) {
    TEE_ObjectHandle object = TEE_HANDLE_NULL;
    uint32_t flags = TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_ACCESS_WRITE |
                     TEE_DATA_FLAG_ACCESS_WRITE_META | TEE_DATA_FLAG_OVERWRITE;
    TEE_Result res = TEE_CreatePersistentObject(TEE_STORAGE_PRIVATE,
                                                COUNTER_OBJECT_ID,
                                                sizeof(COUNTER_OBJECT_ID) - 1,
                                                flags,
                                                TEE_HANDLE_NULL,
                                                &value,
                                                sizeof(value),
                                                &object);
    if (object != TEE_HANDLE_NULL) {
        TEE_CloseObject(object);
    }
    return res;
}

static TEE_Result read_counter(uint32_t *value) {
    TEE_ObjectHandle object = TEE_HANDLE_NULL;
    uint32_t flags = TEE_DATA_FLAG_ACCESS_READ;
    uint32_t read_bytes = 0;
    TEE_Result res = TEE_OpenPersistentObject(TEE_STORAGE_PRIVATE,
                                              COUNTER_OBJECT_ID,
                                              sizeof(COUNTER_OBJECT_ID) - 1,
                                              flags,
                                              &object);
    if (res == TEE_ERROR_ITEM_NOT_FOUND) {
        *value = 0;
        return TEE_SUCCESS;
    }
    if (res != TEE_SUCCESS) {
        return res;
    }

    res = TEE_ReadObjectData(object, value, sizeof(*value), &read_bytes);
    TEE_CloseObject(object);
    if (res != TEE_SUCCESS) {
        return res;
    }
    if (read_bytes != sizeof(*value)) {
        return TEE_ERROR_CORRUPT_OBJECT;
    }
    return TEE_SUCCESS;
}

TEE_Result TA_CreateEntryPoint(void) {
    return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void) {
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types, TEE_Param params[4],
                                    void **session_ctx) {
    (void)param_types;
    (void)params;
    (void)session_ctx;
    return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void *session_ctx) {
    (void)session_ctx;
}

static TEE_Result cmd_get_version(uint32_t param_types, TEE_Param params[4]) {
    static const char version[] = "optee-ta-skeleton-1.0";
    const uint32_t expected = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_OUTPUT,
                                              TEE_PARAM_TYPE_NONE,
                                              TEE_PARAM_TYPE_NONE,
                                              TEE_PARAM_TYPE_NONE);
    if (param_types != expected) {
        return TEE_ERROR_BAD_PARAMETERS;
    }
    if (params[0].memref.buffer == NULL || params[0].memref.size < sizeof(version)) {
        params[0].memref.size = sizeof(version);
        return TEE_ERROR_SHORT_BUFFER;
    }
    TEE_MemMove(params[0].memref.buffer, version, sizeof(version));
    params[0].memref.size = sizeof(version);
    return TEE_SUCCESS;
}

static TEE_Result cmd_get_random(uint32_t param_types, TEE_Param params[4]) {
    const uint32_t expected = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_OUTPUT,
                                              TEE_PARAM_TYPE_NONE,
                                              TEE_PARAM_TYPE_NONE,
                                              TEE_PARAM_TYPE_NONE);
    if (param_types != expected) {
        return TEE_ERROR_BAD_PARAMETERS;
    }
    if (params[0].memref.buffer == NULL || params[0].memref.size == 0) {
        return TEE_ERROR_BAD_PARAMETERS;
    }
    TEE_GenerateRandom(params[0].memref.buffer, params[0].memref.size);
    return TEE_SUCCESS;
}

static TEE_Result cmd_counter_read(uint32_t param_types, TEE_Param params[4]) {
    uint32_t value = 0;
    const uint32_t expected = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_OUTPUT,
                                              TEE_PARAM_TYPE_NONE,
                                              TEE_PARAM_TYPE_NONE,
                                              TEE_PARAM_TYPE_NONE);
    if (param_types != expected) {
        return TEE_ERROR_BAD_PARAMETERS;
    }
    TEE_Result res = read_counter(&value);
    if (res != TEE_SUCCESS) {
        return res;
    }
    params[0].value.a = value;
    params[0].value.b = 0;
    return TEE_SUCCESS;
}

static TEE_Result cmd_counter_write(uint32_t param_types, TEE_Param params[4]) {
    const uint32_t expected = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT,
                                              TEE_PARAM_TYPE_NONE,
                                              TEE_PARAM_TYPE_NONE,
                                              TEE_PARAM_TYPE_NONE);
    if (param_types != expected) {
        return TEE_ERROR_BAD_PARAMETERS;
    }
    return write_counter(params[0].value.a);
}

TEE_Result TA_InvokeCommandEntryPoint(void *session_ctx, uint32_t cmd_id,
                                      uint32_t param_types, TEE_Param params[4]) {
    (void)session_ctx;

    switch (cmd_id) {
    case TA_SKELETON_CMD_GET_VERSION:
        return cmd_get_version(param_types, params);
    case TA_SKELETON_CMD_GET_RANDOM:
        return cmd_get_random(param_types, params);
    case TA_SKELETON_CMD_COUNTER_READ:
        return cmd_counter_read(param_types, params);
    case TA_SKELETON_CMD_COUNTER_WRITE:
        return cmd_counter_write(param_types, params);
    default:
        return TEE_ERROR_NOT_SUPPORTED;
    }
}
