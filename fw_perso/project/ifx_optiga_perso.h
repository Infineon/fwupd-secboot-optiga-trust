/**
* MIT License
*
* Copyright (c) 2019 Infineon Technologies AG
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE
*/

#ifndef _IFX_OPTIGA_PERSO_H_
#define _IFX_OPTIGA_PERSO_H_

#include <stdint.h>
#include "nrf_log_ctrl.h"
#include "optiga/common/ErrorCodes.h"

#define OPTIGA_ERROR_CHECK(status)  \
do                                  \
{                                   \
    if (status != OPTIGA_LIB_SUCCESS)\
    {                               \
        NRF_LOG_ERROR("OPTIGA lib status: error 0x%04X: %s", status, "");\
        NRF_LOG_FLUSH();            \
        APP_ERROR_HANDLER(0);       \
    }                               \
} while(0)

void ifx_optiga_perso_print_version(void);
void ifx_optiga_perso_print_all_metadata(void);
void ifx_optiga_perso_try_write_oid(
    uint16_t   oid,
    uint8_t  * data,
    uint16_t   data_len);

#endif // _IFX_OPTIGA_PERSO_H_