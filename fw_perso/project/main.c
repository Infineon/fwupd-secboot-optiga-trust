/**
* MIT License
*
* Copyright (c) 2018 Infineon Technologies AG
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

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "app_timer.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrfx_clock.h"
#include "ifx_optiga_perso_dfu.h"
#include "ifx_optiga_perso.h"
#include "nrf_crypto.h"
#include "optiga/optiga_util.h"



void clock_event_handler(nrfx_clock_evt_type_t event)
{
}


int main(void)
{
    ret_code_t err_code;

    // Log init
    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    // Start internal LFCLK XTAL oscillator, needed for RTC and OPTIGA
    err_code = nrfx_clock_init(&clock_event_handler);
    APP_ERROR_CHECK(err_code);
    nrfx_clock_lfclk_start();

    // OPTIGA stack needs LF clock for RTC2
    if (!nrf_clock_lf_is_running())
    {
        nrf_clock_task_trigger(NRF_CLOCK_TASK_LFCLKSTART);
    }
    
    // Optiga init
    // Implicitly inits Trust X, if OPTIGA backend is enabled in sdk_config
    err_code = nrf_crypto_init();
    APP_ERROR_CHECK(err_code);

    ifx_optiga_perso_print_version();

    ifx_optiga_perso_print_all_metadata();

    // Do personalization
    if (!ifx_optiga_perso_dfu_run())
    {
        NRF_LOG_ERROR("Personalization failed: error during personalization");
        APP_ERROR_CHECK(NRF_ERROR_INTERNAL);
    }

    // Verify personalization
    if (!ifx_optiga_perso_dfu_verify())
    {
        NRF_LOG_ERROR("Personalization failed: DFU certificate could not validate FW signature");
        APP_ERROR_CHECK(NRF_ERROR_INTERNAL);
    }

    ifx_optiga_perso_print_all_metadata();

    // Done
    NRF_LOG_INFO("Personalization successfully completed.");
    NRF_LOG_FLUSH();

    while (1)
    {
        ;
    }
}
