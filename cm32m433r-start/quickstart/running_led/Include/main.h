// See LICENSE for license details.

#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "nuclei_sdk_soc.h"
#include "bsp.h"

typedef enum
{
    FAILED = 0,
    PASSED = !FAILED
} Status;

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */
