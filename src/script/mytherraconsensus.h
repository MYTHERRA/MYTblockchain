// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Copyright (c) 2025 The Mytherra Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MYTHERRA_SCRIPT_MYTHERRACONSENSUS_H
#define MYTHERRA_SCRIPT_MYTHERRACONSENSUS_H

#include <stdint.h>

#if defined(BUILD_MYTHERRA_INTERNAL) && defined(HAVE_CONFIG_H)
#include <config/mytherra-config.h>
  #if defined(_WIN32)
    #if defined(HAVE_DLLEXPORT_ATTRIBUTE)
      #define EXPORT_SYMBOL __declspec(dllexport)
    #else
      #define EXPORT_SYMBOL
    #endif
  #elif defined(HAVE_DEFAULT_VISIBILITY_ATTRIBUTE)
    #define EXPORT_SYMBOL __attribute__ ((visibility ("default")))
  #endif
#elif defined(MSC_VER) && !defined(STATIC_LIBMYTHERRACONSENSUS)
  #define EXPORT_SYMBOL __declspec(dllimport)
#endif

#ifndef EXPORT_SYMBOL
  #define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MYTHERRACONSENSUS_API_VER 1

typedef enum mytherraconsensus_error_t
{
    mytherraconsensus_ERR_OK = 0,
    mytherraconsensus_ERR_TX_INDEX,
    mytherraconsensus_ERR_TX_SIZE_MISMATCH,
    mytherraconsensus_ERR_TX_DESERIALIZE,
    mytherraconsensus_ERR_AMOUNT_REQUIRED,
    mytherraconsensus_ERR_INVALID_FLAGS,
} mytherraconsensus_error;

/** Script verification flags */
enum
{
    mytherraconsensus_SCRIPT_FLAGS_VERIFY_NONE                = 0,
    mytherraconsensus_SCRIPT_FLAGS_VERIFY_P2SH                = (1U << 0), // evaluate P2SH (BIP16) subscripts
    mytherraconsensus_SCRIPT_FLAGS_VERIFY_DERSIG              = (1U << 2), // enforce strict DER (BIP66) compliance
    mytherraconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY           = (1U << 4), // enforce NULLDUMMY (BIP147)
    mytherraconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY = (1U << 9), // enable CHECKLOCKTIMEVERIFY (BIP65)
    mytherraconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY = (1U << 10), // enable CHECKSEQUENCEVERIFY (BIP112)
    mytherraconsensus_SCRIPT_FLAGS_VERIFY_WITNESS             = (1U << 11), // enable WITNESS (BIP141)
    mytherraconsensus_SCRIPT_FLAGS_VERIFY_ALL                 = mytherraconsensus_SCRIPT_FLAGS_VERIFY_P2SH | mytherraconsensus_SCRIPT_FLAGS_VERIFY_DERSIG |
                                                               mytherraconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY | mytherraconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY |
                                                               mytherraconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY | mytherraconsensus_SCRIPT_FLAGS_VERIFY_WITNESS
};

/// Returns 1 if the input nIn of the serialized transaction pointed to by
/// txTo correctly spends the scriptPubKey pointed to by scriptPubKey under
/// the additional constraints specified by flags.
/// If not nullptr, err will contain an error/success code for the operation
EXPORT_SYMBOL int mytherraconsensus_verify_script(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen,
                                                 const unsigned char *txTo        , unsigned int txToLen,
                                                 unsigned int nIn, unsigned int flags, mytherraconsensus_error* err);

EXPORT_SYMBOL int mytherraconsensus_verify_script_with_amount(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen, int64_t amount,
                                    const unsigned char *txTo        , unsigned int txToLen,
                                    unsigned int nIn, unsigned int flags, mytherraconsensus_error* err);

EXPORT_SYMBOL unsigned int mytherraconsensus_version();

#ifdef __cplusplus
} // extern "C"
#endif

#undef EXPORT_SYMBOL

#endif // MYTHERRA_SCRIPT_MYTHERRACONSENSUS_H
