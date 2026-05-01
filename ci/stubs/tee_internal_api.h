#ifndef TEE_INTERNAL_API_H
#define TEE_INTERNAL_API_H

#include <stddef.h>
#include <stdint.h>

typedef uint32_t TEE_Result;
typedef void *TEE_ObjectHandle;

typedef struct {
    struct {
        void *buffer;
        size_t size;
    } memref;
    struct {
        uint32_t a;
        uint32_t b;
    } value;
} TEE_Param;

#define TEE_HANDLE_NULL ((TEE_ObjectHandle)0)
#define TEE_SUCCESS 0x00000000u
#define TEE_ERROR_BAD_PARAMETERS 0xFFFF0006u
#define TEE_ERROR_ITEM_NOT_FOUND 0xFFFF0008u
#define TEE_ERROR_CORRUPT_OBJECT 0xF0100001u
#define TEE_ERROR_NOT_SUPPORTED 0xFFFF000Au
#define TEE_ERROR_SHORT_BUFFER 0xFFFF0010u

#define TEE_STORAGE_PRIVATE 0u
#define TEE_DATA_FLAG_ACCESS_READ 0x1u
#define TEE_DATA_FLAG_ACCESS_WRITE 0x2u
#define TEE_DATA_FLAG_ACCESS_WRITE_META 0x4u
#define TEE_DATA_FLAG_OVERWRITE 0x8u

#define TA_FLAG_SINGLE_INSTANCE 0x1u
#define TA_FLAG_INSTANCE_KEEP_ALIVE 0x2u

#define TEE_PARAM_TYPE_NONE 0u
#define TEE_PARAM_TYPE_MEMREF_OUTPUT 1u
#define TEE_PARAM_TYPE_VALUE_OUTPUT 2u
#define TEE_PARAM_TYPE_VALUE_INPUT 3u
#define TEE_PARAM_TYPES(t0, t1, t2, t3) 0u
#define TEE_MIN(a, b) ((a) < (b) ? (a) : (b))

TEE_Result TEE_CreatePersistentObject(uint32_t storage_id, const void *object_id,
                                      size_t object_id_len, uint32_t flags,
                                      TEE_ObjectHandle attributes, const void *initial_data,
                                      size_t initial_data_len, TEE_ObjectHandle *object);
TEE_Result TEE_OpenPersistentObject(uint32_t storage_id, const void *object_id,
                                    size_t object_id_len, uint32_t flags,
                                    TEE_ObjectHandle *object);
TEE_Result TEE_ReadObjectData(TEE_ObjectHandle object, void *buffer,
                              size_t size, uint32_t *count);
void TEE_CloseObject(TEE_ObjectHandle object);
void TEE_GenerateRandom(void *buffer, size_t size);
void *TEE_MemMove(void *dest, const void *src, size_t n);

#endif

