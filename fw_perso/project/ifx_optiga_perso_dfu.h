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

#ifndef _IFX_OPTIGA_PERSO_DFU_H_
#define _IFX_OPTIGA_PERSO_DFU_H_

#include <stdint.h>
#include "optiga/optiga_util.h"

// TODO: set when ready to lock
#define I_AM_NOT_NOT_NOT_READY_TO_LOCK

#ifndef I_AM_READY_TO_LOCK
#warning You are currently NOT locking your personalized data!
#warning ! For production, enable I_AM_READY_TO_LOCK to lock !
#endif

#ifdef I_AM_READY_TO_LOCK
#warning !!!!!!!!!!!!!!!!! YOU ARE LOCKING DATA OBJECTS FOREVER !!!!!!!!!!!!!!!!!
#warning !Do this only when you have the final data that you want to personalize!
#warning !!!!!!!!!!!!!!!!! YOU ARE LOCKING DATA OBJECTS FOREVER !!!!!!!!!!!!!!!!!
#endif

bool ifx_optiga_perso_dfu_run(void);

bool ifx_optiga_perso_dfu_verify(void);

#endif // _IFX_OPTIGA_PERSO_DFU_H_