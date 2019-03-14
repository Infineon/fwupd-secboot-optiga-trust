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

#include <string.h>
#include "boards.h"
#include "nrf_log.h"
#include "nrf_crypto.h"
#include "optiga_backend_utils.h"
#include "ifx_optiga_perso_dfu.h"


static bool _lock_data_object(
    uint16_t object_size,
    uint16_t object_oid
);

static bool _verify_secure_boot_dfu_certificate(void);



/**
 * Output of command
 *  $ python.exe .\tools\bin2chex.py -f .\key\developer_key.cert.der
 */
static const uint8_t SECURE_DFU_BOOT_CERTIFICATE[] = 
{
    0x30, 0x82, 0x01, 0x43, 0x30, 0x81, 0xEA, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x14, 0x2D, 0xED,
    0x16, 0x68, 0x79, 0x6B, 0x80, 0xF4, 0xBF, 0x1F, 0x34, 0x78, 0x43, 0x6C, 0x69, 0xFA, 0xAF, 0xF1,
    0xF9, 0x89, 0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02, 0x30, 0x14,
    0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x09, 0x44, 0x65, 0x76, 0x65, 0x6C,
    0x6F, 0x70, 0x65, 0x72, 0x30, 0x1E, 0x17, 0x0D, 0x31, 0x39, 0x30, 0x33, 0x30, 0x34, 0x31, 0x33,
    0x35, 0x36, 0x32, 0x30, 0x5A, 0x17, 0x0D, 0x34, 0x36, 0x30, 0x37, 0x32, 0x30, 0x31, 0x33, 0x35,
    0x36, 0x32, 0x30, 0x5A, 0x30, 0x14, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C,
    0x09, 0x44, 0x65, 0x76, 0x65, 0x6C, 0x6F, 0x70, 0x65, 0x72, 0x30, 0x59, 0x30, 0x13, 0x06, 0x07,
    0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01,
    0x07, 0x03, 0x42, 0x00, 0x04, 0xBB, 0xBB, 0xB1, 0x23, 0x13, 0xF3, 0x2F, 0x1B, 0x29, 0xDA, 0xC0,
    0x4E, 0x62, 0xF0, 0xEC, 0xBE, 0x34, 0xBA, 0x70, 0x98, 0x68, 0x69, 0xC4, 0x01, 0x53, 0x0B, 0xC1,
    0xE3, 0x7D, 0x83, 0xAF, 0x73, 0x12, 0x6C, 0xC5, 0x7C, 0x05, 0x77, 0xA9, 0x3D, 0x58, 0x1F, 0xE5,
    0x22, 0x52, 0x0C, 0x09, 0xFB, 0x0C, 0xBE, 0x86, 0x59, 0xE3, 0xC0, 0x72, 0xFE, 0xCE, 0x17, 0x1C,
    0xF1, 0x6B, 0xDF, 0x06, 0x31, 0xA3, 0x1A, 0x30, 0x18, 0x30, 0x09, 0x06, 0x03, 0x55, 0x1D, 0x13,
    0x04, 0x02, 0x30, 0x00, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x1D, 0x0F, 0x04, 0x04, 0x03, 0x02, 0x05,
    0xE0, 0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02, 0x03, 0x48, 0x00,
    0x30, 0x45, 0x02, 0x20, 0x6B, 0xA9, 0xD0, 0x57, 0xAC, 0x78, 0x69, 0xEA, 0xE4, 0x20, 0x14, 0x65,
    0xD0, 0x5C, 0x87, 0x66, 0xE2, 0xBC, 0x0F, 0x96, 0x38, 0x7E, 0x5C, 0x11, 0x16, 0xCA, 0x14, 0x05,
    0xCE, 0xCD, 0x0E, 0x95, 0x02, 0x21, 0x00, 0x8C, 0x9B, 0x73, 0x7C, 0xD7, 0xDB, 0xFE, 0x5B, 0xBB,
    0x54, 0x9E, 0x4E, 0xAA, 0xFC, 0xE3, 0xF5, 0x6D, 0xC6, 0xAE, 0x7D, 0x76, 0x23, 0x68, 0xD3, 0xCB,
    0x4F, 0x15, 0xDA, 0x21, 0x06, 0x59, 0x2E,
};

/**
  This function personalizes the Trust X.
  The function expects the OPTIGA Trust X to be opened (open application already called).
*/
bool ifx_optiga_perso_dfu_run(void)
{
    optiga_lib_status_t optiga_lib_status = OPTIGA_LIB_ERROR;

    uint16_t SECURE_DFU_BOOT_CERTIFICATE_OID = 0xE0EF; // Platform integrity slot


    NRF_LOG_INFO("Personalize: Write secure boot and secure DFU certificate containing public verification key");
    optiga_lib_status = optiga_util_write_data(
        SECURE_DFU_BOOT_CERTIFICATE_OID, 
        OPTIGA_UTIL_ERASE_AND_WRITE,
        0,
        (uint8_t *) SECURE_DFU_BOOT_CERTIFICATE, 
        sizeof(SECURE_DFU_BOOT_CERTIFICATE));
    if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
    {
        NRF_LOG_ERROR("Error writing secure DFU/boot certificate");
        return false;
    }

    if (!_lock_data_object(
          SECURE_DFU_BOOT_CERTIFICATE_OID, 
          sizeof(SECURE_DFU_BOOT_CERTIFICATE)))
    {
        return false;
    }

    return true;
}

