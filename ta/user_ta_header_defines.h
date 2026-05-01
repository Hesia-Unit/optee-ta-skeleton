#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

#include <ta_skeleton.h>

#define TA_UUID TA_SKELETON_UUID

#define TA_FLAGS (TA_FLAG_SINGLE_INSTANCE | TA_FLAG_INSTANCE_KEEP_ALIVE)
#define TA_STACK_SIZE (16 * 1024)
#define TA_DATA_SIZE  (64 * 1024)

#define TA_VERSION "1.0"
#define TA_DESCRIPTION "Generic OP-TEE public skeleton"

#endif /* USER_TA_HEADER_DEFINES_H */

