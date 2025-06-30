// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Copyright (c) 2025 The Mytherra Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MYTHERRA_WALLET_SALVAGE_H
#define MYTHERRA_WALLET_SALVAGE_H

#include <streams.h>
#include <util/fs.h>

class ArgsManager;
struct bilingual_str;

namespace wallet {
bool RecoverDatabaseFile(const ArgsManager& args, const fs::path& file_path, bilingual_str& error, std::vector<bilingual_str>& warnings);
} // namespace wallet

#endif // MYTHERRA_WALLET_SALVAGE_H
