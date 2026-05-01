#ifndef TEE_CLIENT_API_H
#define TEE_CLIENT_API_H

#include <stddef.h>
#include <stdint.h>

typedef uint32_t TEEC_Result;
typedef struct { int unused; } TEEC_Context;
typedef struct { int unused; } TEEC_Session;
typedef struct {
    uint32_t timeLow;
    uint16_t timeMid;
    uint16_t timeHiAndVersion;
    uint8_t clockSeqAndNode[8];
} TEEC_UUID;

typedef struct {
    struct {
        void *buffer;
        size_t size;
    } tmpref;
    struct {
        uint32_t a;
        uint32_t b;
    } value;
} TEEC_Parameter;

typedef struct {
    uint32_t paramTypes;
    TEEC_Parameter params[4];
} TEEC_Operation;

#define TEEC_SUCCESS 0x00000000u
#define TEEC_ERROR_BAD_PARAMETERS 0xFFFF0006u
#define TEEC_LOGIN_PUBLIC 0u
#define TEEC_NONE 0u
#define TEEC_MEMREF_TEMP_OUTPUT 0u
#define TEEC_VALUE_OUTPUT 0u
#define TEEC_VALUE_INPUT 0u
#define TEEC_PARAM_TYPES(t0, t1, t2, t3) 0u

TEEC_Result TEEC_InitializeContext(const char *name, TEEC_Context *context);
void TEEC_FinalizeContext(TEEC_Context *context);
TEEC_Result TEEC_OpenSession(TEEC_Context *context, TEEC_Session *session,
                             const TEEC_UUID *destination, uint32_t connection_method,
                             const void *connection_data, TEEC_Operation *operation,
                             uint32_t *return_origin);
void TEEC_CloseSession(TEEC_Session *session);
TEEC_Result TEEC_InvokeCommand(TEEC_Session *session, uint32_t command_id,
                               TEEC_Operation *operation, uint32_t *return_origin);

#endif

