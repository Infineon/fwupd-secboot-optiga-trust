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

#include "ifx_optiga_perso.h"
#include "optiga/optiga_util.h"
#include "nrf_log.h"

void _dump_metadata_for_oid(
    uint16_t oid);
void _dump_data_for_oid(
    uint16_t oid);

#define BUFFER_MAX   (0x3FF)


void ifx_optiga_perso_try_write_oid(
    uint16_t   oid,
    uint8_t  * data,
    uint16_t   data_len)
{
    optiga_lib_status_t optiga_lib_status = OPTIGA_LIB_ERROR;

    NRF_LOG_INFO("TRYING TO CHANGE content of data object 0x%4x", oid);
    NRF_LOG_INFO("BEFORE");
    _dump_metadata_for_oid(oid);
    _dump_data_for_oid(oid);

    optiga_lib_status = optiga_util_write_data(
        oid, OPTIGA_UTIL_WRITE_ONLY, 0, data, data_len);
    if (OPTIGA_LIB_SUCCESS == optiga_lib_status)
    {
        NRF_LOG_INFO("Write data for OID=0x%4x (%d Bytes) successful.", oid, data_len);
        NRF_LOG_HEXDUMP_INFO(data, data_len);
    }
    else
    {
        NRF_LOG_INFO("Write data for OID=0x%4x (%d Bytes) failed.", oid, data_len);
    }

    NRF_LOG_INFO("AFTER");
    _dump_metadata_for_oid(oid);
    _dump_data_for_oid(oid);
}


void ifx_optiga_perso_print_version(void)
{
    uint8_t  data[2]   = { 0 };
    uint16_t data_len  = sizeof(data);
    optiga_util_read_data(0xE0C2, 25, data, &data_len);
    NRF_LOG_INFO("Trust X: FW version (from coprocessor UID): %2x%2x", 
        (uint16_t) data[0], (uint16_t) data[1]);
}


void ifx_optiga_perso_print_all_metadata(void)
{
    uint16_t oid;

    NRF_LOG_INFO("All metadata");

    NRF_LOG_INFO("Private keys (0xE0F0..0xE0F3)");
    for (oid = 0xE0F0; oid <= 0xE0F3; oid++)
    {
        _dump_metadata_for_oid(oid);
    }

    NRF_LOG_INFO("Public key certificates (0xE0E0..0xE0E3)");
    for (oid = 0xE0E0; oid <= 0xE0E3; oid++)
    {
        _dump_metadata_for_oid(oid);
    }

    NRF_LOG_INFO("Trust anchors (0xE0E8, 0xE0EF)");
    _dump_metadata_for_oid(0xE0E8);
    _dump_metadata_for_oid(0xE0EF);

    NRF_LOG_INFO("Arbitrary data objects (0xF1D0..0xF1DF)");
    for (oid = 0xF1D0; oid <= 0xF1DF; oid++)
    {
        _dump_metadata_for_oid(oid);
    }

    NRF_LOG_INFO("Arbitrary data objects (0xF1E0..0xF1E1)");
    for (oid = 0xF1E0; oid <= 0xF1E1; oid++)
    {
        _dump_metadata_for_oid(oid);
    }

    NRF_LOG_INFO("Done.");
}

void _dump_data_for_oid(
    uint16_t data_oid)
{
    optiga_lib_status_t optiga_lib_status = OPTIGA_LIB_ERROR;

    uint8_t  data[BUFFER_MAX];
    uint16_t data_len = sizeof(data);

    optiga_lib_status = optiga_util_read_data(
        data_oid,
        0, // offset
        data,
        &data_len);
    if (OPTIGA_LIB_SUCCESS == optiga_lib_status)
    {
        NRF_LOG_INFO("Data for OID=0x%4x (%d Bytes)", data_oid, data_len);
        NRF_LOG_HEXDUMP_INFO(data, data_len);
    }
    else
    {
        NRF_LOG_INFO("Data for OID=0x%4x could not be read.", data_oid);
    }

    NRF_LOG_FLUSH();
}

void _dump_metadata_for_oid(
    uint16_t md_oid)
{
    optiga_lib_status_t optiga_lib_status = OPTIGA_LIB_ERROR;

    uint8_t  md[BUFFER_MAX]; // md .. metadata
    uint16_t md_len = sizeof(md);

    optiga_lib_status = optiga_util_read_metadata(
        md_oid,
        md,
        &md_len);
    if (OPTIGA_LIB_SUCCESS == optiga_lib_status)
    {
        NRF_LOG_INFO("Metadata for OID=0x%4x (%d Bytes)", md_oid, md_len);
        NRF_LOG_HEXDUMP_INFO(md, md_len);
    }
    else
    {
        NRF_LOG_INFO("Metadata for OID=0x%4x could not be read.", md_oid);
    }

    NRF_LOG_FLUSH();
}