static bool _lock_data_object(
    uint16_t object_oid,
    uint16_t object_size
)
{
    optiga_lib_status_t optiga_lib_status = OPTIGA_LIB_ERROR;

    uint8_t len_hi = (uint8_t) (object_size >> 8) & 0xFF;
    uint8_t len_lo = (uint8_t)  object_size       & 0xFF;

    // The following metadata "locks" in object in terms of
    // prohibiting future changes to it, but still allowing
    // to read and use it.

    uint8_t metadata[] = {
        0x20, 0x0b,                    // Header
    //  ****                           //   Type
    //        ****                     //   Length of following fields (0x0F==15d)
        0xc0, 0x01, 0x07,              // LcsO: set LcsO to operational (0x07)
    //              ****               //   0x07 for op
        0xd0, 0x03, 0xe1, 0xfc, 0x04,  // Access condition for CHANGING:
    //              ****               //   LcsO (E1)
    //                    ****         //   "<"
    //                          ****   //   Operational state
        0xd1, 0x01, 0x00,              // Access condition for READ:
    //              ****               //   Always
//        0xc5, 0x02, len_hi, len_lo,    // Used size of data object
    };

#ifdef I_AM_READY_TO_LOCK
    optiga_lib_status = optiga_util_write_metadata(
        object_oid,
        metadata,
        sizeof(metadata));
    if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
    {
        NRF_LOG_ERROR("Error writing metadata for OID=0x%4x", object_oid);
        return false;
    }
#endif

    return true;
}

bool ifx_optiga_perso_dfu_verify(void)
{
    return _verify_secure_boot_dfu_certificate();
}

static bool _verify_secure_boot_dfu_certificate(void)
{
    ret_code_t          nordic_err_code;
    uint16_t oid        = 0xE0EF;

    /*
      The developer compiles a firmware binary, with the content "0xFIRMWARE":
      (using PowerShell, Python 2, bin2chex.py and OpenSSL)

      **) Generate the (fake) firmware binary
          $ echo "0xFIRMWARE" | Out-File firmware.bin -Encoding ASCII

      **) Convert the file into C-array formatted string, and put it into the variable 'firmware_binary'
          $ python.exe optiga_tools.py -f firmware.bin

      **) Sign the firmware, using OpenSSL, and the developer' private key
          $ openssl.exe dgst -sha256 -sign .\key\developer_key.private.pem -out .\firmware.sig.der .\firmware.bin
     
      **) Convert the DER-coded file into C-array formatted string, and put it into the variable 'sig_asn'
          $ python.exe .\bin2chex.py -f .\firmware.sig.der
    */

    uint8_t firmware_binary[] = { // == "0xFIRMWARE"
        0x30, 0x78, 0x46, 0x49, 0x52, 0x4D, 0x57, 0x41, 0x52, 0x45, 0x0D, 0x0A
     };
    uint8_t sig_asn[] = {
        0x30, 0x46, 0x02, 0x21, 0x00, 0xF3, 0xCA, 0xF5, 0x58, 0xDF, 0xC8, 0xBC, 0xAF, 0x1E, 0xB2, 0x1E,
        0xA4, 0xA5, 0x1C, 0xF8, 0x3E, 0x45, 0xC1, 0x86, 0xE1, 0xF1, 0xCC, 0x57, 0x56, 0x0F, 0x02, 0xC0,
        0xE0, 0x66, 0x99, 0x8B, 0x20, 0x02, 0x21, 0x00, 0xB7, 0xE0, 0x4D, 0xF5, 0x50, 0x5A, 0x4B, 0xFB,
        0xD8, 0x6B, 0x8D, 0xCC, 0xDE, 0x8F, 0xBD, 0x9E, 0x5F, 0xD6, 0x17, 0xF0, 0xEF, 0x7E, 0x6F, 0xC0,
        0xDB, 0x01, 0xD4, 0x13, 0x3C, 0x9F, 0x22, 0xDD
    };

    // Convert signature format
    uint8_t   sig_rs[64] = { 0 };
    size_t    sig_rs_len  = sizeof(sig_rs);
    uint8_t * p_sig_asn   = &sig_asn[2];
    size_t    sig_asn_len = sizeof(sig_asn) - 2;
    nordic_err_code = asn1_to_ecdsa_rs(p_sig_asn, sig_asn_len, sig_rs, &sig_rs_len);
    if (NRF_SUCCESS != nordic_err_code)
    {
        return false;
    }

    // Hash the msg data using the nrf_crypto API
    uint8_t                   hash[NRF_CRYPTO_HASH_SIZE_SHA256] = {0};
    size_t                    hash_len                          = NRF_CRYPTO_HASH_SIZE_SHA256;
    nrf_crypto_hash_context_t hash_ctx;
    nordic_err_code = nrf_crypto_hash_calculate(
        &hash_ctx,
        &g_nrf_crypto_hash_sha256_info,
        firmware_binary, sizeof(firmware_binary),
        hash, &hash_len);
    if (NRF_SUCCESS != nordic_err_code)
    {
        return false;
    }

    // Verify using public contained in certificate in oid slot
    nrf_crypto_ecc_public_key_t pub_key = NRF_CRYPTO_INFINEON_SECP256R1_PUBLIC_KEY_FROM_OID(oid);
    nordic_err_code = nrf_crypto_ecdsa_verify(NULL, &pub_key, hash, hash_len, sig_rs, sig_rs_len);
    NRF_LOG_INFO("Verify signature with public key certificate(OID=0x%4x): %s",
                 oid, nordic_err_code == NRF_SUCCESS ? "PASS" : "FAIL");
    if (NRF_SUCCESS != nordic_err_code)
    {
        return false;
    }

    return true;
}
