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
#include "nrf_crypto.h"
#include "optiga/optiga_util.h"
#include "nrf_delay.h"
//#include "SEGGER_RTT.h"



void clock_event_handler(nrfx_clock_evt_type_t event)
{
}


int main(void)
{
    ret_code_t err_code;

//    SEGGER_RTT_WriteString(0, "Hello World!\n");

    // Log init
    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    // Start internal LFCLK XTAL oscillator, needed for RTC and OPTIGA
    err_code = nrfx_clock_init(&clock_event_handler);
    APP_ERROR_CHECK(err_code);
    nrfx_clock_lfclk_start();

    // OPTIGA stack needs LF clock
    if (!nrf_clock_lf_is_running())
    {
        nrf_clock_task_trigger(NRF_CLOCK_TASK_LFCLKSTART);
    }

    NRF_LOG_INFO("Running application");

    // Optiga init
    // Implicitly inits Trust X, if OPTIGA backend is enabled in sdk_config
    err_code = nrf_crypto_init();
    APP_ERROR_CHECK(err_code);

    // Print the OPTIGA version;
    uint8_t  data[2]   = { 0 };
    uint16_t data_len  = sizeof(data);
    optiga_util_read_data(0xE0C2, 25, data, &data_len);
    NRF_LOG_INFO("Trust X: FW version (from coprocessor UID): %2x%2x", (uint16_t) data[0], (uint16_t) data[1]);

    while (1)
    {
        NRF_LOG_INFO("Running application Version 1");
        NRF_LOG_FLUSH();
        nrf_delay_ms(5000);
    }
}
