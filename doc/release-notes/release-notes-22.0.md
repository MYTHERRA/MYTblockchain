22.0 Release Notes
==================

Mytherra Core version 22.0 is now available from:

  <https://bitcoincore.org/bin/mytherra-core-22.0/>

This release includes new features, various bug fixes and performance
improvements, as well as updated translations.

Please report bugs using the issue tracker at GitHub:

  <https://github.com/mytherra/mytherra/issues>

To receive security and update notifications, please subscribe to:

  <https://bitcoincore.org/en/list/announcements/join/>

How to Upgrade
==============

If you are running an older version, shut it down. Wait until it has completely
shut down (which might take a few minutes in some cases), then run the
installer (on Windows) or just copy over `/Applications/Mytherra-Qt` (on Mac)
or `mytherrad`/`mytherra-qt` (on Linux).

Upgrading directly from a version of Mytherra Core that has reached its EOL is
possible, but it might take some time if the data directory needs to be migrated. Old
wallet versions of Mytherra Core are generally supported.

Compatibility
==============

Mytherra Core is supported and extensively tested on operating systems
using the Linux kernel, macOS 10.14+, and Windows 7 and newer.  Mytherra
Core should also work on most other Unix-like systems but is not as
frequently tested on them.  It is not recommended to use Mytherra Core on
unsupported systems.

From Mytherra Core 22.0 onwards, macOS versions earlier than 10.14 are no longer supported.

Notable changes
===============

P2P and network changes
-----------------------
- Added support for running Mytherra Core as an
  [I2P (Invisible Internet Project)](https://en.wikipedia.org/wiki/I2P) service
  and connect to such services. See [i2p.md](https://github.com/mytherra/mytherra/blob/22.x/doc/i2p.md) for details. (#20685)
- This release removes support for Tor version 2 hidden services in favor of Tor
  v3 only, as the Tor network [dropped support for Tor
  v2](https://blog.torproject.org/v2-deprecation-timeline) with the release of
  Tor version 0.4.6.  Henceforth, Mytherra Core ignores Tor v2 addresses; it
  neither rumors them over the network to other peers, nor stores them in memory
  or to `peers.dat`.  (#22050)

- Added NAT-PMP port mapping support via
  [`libnatpmp`](https://miniupnp.tuxfamily.org/libnatpmp.html). (#18077)

New and Updated RPCs
--------------------

- Due to [BIP 350](https://github.com/mytherra/bips/blob/master/bip-0350.mediawiki)
  being implemented, behavior for all RPCs that accept addresses is changed when
  a native witness version 1 (or higher) is passed. These now require a Bech32m
  encoding instead of a Bech32 one, and Bech32m encoding will be used for such
  addresses in RPC output as well. No version 1 addresses should be created
  for mainnet until consensus rules are adopted that give them meaning
  (as will happen through [BIP 341](https://github.com/mytherra/bips/blob/master/bip-0341.mediawiki)).
  Once that happens, Bech32m is expected to be used for them, so this shouldn't
  affect any production systems, but may be observed on other networks where such
  addresses already have meaning (like signet). (#20861)

- The `getpeerinfo` RPC returns two new boolean fields, `bip152_hb_to` and
  `bip152_hb_from`, that respectively indicate whether we selected a peer to be
  in compact blocks high-bandwidth mode or whether a peer selected us as a
  compact blocks high-bandwidth peer. High-bandwidth peers send new block
  announcements via a `cmpctblock` message rather than the usual inv/headers
  announcements. See BIP 152 for more details. (#19776)

- `getpeerinfo` no longer returns the following fields: `addnode`, `banscore`,
  and `whitelisted`, which were previously deprecated in 0.21. Instead of
  `addnode`, the `connection_type` field returns manual. Instead of
  `whitelisted`, the `permissions` field indicates if the peer has special
  privileges. The `banscore` field has simply been removed. (#20755)

- The following RPCs:  `gettxout`, `getrawtransaction`, `decoderawtransaction`,
  `decodescript`, `gettransaction`, and REST endpoints: `/rest/tx`,
  `/rest/getutxos`, `/rest/block` deprecated the following fields (which are no
  longer returned in the responses by default): `addresses`, `reqSigs`.
  The `-deprecatedrpc=addresses` flag must be passed for these fields to be
  included in the RPC response. This flag/option will be available only for this major release, after which
  the deprecation will be removed entirely. Note that these fields are attributes of
  the `scriptPubKey` object returned in the RPC response. However, in the response
  of `decodescript` these fields are top-level attributes, and included again as attributes
  of the `scriptPubKey` object. (#20286)

- When creating a hex-encoded mytherra transaction using the `mytherra-tx` utility
  with the `-json` option set, the following fields: `addresses`, `reqSigs` are no longer
  returned in the tx output of the response. (#20286)

- The `listbanned` RPC now returns two new numeric fields: `ban_duration` and `time_remaining`.
  Respectively, these new fields indicate the duration of a ban and the time remaining until a ban expires,
  both in seconds. Additionally, the `ban_created` field is repositioned to come before `banned_until`. (#21602)

- The `setban` RPC can ban onion addresses again. This fixes a regression introduced in version 0.21.0. (#20852)

- The `getnodeaddresses` RPC now returns a "network" field indicating the
  network type (ipv4, ipv6, onion, or i2p) for each address.  (#21594)

- `getnodeaddresses` now also accepts a "network" argument (ipv4, ipv6, onion,
  or i2p) to return only addresses of the specified network.  (#21843)

- The `testmempoolaccept` RPC now accepts multiple transactions (still experimental at the moment,
  API may be unstable). This is intended for testing transaction packages with dependency
  relationships; it is not recommended for batch-validating independent transactions. In addition to
  mempool policy, package policies apply: the list cannot contain more than 25 transactions or have a
  total size exceeding 101K virtual bytes, and cannot conflict with (spend the same inputs as) each other or
  the mempool, even if it would be a valid BIP125 replace-by-fee. There are some known limitations to
  the accuracy of the test accept: it's possible for `testmempoolaccept` to return "allowed"=True for a
  group of transactions, but "too-long-mempool-chain" if they are actually submitted. (#20833)

- `addmultisigaddress` and `createmultisig` now support up to 20 keys for
  Segwit addresses. (#20867)

Changes to Wallet or GUI related RPCs can be found in the GUI or Wallet section below.

Build System
------------

- Release binaries are now produced using the new `guix`-based build system.
  The [/doc/release-process.md](/doc/release-process.md) document has been updated accordingly.

Files
-----

- The list of banned hosts and networks (via `setban` RPC) is now saved on disk
  in JSON format in `banlist.json` instead of `banlist.dat`. `banlist.dat` is
  only read on startup if `banlist.json` is not present. Changes are only written to the new
  `banlist.json`. A future version of Mytherra Core may completely ignore
  `banlist.dat`. (#20966)

New settings
------------

- The `-natpmp` option has been added to use NAT-PMP to map the listening port.
  If both UPnP and NAT-PMP are enabled, a successful allocation from UPnP
  prevails over one from NAT-PMP. (#18077)

Updated settings
----------------

Changes to Wallet or GUI related settings can be found in the GUI or Wallet section below.

- Passing an invalid `-rpcauth` argument now cause mytherrad to fail to start.  (#20461)

Tools and Utilities
-------------------

- A new CLI `-addrinfo` command returns the number of addresses known to the
  node per network type (including Tor v2 versus v3) and total. This can be
  useful to see if the node knows enough addresses in a network to use options
  like `-onlynet=<network>` or to upgrade to this release of Mytherra Core 22.0
  that supports Tor v3 only.  (#21595)

- A new `-rpcwaittimeout` argument to `mytherra-cli` sets the timeout
  in seconds to use with `-rpcwait`. If the timeout expires,
  `mytherra-cli` will report a failure. (#21056)

Wallet
------

- External signers such as hardware wallets can now be used through the new RPC methods `enumeratesigners` and `displayaddress`. Support is also added to the `send` RPC call. This feature is experimental. See [external-signer.md](https://github.com/mytherra/mytherra/blob/22.x/doc/external-signer.md) for details. (#16546)

- A new `listdescriptors` RPC is available to inspect the contents of descriptor-enabled wallets.
  The RPC returns public versions of all imported descriptors, including their timestamp and flags.
  For ranged descriptors, it also returns the range boundaries and the next index to generate addresses from. (#20226)

- The `bumpfee` RPC is not available with wallets that have private keys
  disabled. `psbtbumpfee` can be used instead. (#20891)

- The `fundrawtransaction`, `send` and `walletcreatefundedpsbt` RPCs now support an `include_unsafe` option
  that when `true` allows using unsafe inputs to fund the transaction.
  Note that the resulting transaction may become invalid if one of the unsafe inputs disappears.
  If that happens, the transaction must be funded with different inputs and republished. (#21359)

- We now support up to 20 keys in `multi()` and `sortedmulti()` descriptors
  under `wsh()`. (#20867)

- Taproot descriptors can be imported into the wallet only after activation has occurred on the network (e.g. mainnet, testnet, signet) in use. See [descriptors.md](https://github.com/mytherra/mytherra/blob/22.x/doc/descriptors.md) for supported descriptors.

GUI changes
-----------

- External signers such as hardware wallets can now be used. These require an external tool such as [HWI](https://github.com/mytherra-core/HWI) to be installed and configured under Options -> Wallet. When creating a new wallet a new option "External signer" will appear in the dialog. If the device is detected, its name is suggested as the wallet name. The watch-only keys are then automatically imported. Receive addresses can be verified on the device. The send dialog will automatically use the connected device. This feature is experimental and the UI may freeze for a few seconds when performing these actions.

Low-level changes
=================

RPC
---

- The RPC server can process a limited number of simultaneous RPC requests.
  Previously, if this limit was exceeded, the RPC server would respond with
  [status code 500 (`HTTP_INTERNAL_SERVER_ERROR`)](https://en.wikipedia.org/wiki/List_of_HTTP_status_codes#5xx_server_errors).
  Now it returns status code 503 (`HTTP_SERVICE_UNAVAILABLE`). (#18335)

- Error codes have been updated to be more accurate for the following error cases (#18466):
  - `signmessage` now returns RPC_INVALID_ADDRESS_OR_KEY (-5) if the
    passed address is invalid. Previously returned RPC_TYPE_ERROR (-3).
  - `verifymessage` now returns RPC_INVALID_ADDRESS_OR_KEY (-5) if the
    passed address is invalid. Previously returned RPC_TYPE_ERROR (-3).
  - `verifymessage` now returns RPC_TYPE_ERROR (-3) if the passed signature
    is malformed. Previously returned RPC_INVALID_ADDRESS_OR_KEY (-5).

Tests
-----

22.0 change log
===============

A detailed list of changes in this version follows. To keep the list to a manageable length, small refactors and typo fixes are not included, and similar changes are sometimes condensed into one line.

### Consensus
- mytherra/mytherra#19438 Introduce deploymentstatus (ajtowns)
- mytherra/mytherra#20207 Follow-up extra comments on taproot code and tests (sipa)
- mytherra/mytherra#21330 Deal with missing data in signature hashes more consistently (sipa)

### Policy
- mytherra/mytherra#18766 Disable fee estimation in blocksonly mode (by removing the fee estimates global) (darosior)
- mytherra/mytherra#20497 Add `MAX_STANDARD_SCRIPTSIG_SIZE` to policy (sanket1729)
- mytherra/mytherra#20611 Move `TX_MAX_STANDARD_VERSION` to policy (MarcoFalke)

### Mining
- mytherra/mytherra#19937, mytherra/mytherra#20923 Signet mining utility (ajtowns)

### Block and transaction handling
- mytherra/mytherra#14501 Fix possible data race when committing block files (luke-jr)
- mytherra/mytherra#15946 Allow maintaining the blockfilterindex when using prune (jonasschnelli)
- mytherra/mytherra#18710 Add local thread pool to CCheckQueue (hebasto)
- mytherra/mytherra#19521 Coinstats Index (fjahr)
- mytherra/mytherra#19806 UTXO snapshot activation (jamesob)
- mytherra/mytherra#19905 Remove dead CheckForkWarningConditionsOnNewFork (MarcoFalke)
- mytherra/mytherra#19935 Move SaltedHashers to separate file and add some new ones (achow101)
- mytherra/mytherra#20054 Remove confusing and useless "unexpected version" warning (MarcoFalke)
- mytherra/mytherra#20519 Handle rename failure in `DumpMempool(…)` by using the `RenameOver(…)` return value (practicalswift)
- mytherra/mytherra#20749, mytherra/mytherra#20750, mytherra/mytherra#21055, mytherra/mytherra#21270, mytherra/mytherra#21525, mytherra/mytherra#21391, mytherra/mytherra#21767, mytherra/mytherra#21866 Prune `g_chainman` usage (dongcarl)
- mytherra/mytherra#20833 rpc/validation: enable packages through testmempoolaccept (glozow)
- mytherra/mytherra#20834 Locks and docs in ATMP and CheckInputsFromMempoolAndCache (glozow)
- mytherra/mytherra#20854 Remove unnecessary try-block (amitiuttarwar)
- mytherra/mytherra#20868 Remove redundant check on pindex (jarolrod)
- mytherra/mytherra#20921 Don't try to invalidate genesis block in CChainState::InvalidateBlock (theStack)
- mytherra/mytherra#20972 Locks: Annotate CTxMemPool::check to require `cs_main` (dongcarl)
- mytherra/mytherra#21009 Remove RewindBlockIndex logic (dhruv)
- mytherra/mytherra#21025 Guard chainman chainstates with `cs_main` (dongcarl)
- mytherra/mytherra#21202 Two small clang lock annotation improvements (amitiuttarwar)
- mytherra/mytherra#21523 Run VerifyDB on all chainstates (jamesob)
- mytherra/mytherra#21573 Update libsecp256k1 subtree to latest master (sipa)
- mytherra/mytherra#21582, mytherra/mytherra#21584, mytherra/mytherra#21585 Fix assumeutxo crashes (MarcoFalke)
- mytherra/mytherra#21681 Fix ActivateSnapshot to use hardcoded nChainTx (jamesob)
- mytherra/mytherra#21796 index: Avoid async shutdown on init error (MarcoFalke)
- mytherra/mytherra#21946 Document and test lack of inherited signaling in RBF policy (ariard)
- mytherra/mytherra#22084 Package testmempoolaccept followups (glozow)
- mytherra/mytherra#22102 Remove `Warning:` from warning message printed for unknown new rules (prayank23)
- mytherra/mytherra#22112 Force port 0 in I2P (vasild)
- mytherra/mytherra#22135 CRegTestParams: Use `args` instead of `gArgs` (kiminuo)
- mytherra/mytherra#22146 Reject invalid coin height and output index when loading assumeutxo (MarcoFalke)
- mytherra/mytherra#22253 Distinguish between same tx and same-nonwitness-data tx in mempool (glozow)
- mytherra/mytherra#22261 Two small fixes to node broadcast logic (jnewbery)
- mytherra/mytherra#22415 Make `m_mempool` optional in CChainState (jamesob)
- mytherra/mytherra#22499 Update assumed chain params (sriramdvt)
- mytherra/mytherra#22589 net, doc: update I2P hardcoded seeds and docs for 22.0 (jonatack)

### P2P protocol and network code
- mytherra/mytherra#18077 Add NAT-PMP port forwarding support (hebasto)
- mytherra/mytherra#18722 addrman: improve performance by using more suitable containers (vasild)
- mytherra/mytherra#18819 Replace `cs_feeFilter` with simple std::atomic (MarcoFalke)
- mytherra/mytherra#19203 Add regression fuzz harness for CVE-2017-18350. Add FuzzedSocket (practicalswift)
- mytherra/mytherra#19288 fuzz: Add fuzzing harness for TorController (practicalswift)
- mytherra/mytherra#19415 Make DNS lookup mockable, add fuzzing harness (practicalswift)
- mytherra/mytherra#19509 Per-Peer Message Capture (troygiorshev)
- mytherra/mytherra#19763 Don't try to relay to the address' originator (vasild)
- mytherra/mytherra#19771 Replace enum CConnMan::NumConnections with enum class ConnectionDirection (luke-jr)
- mytherra/mytherra#19776 net, rpc: expose high bandwidth mode state via getpeerinfo (theStack)
- mytherra/mytherra#19832 Put disconnecting logs into BCLog::NET category (hebasto)
- mytherra/mytherra#19858 Periodically make block-relay connections and sync headers (sdaftuar)
- mytherra/mytherra#19884 No delay in adding fixed seeds if -dnsseed=0 and peers.dat is empty (dhruv)
- mytherra/mytherra#20079 Treat handshake misbehavior like unknown message (MarcoFalke)
- mytherra/mytherra#20138 Assume that SetCommonVersion is called at most once per peer (MarcoFalke)
- mytherra/mytherra#20162 p2p: declare Announcement::m_state as uint8_t, add getter/setter (jonatack)
- mytherra/mytherra#20197 Protect onions in AttemptToEvictConnection(), add eviction protection test coverage (jonatack)
- mytherra/mytherra#20210 assert `CNode::m_inbound_onion` is inbound in ctor, add getter, unit tests (jonatack)
- mytherra/mytherra#20228 addrman: Make addrman a top-level component (jnewbery)
- mytherra/mytherra#20234 Don't bind on 0.0.0.0 if binds are restricted to Tor (vasild)
- mytherra/mytherra#20477 Add unit testing of node eviction logic (practicalswift)
- mytherra/mytherra#20516 Well-defined CAddress disk serialization, and addrv2 anchors.dat (sipa)
- mytherra/mytherra#20557 addrman: Fix new table bucketing during unserialization (jnewbery)
- mytherra/mytherra#20561 Periodically clear `m_addr_known` (sdaftuar)
- mytherra/mytherra#20599 net processing: Tolerate sendheaders and sendcmpct messages before verack (jnewbery)
- mytherra/mytherra#20616 Check CJDNS address is valid (lontivero)
- mytherra/mytherra#20617 Remove `m_is_manual_connection` from CNodeState (ariard)
- mytherra/mytherra#20624 net processing: Remove nStartingHeight check from block relay (jnewbery)
- mytherra/mytherra#20651 Make p2p recv buffer timeout 20 minutes for all peers (jnewbery)
- mytherra/mytherra#20661 Only select from addrv2-capable peers for torv3 address relay (sipa)
- mytherra/mytherra#20685 Add I2P support using I2P SAM (vasild)
- mytherra/mytherra#20690 Clean up logging of outbound connection type (sdaftuar)
- mytherra/mytherra#20721 Move ping data to `net_processing` (jnewbery)
- mytherra/mytherra#20724 Cleanup of -debug=net log messages (ajtowns)
- mytherra/mytherra#20747 net processing: Remove dropmessagestest (jnewbery)
- mytherra/mytherra#20764 cli -netinfo peer connections dashboard updates 🎄 ✨ (jonatack)
- mytherra/mytherra#20788 add RAII socket and use it instead of bare SOCKET (vasild)
- mytherra/mytherra#20791 remove unused legacyWhitelisted in AcceptConnection() (jonatack)
- mytherra/mytherra#20816 Move RecordBytesSent() call out of `cs_vSend` lock (jnewbery)
- mytherra/mytherra#20845 Log to net debug in MaybeDiscourageAndDisconnect except for noban and manual peers (MarcoFalke)
- mytherra/mytherra#20864 Move SocketSendData lock annotation to header (MarcoFalke)
- mytherra/mytherra#20965 net, rpc:  return `NET_UNROUTABLE` as `not_publicly_routable`, automate helps (jonatack)
- mytherra/mytherra#20966 banman: save the banlist in a JSON format on disk (vasild)
- mytherra/mytherra#21015 Make all of `net_processing` (and some of net) use std::chrono types (dhruv)
- mytherra/mytherra#21029 mytherra-cli: Correct docs (no "generatenewaddress" exists) (luke-jr)
- mytherra/mytherra#21148 Split orphan handling from `net_processing` into txorphanage (ajtowns)
- mytherra/mytherra#21162 Net Processing: Move RelayTransaction() into PeerManager (jnewbery)
- mytherra/mytherra#21167 make `CNode::m_inbound_onion` public, initialize explicitly (jonatack)
- mytherra/mytherra#21186 net/net processing: Move addr data into `net_processing` (jnewbery)
- mytherra/mytherra#21187 Net processing: Only call PushAddress() from `net_processing` (jnewbery)
- mytherra/mytherra#21198 Address outstanding review comments from PR20721 (jnewbery)
- mytherra/mytherra#21222 log: Clarify log message when file does not exist (MarcoFalke)
- mytherra/mytherra#21235 Clarify disconnect log message in ProcessGetBlockData, remove send bool (MarcoFalke)
- mytherra/mytherra#21236 Net processing: Extract `addr` send functionality into MaybeSendAddr() (jnewbery)
- mytherra/mytherra#21261 update inbound eviction protection for multiple networks, add I2P peers (jonatack)
- mytherra/mytherra#21328 net, refactor: pass uint16 CService::port as uint16 (jonatack)
- mytherra/mytherra#21387 Refactor sock to add I2P fuzz and unit tests (vasild)
- mytherra/mytherra#21395 Net processing: Remove unused CNodeState.address member (jnewbery)
- mytherra/mytherra#21407 i2p: limit the size of incoming messages (vasild)
- mytherra/mytherra#21506 p2p, refactor: make NetPermissionFlags an enum class (jonatack)
- mytherra/mytherra#21509 Don't send FEEFILTER in blocksonly mode (mzumsande)
- mytherra/mytherra#21560 Add Tor v3 hardcoded seeds (laanwj)
- mytherra/mytherra#21563 Restrict period when `cs_vNodes` mutex is locked (hebasto)
- mytherra/mytherra#21564 Avoid calling getnameinfo when formatting IPv4 addresses in CNetAddr::ToStringIP (practicalswift)
- mytherra/mytherra#21631 i2p: always check the return value of Sock::Wait() (vasild)
- mytherra/mytherra#21644 p2p, bugfix: use NetPermissions::HasFlag() in CConnman::Bind() (jonatack)
- mytherra/mytherra#21659 flag relevant Sock methods with [[nodiscard]] (vasild)
- mytherra/mytherra#21750 remove unnecessary check of `CNode::cs_vSend` (vasild)
- mytherra/mytherra#21756 Avoid calling `getnameinfo` when formatting IPv6 addresses in `CNetAddr::ToStringIP` (practicalswift)
- mytherra/mytherra#21775 Limit `m_block_inv_mutex` (MarcoFalke)
- mytherra/mytherra#21825 Add I2P hardcoded seeds (jonatack)
- mytherra/mytherra#21843 p2p, rpc: enable GetAddr, GetAddresses, and getnodeaddresses by network (jonatack)
- mytherra/mytherra#21845 net processing: Don't require locking `cs_main` before calling RelayTransactions() (jnewbery)
- mytherra/mytherra#21872 Sanitize message type for logging (laanwj)
- mytherra/mytherra#21914 Use stronger AddLocal() for our I2P address (vasild)
- mytherra/mytherra#21985 Return IPv6 scope id in `CNetAddr::ToStringIP()` (laanwj)
- mytherra/mytherra#21992 Remove -feefilter option (amadeuszpawlik)
- mytherra/mytherra#21996 Pass strings to NetPermissions::TryParse functions by const ref (jonatack)
- mytherra/mytherra#22013 ignore block-relay-only peers when skipping DNS seed (ajtowns)
- mytherra/mytherra#22050 Remove tor v2 support (jonatack)
- mytherra/mytherra#22096 AddrFetch - don't disconnect on self-announcements (mzumsande)
- mytherra/mytherra#22141 net processing: Remove hash and fValidatedHeaders from QueuedBlock (jnewbery)
- mytherra/mytherra#22144 Randomize message processing peer order (sipa)
- mytherra/mytherra#22147 Protect last outbound HB compact block peer (sdaftuar)
- mytherra/mytherra#22179 Torv2 removal followups (vasild)
- mytherra/mytherra#22211 Relay I2P addresses even if not reachable (by us) (vasild)
- mytherra/mytherra#22284 Performance improvements to ProtectEvictionCandidatesByRatio() (jonatack)
- mytherra/mytherra#22387 Rate limit the processing of rumoured addresses (sipa)
- mytherra/mytherra#22455 addrman: detect on-disk corrupted nNew and nTried during unserialization (vasild)

### Wallet
- mytherra/mytherra#15710 Catch `ios_base::failure` specifically (Bushstar)
- mytherra/mytherra#16546 External signer support - Wallet Box edition (Sjors)
- mytherra/mytherra#17331 Use effective values throughout coin selection (achow101)
- mytherra/mytherra#18418 Increase `OUTPUT_GROUP_MAX_ENTRIES` to 100 (fjahr)
- mytherra/mytherra#18842 Mark replaced tx to not be in the mempool anymore (MarcoFalke)
- mytherra/mytherra#19136 Add `parent_desc` to `getaddressinfo` (achow101)
- mytherra/mytherra#19137 wallettool: Add dump and createfromdump commands (achow101)
- mytherra/mytherra#19651 `importdescriptor`s update existing (S3RK)
- mytherra/mytherra#20040 Refactor OutputGroups to handle fees and spending eligibility on grouping (achow101)
- mytherra/mytherra#20202 Make BDB support optional (achow101)
- mytherra/mytherra#20226, mytherra/mytherra#21277, - mytherra/mytherra#21063 Add `listdescriptors` command (S3RK)
- mytherra/mytherra#20267 Disable and fix tests for when BDB is not compiled (achow101)
- mytherra/mytherra#20275 List all wallets in non-SQLite and non-BDB builds (ryanofsky)
- mytherra/mytherra#20365 wallettool: Add parameter to create descriptors wallet (S3RK)
- mytherra/mytherra#20403 `upgradewallet` fixes, improvements, test coverage (jonatack)
- mytherra/mytherra#20448 `unloadwallet`: Allow specifying `wallet_name` param matching RPC endpoint wallet (luke-jr)
- mytherra/mytherra#20536 Error with "Transaction too large" if the funded tx will end up being too large after signing (achow101)
- mytherra/mytherra#20687 Add missing check for -descriptors wallet tool option (MarcoFalke)
- mytherra/mytherra#20952 Add BerkeleyDB version sanity check at init time (laanwj)
- mytherra/mytherra#21127 Load flags before everything else (Sjors)
- mytherra/mytherra#21141 Add new format string placeholders for walletnotify (maayank)
- mytherra/mytherra#21238 A few descriptor improvements to prepare for Taproot support (sipa)
- mytherra/mytherra#21302 `createwallet` examples for descriptor wallets (S3RK)
- mytherra/mytherra#21329 descriptor wallet: Cache last hardened xpub and use in normalized descriptors (achow101)
- mytherra/mytherra#21365 Basic Taproot signing support for descriptor wallets (sipa)
- mytherra/mytherra#21417 Misc external signer improvement and HWI 2 support (Sjors)
- mytherra/mytherra#21467 Move external signer out of wallet module (Sjors)
- mytherra/mytherra#21572 Fix wrong wallet RPC context set after #21366 (ryanofsky)
- mytherra/mytherra#21574 Drop JSONRPCRequest constructors after #21366 (ryanofsky)
- mytherra/mytherra#21666 Miscellaneous external signer changes (fanquake)
- mytherra/mytherra#21759 Document coin selection code (glozow)
- mytherra/mytherra#21786 Ensure sat/vB feerates are in range (mantissa of 3) (jonatack)
- mytherra/mytherra#21944 Fix issues when `walletdir` is root directory (prayank23)
- mytherra/mytherra#22042 Replace size/weight estimate tuple with struct for named fields (instagibbs)
- mytherra/mytherra#22051 Basic Taproot derivation support for descriptors (sipa)
- mytherra/mytherra#22154 Add OutputType::BECH32M and related wallet support for fetching bech32m addresses (achow101)
- mytherra/mytherra#22156 Allow tr() import only when Taproot is active (achow101)
- mytherra/mytherra#22166 Add support for inferring tr() descriptors (sipa)
- mytherra/mytherra#22173 Do not load external signers wallets when unsupported (achow101)
- mytherra/mytherra#22308 Add missing BlockUntilSyncedToCurrentChain (MarcoFalke)
- mytherra/mytherra#22334 Do not spam about non-existent spk managers (S3RK)
- mytherra/mytherra#22379 Erase spkmans rather than setting to nullptr (achow101)
- mytherra/mytherra#22421 Make IsSegWitOutput return true for taproot outputs (sipa)
- mytherra/mytherra#22461 Change ScriptPubKeyMan::Upgrade default to True (achow101)
- mytherra/mytherra#22492 Reorder locks in dumpwallet to avoid lock order assertion (achow101)
- mytherra/mytherra#22686 Use GetSelectionAmount in ApproximateBestSubset (achow101)

### RPC and other APIs
- mytherra/mytherra#18335, mytherra/mytherra#21484 cli: Print useful error if mytherrad rpc work queue exceeded (LarryRuane)
- mytherra/mytherra#18466 Fix invalid parameter error codes for `{sign,verify}message` RPCs (theStack)
- mytherra/mytherra#18772 Calculate fees in `getblock` using BlockUndo data (robot-visions)
- mytherra/mytherra#19033 http: Release work queue after event base finish (promag)
- mytherra/mytherra#19055 Add MuHash3072 implementation (fjahr)
- mytherra/mytherra#19145 Add `hash_type` MUHASH for gettxoutsetinfo (fjahr)
- mytherra/mytherra#19847 Avoid duplicate set lookup in `gettxoutproof` (promag)
- mytherra/mytherra#20286 Deprecate `addresses` and `reqSigs` from RPC outputs (mjdietzx)
- mytherra/mytherra#20459 Fail to return undocumented return values (MarcoFalke)
- mytherra/mytherra#20461 Validate `-rpcauth` arguments (promag)
- mytherra/mytherra#20556 Properly document return values (`submitblock`, `gettxout`, `getblocktemplate`, `scantxoutset`) (MarcoFalke)
- mytherra/mytherra#20755 Remove deprecated fields from `getpeerinfo` (amitiuttarwar)
- mytherra/mytherra#20832 Better error messages for invalid addresses (eilx2)
- mytherra/mytherra#20867 Support up to 20 keys for multisig under Segwit context (darosior)
- mytherra/mytherra#20877 cli: `-netinfo` user help and argument parsing improvements (jonatack)
- mytherra/mytherra#20891 Remove deprecated bumpfee behavior (achow101)
- mytherra/mytherra#20916 Return wtxid from `testmempoolaccept` (MarcoFalke)
- mytherra/mytherra#20917 Add missing signet mentions in network name lists (theStack)
- mytherra/mytherra#20941 Document `RPC_TRANSACTION_ALREADY_IN_CHAIN` exception (jarolrod)
- mytherra/mytherra#20944 Return total fee in `getmempoolinfo` (MarcoFalke)
- mytherra/mytherra#20964 Add specific error code for "wallet already loaded" (laanwj)
- mytherra/mytherra#21053 Document {previous,next}blockhash as optional (theStack)
- mytherra/mytherra#21056 Add a `-rpcwaittimeout` parameter to limit time spent waiting (cdecker)
- mytherra/mytherra#21192 cli: Treat high detail levels as maximum in `-netinfo` (laanwj)
- mytherra/mytherra#21311 Document optional fields for `getchaintxstats` result (theStack)
- mytherra/mytherra#21359 `include_unsafe` option for fundrawtransaction (t-bast)
- mytherra/mytherra#21426 Remove `scantxoutset` EXPERIMENTAL warning (jonatack)
- mytherra/mytherra#21544 Missing doc updates for bumpfee psbt update (MarcoFalke)
- mytherra/mytherra#21594 Add `network` field to `getnodeaddresses` (jonatack)
- mytherra/mytherra#21595, mytherra/mytherra#21753 cli: Create `-addrinfo` (jonatack)
- mytherra/mytherra#21602 Add additional ban time fields to `listbanned` (jarolrod)
- mytherra/mytherra#21679 Keep default argument value in correct type (promag)
- mytherra/mytherra#21718 Improve error message for `getblock` invalid datatype (klementtan)
- mytherra/mytherra#21913 RPCHelpMan fixes (kallewoof)
- mytherra/mytherra#22021 `bumpfee`/`psbtbumpfee` fixes and updates (jonatack)
- mytherra/mytherra#22043 `addpeeraddress` test coverage, code simplify/constness (jonatack)
- mytherra/mytherra#22327 cli: Avoid truncating `-rpcwaittimeout` (MarcoFalke)

### GUI
- mytherra/mytherra#18948 Call setParent() in the parent's context (hebasto)
- mytherra/mytherra#20482 Add depends qt fix for ARM macs (jonasschnelli)
- mytherra/mytherra#21836 scripted-diff: Replace three dots with ellipsis in the ui strings (hebasto)
- mytherra/mytherra#21935 Enable external signer support for GUI builds (Sjors)
- mytherra/mytherra#22133 Make QWindowsVistaStylePlugin available again (regression) (hebasto)
- mytherra-core/gui#4 UI external signer support (e.g. hardware wallet) (Sjors)
- mytherra-core/gui#13 Hide peer detail view if multiple are selected (promag)
- mytherra-core/gui#18 Add peertablesortproxy module (hebasto)
- mytherra-core/gui#21 Improve pruning tooltip (fluffypony, MytherraErrorLog)
- mytherra-core/gui#72 Log static plugins meta data and used style (hebasto)
- mytherra-core/gui#79 Embed monospaced font (hebasto)
- mytherra-core/gui#85 Remove unused "What's This" button in dialogs on Windows OS (hebasto)
- mytherra-core/gui#115 Replace "Hide tray icon" option with positive "Show tray icon" one (hebasto)
- mytherra-core/gui#118 Remove BDB version from the Information tab (hebasto)
- mytherra-core/gui#121 Early subscribe core signals in transaction table model (promag)
- mytherra-core/gui#123 Do not accept command while executing another one (hebasto)
- mytherra-core/gui#125 Enable changing the autoprune block space size in intro dialog (luke-jr)
- mytherra-core/gui#138 Unlock encrypted wallet "OK" button bugfix (mjdietzx)
- mytherra-core/gui#139 doc: Improve gui/src/qt README.md (jarolrod)
- mytherra-core/gui#154 Support macOS Dark mode (goums, Uplab)
- mytherra-core/gui#162 Add network to peers window and peer details (jonatack)
- mytherra-core/gui#163, mytherra-core/gui#180 Peer details: replace Direction with Connection Type (jonatack)
- mytherra-core/gui#164 Handle peer addition/removal in a right way (hebasto)
- mytherra-core/gui#165 Save QSplitter state in QSettings (hebasto)
- mytherra-core/gui#173 Follow Qt docs when implementing rowCount and columnCount (hebasto)
- mytherra-core/gui#179 Add Type column to peers window, update peer details name/tooltip (jonatack)
- mytherra-core/gui#186 Add information to "Confirm fee bump" window (prayank23)
- mytherra-core/gui#189 Drop workaround for QTBUG-42503 which was fixed in Qt 5.5.0 (prusnak)
- mytherra-core/gui#194 Save/restore RPCConsole geometry only for window (hebasto)
- mytherra-core/gui#202 Fix right panel toggle in peers tab (RandyMcMillan)
- mytherra-core/gui#203 Display plain "Inbound" in peer details (jonatack)
- mytherra-core/gui#204 Drop buggy TableViewLastColumnResizingFixer class (hebasto)
- mytherra-core/gui#205, mytherra-core/gui#229 Save/restore TransactionView and recentRequestsView tables column sizes (hebasto)
- mytherra-core/gui#206 Display fRelayTxes and `bip152_highbandwidth_{to, from}` in peer details (jonatack)
- mytherra-core/gui#213 Add Copy Address Action to Payment Requests (jarolrod)
- mytherra-core/gui#214 Disable requests context menu actions when appropriate (jarolrod)
- mytherra-core/gui#217 Make warning label look clickable (jarolrod)
- mytherra-core/gui#219 Prevent the main window popup menu (hebasto)
- mytherra-core/gui#220 Do not translate file extensions (hebasto)
- mytherra-core/gui#221 RPCConsole translatable string fixes and improvements (jonatack)
- mytherra-core/gui#226 Add "Last Block" and "Last Tx" rows to peer details area (jonatack)
- mytherra-core/gui#233 qt test: Don't bind to regtest port (achow101)
- mytherra-core/gui#243 Fix issue when disabling the auto-enabled blank wallet checkbox (jarolrod)
- mytherra-core/gui#246 Revert "qt: Use "fusion" style on macOS Big Sur with old Qt" (hebasto)
- mytherra-core/gui#248 For values of "Bytes transferred" and "Bytes/s" with 1000-based prefix names use 1000-based divisor instead of 1024-based (wodry)
- mytherra-core/gui#251 Improve URI/file handling message (hebasto)
- mytherra-core/gui#256 Save/restore column sizes of the tables in the Peers tab (hebasto)
- mytherra-core/gui#260 Handle exceptions isntead of crash (hebasto)
- mytherra-core/gui#263 Revamp context menus (hebasto)
- mytherra-core/gui#271 Don't clear console prompt when font resizing (jarolrod)
- mytherra-core/gui#275 Support runtime appearance adjustment on macOS (hebasto)
- mytherra-core/gui#276 Elide long strings in their middle in the Peers tab (hebasto)
- mytherra-core/gui#281 Set shortcuts for console's resize buttons (jarolrod)
- mytherra-core/gui#293 Enable wordWrap for Services (RandyMcMillan)
- mytherra-core/gui#296 Do not use QObject::tr plural syntax for numbers with a unit symbol (hebasto)
- mytherra-core/gui#297 Avoid unnecessary translations (hebasto)
- mytherra-core/gui#298 Peertableview alternating row colors (RandyMcMillan)
- mytherra-core/gui#300 Remove progress bar on modal overlay (brunoerg)
- mytherra-core/gui#309 Add access to the Peers tab from the network icon (hebasto)
- mytherra-core/gui#311 Peers Window rename 'Peer id' to 'Peer' (jarolrod)
- mytherra-core/gui#313 Optimize string concatenation by default (hebasto)
- mytherra-core/gui#325 Align numbers in the "Peer Id" column to the right (hebasto)
- mytherra-core/gui#329 Make console buttons look clickable (jarolrod)
- mytherra-core/gui#330 Allow prompt icon to be colorized (jarolrod)
- mytherra-core/gui#331 Make RPC console welcome message translation-friendly (hebasto)
- mytherra-core/gui#332 Replace disambiguation strings with translator comments (hebasto)
- mytherra-core/gui#335 test: Use QSignalSpy instead of QEventLoop (jarolrod)
- mytherra-core/gui#343 Improve the GUI responsiveness when progress dialogs are used (hebasto)
- mytherra-core/gui#361 Fix GUI segfault caused by mytherra/mytherra#22216 (ryanofsky)
- mytherra-core/gui#362 Add keyboard shortcuts to context menus (luke-jr)
- mytherra-core/gui#366 Dark Mode fixes/portability (luke-jr)
- mytherra-core/gui#375 Emit dataChanged signal to dynamically re-sort Peers table (hebasto)
- mytherra-core/gui#393 Fix regression in "Encrypt Wallet" menu item (hebasto)
- mytherra-core/gui#396 Ensure external signer option remains disabled without signers (achow101)
- mytherra-core/gui#406 Handle new added plurals in `mytherra_en.ts` (hebasto)

### Build system
- mytherra/mytherra#17227 Add Android packaging support (icota)
- mytherra/mytherra#17920 guix: Build support for macOS (dongcarl)
- mytherra/mytherra#18298 Fix Qt processing of configure script for depends with DEBUG=1 (hebasto)
- mytherra/mytherra#19160 multiprocess: Add basic spawn and IPC support (ryanofsky)
- mytherra/mytherra#19504 Bump minimum python version to 3.6 (ajtowns)
- mytherra/mytherra#19522 fix building libconsensus with reduced exports for Darwin targets (fanquake)
- mytherra/mytherra#19683 Pin clang search paths for darwin host (dongcarl)
- mytherra/mytherra#19764 Split boost into build/host packages + bump + cleanup (dongcarl)
- mytherra/mytherra#19817 libtapi 1100.0.11 (fanquake)
- mytherra/mytherra#19846 enable unused member function diagnostic (Zero-1729)
- mytherra/mytherra#19867 Document and cleanup Qt hacks (fanquake)
- mytherra/mytherra#20046 Set `CMAKE_INSTALL_RPATH` for native packages (ryanofsky)
- mytherra/mytherra#20223 Drop the leading 0 from the version number (achow101)
- mytherra/mytherra#20333 Remove `native_biplist` dependency (fanquake)
- mytherra/mytherra#20353 configure: Support -fdebug-prefix-map and -fmacro-prefix-map (ajtowns)
- mytherra/mytherra#20359 Various config.site.in improvements and linting (dongcarl)
- mytherra/mytherra#20413 Require C++17 compiler (MarcoFalke)
- mytherra/mytherra#20419 Set minimum supported macOS to 10.14 (fanquake)
- mytherra/mytherra#20421 miniupnpc 2.2.2 (fanquake)
- mytherra/mytherra#20422 Mac deployment unification (fanquake)
- mytherra/mytherra#20424 Update univalue subtree (MarcoFalke)
- mytherra/mytherra#20449 Fix Windows installer build (achow101)
- mytherra/mytherra#20468 Warn when generating man pages for binaries built from a dirty branch (tylerchambers)
- mytherra/mytherra#20469 Avoid secp256k1.h include from system (dergoegge)
- mytherra/mytherra#20470 Replace genisoimage with xorriso (dongcarl)
- mytherra/mytherra#20471 Use C++17 in depends (fanquake)
- mytherra/mytherra#20496 Drop unneeded macOS framework dependencies (hebasto)
- mytherra/mytherra#20520 Do not force Precompiled Headers (PCH) for building Qt on Linux (hebasto)
- mytherra/mytherra#20549 Support make src/mytherra-node and src/mytherra-gui (promag)
- mytherra/mytherra#20565 Ensure PIC build for bdb on Android (BlockMechanic)
- mytherra/mytherra#20594 Fix getauxval calls in randomenv.cpp (jonasschnelli)
- mytherra/mytherra#20603 Update crc32c subtree (MarcoFalke)
- mytherra/mytherra#20609 configure: output notice that test binary is disabled by fuzzing (apoelstra)
- mytherra/mytherra#20619 guix: Quality of life improvements (dongcarl)
- mytherra/mytherra#20629 Improve id string robustness (dongcarl)
- mytherra/mytherra#20641 Use Qt top-level build facilities (hebasto)
- mytherra/mytherra#20650 Drop workaround for a fixed bug in Qt build system (hebasto)
- mytherra/mytherra#20673 Use more legible qmake commands in qt package (hebasto)
- mytherra/mytherra#20684 Define .INTERMEDIATE target once only (hebasto)
- mytherra/mytherra#20720 more robustly check for fcf-protection support (fanquake)
- mytherra/mytherra#20734 Make platform-specific targets available for proper platform builds only (hebasto)
- mytherra/mytherra#20936 build fuzz tests by default (danben)
- mytherra/mytherra#20937 guix: Make nsis reproducible by respecting SOURCE-DATE-EPOCH (dongcarl)
- mytherra/mytherra#20938 fix linking against -latomic when building for riscv (fanquake)
- mytherra/mytherra#20939 fix `RELOC_SECTION` security check for mytherra-util (fanquake)
- mytherra/mytherra#20963 gitian-linux: Build binaries for 64-bit POWER (continued) (laanwj)
- mytherra/mytherra#21036 gitian: Bump descriptors to focal for 22.0 (fanquake)
- mytherra/mytherra#21045 Adds switch to enable/disable randomized base address in MSVC builds (EthanHeilman)
- mytherra/mytherra#21065 make macOS HOST in download-osx generic (fanquake)
- mytherra/mytherra#21078 guix: only download sources for hosts being built (fanquake)
- mytherra/mytherra#21116 Disable --disable-fuzz-binary for gitian/guix builds (hebasto)
- mytherra/mytherra#21182 remove mostly pointless `BOOST_PROCESS` macro (fanquake)
- mytherra/mytherra#21205 actually fail when Boost is missing (fanquake)
- mytherra/mytherra#21209 use newer source for libnatpmp (fanquake)
- mytherra/mytherra#21226 Fix fuzz binary compilation under windows (danben)
- mytherra/mytherra#21231 Add /opt/homebrew to path to look for boost libraries (fyquah)
- mytherra/mytherra#21239 guix: Add codesignature attachment support for osx+win (dongcarl)
- mytherra/mytherra#21250 Make `HAVE_O_CLOEXEC` available outside LevelDB (bugfix) (theStack)
- mytherra/mytherra#21272 guix: Passthrough `SDK_PATH` into container (dongcarl)
- mytherra/mytherra#21274 assumptions:  Assume C++17 (fanquake)
- mytherra/mytherra#21286 Bump minimum Qt version to 5.9.5 (hebasto)
- mytherra/mytherra#21298 guix: Bump time-machine, glibc, and linux-headers (dongcarl)
- mytherra/mytherra#21304 guix: Add guix-clean script + establish gc-root for container profiles (dongcarl)
- mytherra/mytherra#21320 fix libnatpmp macos cross compile (fanquake)
- mytherra/mytherra#21321 guix: Add curl to required tool list (hebasto)
- mytherra/mytherra#21333 set Unicode true for NSIS installer (fanquake)
- mytherra/mytherra#21339 Make `AM_CONDITIONAL([ENABLE_EXTERNAL_SIGNER])` unconditional (hebasto)
- mytherra/mytherra#21349 Fix fuzz-cuckoocache cross-compiling with DEBUG=1 (hebasto)
- mytherra/mytherra#21354 build, doc: Drop no longer required packages from macOS cross-compiling dependencies (hebasto)
- mytherra/mytherra#21363 build, qt: Improve Qt static plugins/libs check code (hebasto)
- mytherra/mytherra#21375 guix: Misc feedback-based fixes + hier restructuring (dongcarl)
- mytherra/mytherra#21376 Qt 5.12.10 (fanquake)
- mytherra/mytherra#21382 Clean remnants of QTBUG-34748 fix (hebasto)
- mytherra/mytherra#21400 Fix regression introduced in #21363 (hebasto)
- mytherra/mytherra#21403 set --build when configuring packages in depends (fanquake)
- mytherra/mytherra#21421 don't try and use -fstack-clash-protection on Windows (fanquake)
- mytherra/mytherra#21423 Cleanups and follow ups after bumping Qt to 5.12.10 (hebasto)
- mytherra/mytherra#21427 Fix `id_string` invocations (dongcarl)
- mytherra/mytherra#21430 Add -Werror=implicit-fallthrough compile flag (hebasto)
- mytherra/mytherra#21457 Split libtapi and clang out of `native_cctools` (fanquake)
- mytherra/mytherra#21462 guix: Add guix-{attest,verify} scripts (dongcarl)
- mytherra/mytherra#21495 build, qt: Fix static builds on macOS Big Sur (hebasto)
- mytherra/mytherra#21497 Do not opt-in unused CoreWLAN stuff in depends for macOS (hebasto)
- mytherra/mytherra#21543 Enable safe warnings for msvc builds (hebasto)
- mytherra/mytherra#21565 Make `mytherra_qt.m4` more generic (fanquake)
- mytherra/mytherra#21610 remove -Wdeprecated-register from NOWARN flags (fanquake)
- mytherra/mytherra#21613 enable -Wdocumentation (fanquake)
- mytherra/mytherra#21629 Fix configuring when building depends with `NO_BDB=1` (fanquake)
- mytherra/mytherra#21654 build, qt: Make Qt rcc output always deterministic (hebasto)
- mytherra/mytherra#21655 build, qt: No longer need to set `QT_RCC_TEST=1` for determinism (hebasto)
- mytherra/mytherra#21658 fix make deploy for arm64-darwin (sgulls)
- mytherra/mytherra#21694 Use XLIFF file to provide more context to Transifex translators (hebasto)
- mytherra/mytherra#21708, mytherra/mytherra#21593 Drop pointless sed commands (hebasto)
- mytherra/mytherra#21731 Update msvc build to use Qt5.12.10 binaries (sipsorcery)
- mytherra/mytherra#21733 Re-add command to install vcpkg (dplusplus1024)
- mytherra/mytherra#21793 Use `-isysroot` over `--sysroot` on macOS (fanquake)
- mytherra/mytherra#21869 Add missing `-D_LIBCPP_DEBUG=1` to debug flags (MarcoFalke)
- mytherra/mytherra#21889 macho: check for control flow instrumentation (fanquake)
- mytherra/mytherra#21920 Improve macro for testing -latomic requirement (MarcoFalke)
- mytherra/mytherra#21991 libevent 2.1.12-stable (fanquake)
- mytherra/mytherra#22054 Bump Qt version to 5.12.11 (hebasto)
- mytherra/mytherra#22063 Use Qt archive of the same version as the compiled binaries (hebasto)
- mytherra/mytherra#22070 Don't use cf-protection when targeting arm-apple-darwin (fanquake)
- mytherra/mytherra#22071 Latest config.guess and config.sub (fanquake)
- mytherra/mytherra#22075 guix: Misc leftover usability improvements (dongcarl)
- mytherra/mytherra#22123 Fix qt.mk for mac arm64 (promag)
- mytherra/mytherra#22174 build, qt: Fix libraries linking order for Linux hosts (hebasto)
- mytherra/mytherra#22182 guix: Overhaul how guix-{attest,verify} works and hierarchy (dongcarl)
- mytherra/mytherra#22186 build, qt: Fix compiling qt package in depends with GCC 11 (hebasto)
- mytherra/mytherra#22199 macdeploy: minor fixups and simplifications (fanquake)
- mytherra/mytherra#22230 Fix MSVC linker /SubSystem option for mytherra-qt.exe (hebasto)
- mytherra/mytherra#22234 Mark print-% target as phony (dgoncharov)
- mytherra/mytherra#22238 improve detection of eBPF support (fanquake)
- mytherra/mytherra#22258 Disable deprecated-copy warning only when external warnings are enabled (MarcoFalke)
- mytherra/mytherra#22320 set minimum required Boost to 1.64.0 (fanquake)
- mytherra/mytherra#22348 Fix cross build for Windows with Boost Process (hebasto)
- mytherra/mytherra#22365 guix: Avoid relying on newer symbols by rebasing our cross toolchains on older glibcs (dongcarl)
- mytherra/mytherra#22381 guix: Test security-check sanity before performing them (with macOS) (fanquake)
- mytherra/mytherra#22405 Remove --enable-glibc-back-compat from Guix build (fanquake)
- mytherra/mytherra#22406 Remove --enable-determinism configure option (fanquake)
- mytherra/mytherra#22410 Avoid GCC 7.1 ABI change warning in guix build (sipa)
- mytherra/mytherra#22436 use aarch64 Clang if cross-compiling for darwin on aarch64 (fanquake)
- mytherra/mytherra#22465 guix: Pin kernel-header version, time-machine to upstream 1.3.0 commit (dongcarl)
- mytherra/mytherra#22511 guix: Silence `getent(1)` invocation, doc fixups (dongcarl)
- mytherra/mytherra#22531 guix: Fixes to guix-{attest,verify} (achow101)
- mytherra/mytherra#22642 release: Release with separate sha256sums and sig files (dongcarl)
- mytherra/mytherra#22685 clientversion: No suffix `#if CLIENT_VERSION_IS_RELEASE` (dongcarl)
- mytherra/mytherra#22713 Fix build with Boost 1.77.0 (sizeofvoid)

### Tests and QA
- mytherra/mytherra#14604 Add test and refactor `feature_block.py` (sanket1729)
- mytherra/mytherra#17556 Change `feature_config_args.py` not to rely on strange regtest=0 behavior (ryanofsky)
- mytherra/mytherra#18795 wallet issue with orphaned rewards (domob1812)
- mytherra/mytherra#18847 compressor: Use a prevector in CompressScript serialization (jb55)
- mytherra/mytherra#19259 fuzz: Add fuzzing harness for LoadMempool(…) and DumpMempool(…) (practicalswift)
- mytherra/mytherra#19315 Allow outbound & block-relay-only connections in functional tests. (amitiuttarwar)
- mytherra/mytherra#19698 Apply strict verification flags for transaction tests and assert backwards compatibility (glozow)
- mytherra/mytherra#19801 Check for all possible `OP_CLTV` fail reasons in `feature_cltv.py` (BIP 65) (theStack)
- mytherra/mytherra#19893 Remove or explain syncwithvalidationinterfacequeue (MarcoFalke)
- mytherra/mytherra#19972 fuzz: Add fuzzing harness for node eviction logic (practicalswift)
- mytherra/mytherra#19982 Fix inconsistent lock order in `wallet_tests/CreateWallet` (hebasto)
- mytherra/mytherra#20000 Fix creation of "std::string"s with \0s (vasild)
- mytherra/mytherra#20047 Use `wait_for_{block,header}` helpers in `p2p_fingerprint.py` (theStack)
- mytherra/mytherra#20171 Add functional test `test_txid_inv_delay` (ariard)
- mytherra/mytherra#20189 Switch to BIP341's suggested scheme for outputs without script (sipa)
- mytherra/mytherra#20248 Fix length of R check in `key_signature_tests` (dgpv)
- mytherra/mytherra#20276, mytherra/mytherra#20385, mytherra/mytherra#20688, mytherra/mytherra#20692 Run various mempool tests even with wallet disabled (mjdietzx)
- mytherra/mytherra#20323 Create or use existing properly initialized NodeContexts (dongcarl)
- mytherra/mytherra#20354 Add `feature_taproot.py --previous_release` (MarcoFalke)
- mytherra/mytherra#20370 fuzz: Version handshake (MarcoFalke)
- mytherra/mytherra#20377 fuzz: Fill various small fuzzing gaps (practicalswift)
- mytherra/mytherra#20425 fuzz: Make CAddrMan fuzzing harness deterministic (practicalswift)
- mytherra/mytherra#20430 Sanitizers: Add suppression for unsigned-integer-overflow in libstdc++ (jonasschnelli)
- mytherra/mytherra#20437 fuzz: Avoid time-based "non-determinism" in fuzzing harnesses by using mocked GetTime() (practicalswift)
- mytherra/mytherra#20458 Add `is_bdb_compiled` helper (Sjors)
- mytherra/mytherra#20466 Fix intermittent `p2p_fingerprint` issue (MarcoFalke)
- mytherra/mytherra#20472 Add testing of ParseInt/ParseUInt edge cases with leading +/-/0:s (practicalswift)
- mytherra/mytherra#20507 sync: print proper lock order location when double lock is detected (vasild)
- mytherra/mytherra#20522 Fix sync issue in `disconnect_p2ps` (amitiuttarwar)
- mytherra/mytherra#20524 Move `MIN_VERSION_SUPPORTED` to p2p.py (jnewbery)
- mytherra/mytherra#20540 Fix `wallet_multiwallet` issue on windows (MarcoFalke)
- mytherra/mytherra#20560 fuzz: Link all targets once (MarcoFalke)
- mytherra/mytherra#20567 Add option to git-subtree-check to do full check, add help (laanwj)
- mytherra/mytherra#20569 Fix intermittent `wallet_multiwallet` issue with `got_loading_error` (MarcoFalke)
- mytherra/mytherra#20613 Use Popen.wait instead of RPC in `assert_start_raises_init_error` (MarcoFalke)
- mytherra/mytherra#20663 fuzz: Hide `script_assets_test_minimizer` (MarcoFalke)
- mytherra/mytherra#20674 fuzz: Call SendMessages after ProcessMessage to increase coverage (MarcoFalke)
- mytherra/mytherra#20683 Fix restart node race (MarcoFalke)
- mytherra/mytherra#20686 fuzz: replace CNode code with fuzz/util.h::ConsumeNode() (jonatack)
- mytherra/mytherra#20733 Inline non-member functions with body in fuzzing headers (pstratem)
- mytherra/mytherra#20737 Add missing assignment in `mempool_resurrect.py` (MarcoFalke)
- mytherra/mytherra#20745 Correct `epoll_ctl` data race suppression (hebasto)
- mytherra/mytherra#20748 Add race:SendZmqMessage tsan suppression (MarcoFalke)
- mytherra/mytherra#20760 Set correct nValue for multi-op-return policy check (MarcoFalke)
- mytherra/mytherra#20761 fuzz: Check that `NULL_DATA` is unspendable (MarcoFalke)
- mytherra/mytherra#20765 fuzz: Check that certain script TxoutType are nonstandard (mjdietzx)
- mytherra/mytherra#20772 fuzz: Bolster ExtractDestination(s) checks (mjdietzx)
- mytherra/mytherra#20789 fuzz: Rework strong and weak net enum fuzzing (MarcoFalke)
- mytherra/mytherra#20828 fuzz: Introduce CallOneOf helper to replace switch-case (MarcoFalke)
- mytherra/mytherra#20839 fuzz: Avoid extraneous copy of input data, using Span<> (MarcoFalke)
- mytherra/mytherra#20844 Add sanitizer suppressions for AMD EPYC CPUs (MarcoFalke)
- mytherra/mytherra#20857 Update documentation in `feature_csv_activation.py` (PiRK)
- mytherra/mytherra#20876 Replace getmempoolentry with testmempoolaccept in MiniWallet (MarcoFalke)
- mytherra/mytherra#20881 fuzz: net permission flags in net processing (MarcoFalke)
- mytherra/mytherra#20882 fuzz: Add missing muhash registration (MarcoFalke)
- mytherra/mytherra#20908 fuzz: Use mocktime in `process_message*` fuzz targets (MarcoFalke)
- mytherra/mytherra#20915 fuzz: Fail if message type is not fuzzed (MarcoFalke)
- mytherra/mytherra#20946 fuzz: Consolidate fuzzing TestingSetup initialization (dongcarl)
- mytherra/mytherra#20954 Declare `nodes` type `in test_framework.py` (kiminuo)
- mytherra/mytherra#20955 Fix `get_previous_releases.py` for aarch64 (MarcoFalke)
- mytherra/mytherra#20969 check that getblockfilter RPC fails without block filter index (theStack)
- mytherra/mytherra#20971 Work around libFuzzer deadlock (MarcoFalke)
- mytherra/mytherra#20993 Store subversion (user agent) as string in `msg_version` (theStack)
- mytherra/mytherra#20995 fuzz: Avoid initializing version to less than `MIN_PEER_PROTO_VERSION` (MarcoFalke)
- mytherra/mytherra#20998 Fix BlockToJsonVerbose benchmark (martinus)
- mytherra/mytherra#21003 Move MakeNoLogFileContext to `libtest_util`, and use it in bench (MarcoFalke)
- mytherra/mytherra#21008 Fix zmq test flakiness, improve speed (theStack)
- mytherra/mytherra#21023 fuzz: Disable shuffle when merge=1 (MarcoFalke)
- mytherra/mytherra#21037 fuzz: Avoid designated initialization (C++20) in fuzz tests (practicalswift)
- mytherra/mytherra#21042 doc, test: Improve `setup_clean_chain` documentation (fjahr)
- mytherra/mytherra#21080 fuzz: Configure check for main function (take 2) (MarcoFalke)
- mytherra/mytherra#21084 Fix timeout decrease in `feature_assumevalid` (brunoerg)
- mytherra/mytherra#21096 Re-add dead code detection (flack)
- mytherra/mytherra#21100 Remove unused function `xor_bytes` (theStack)
- mytherra/mytherra#21115 Fix Windows cross build (hebasto)
- mytherra/mytherra#21117 Remove `assert_blockchain_height` (MarcoFalke)
- mytherra/mytherra#21121 Small unit test improvements, including helper to make mempool transaction (amitiuttarwar)
- mytherra/mytherra#21124 Remove unnecessary assignment in bdb (brunoerg)
- mytherra/mytherra#21125 Change `BOOST_CHECK` to `BOOST_CHECK_EQUAL` for paths (kiminuo)
- mytherra/mytherra#21142, mytherra/mytherra#21512 fuzz: Add `tx_pool` fuzz target (MarcoFalke)
- mytherra/mytherra#21165 Use mocktime in `test_seed_peers` (dhruv)
- mytherra/mytherra#21169 fuzz: Add RPC interface fuzzing. Increase fuzzing coverage from 65% to 70% (practicalswift)
- mytherra/mytherra#21170 bench: Add benchmark to write json into a string (martinus)
- mytherra/mytherra#21178 Run `mempool_reorg.py` even with wallet disabled (DariusParvin)
- mytherra/mytherra#21185 fuzz: Remove expensive and redundant muhash from crypto fuzz target (MarcoFalke)
- mytherra/mytherra#21200 Speed up `rpc_blockchain.py` by removing miniwallet.generate() (MarcoFalke)
- mytherra/mytherra#21211 Move `P2WSH_OP_TRUE` to shared test library (MarcoFalke)
- mytherra/mytherra#21228 Avoid comparision of integers with different signs (jonasschnelli)
- mytherra/mytherra#21230 Fix `NODE_NETWORK_LIMITED_MIN_BLOCKS` disconnection (MarcoFalke)
- mytherra/mytherra#21252 Add missing wait for sync to `feature_blockfilterindex_prune` (MarcoFalke)
- mytherra/mytherra#21254 Avoid connecting to real network when running tests (MarcoFalke)
- mytherra/mytherra#21264 fuzz: Two scripted diff renames (MarcoFalke)
- mytherra/mytherra#21280 Bug fix in `transaction_tests` (glozow)
- mytherra/mytherra#21293 Replace accidentally placed bit-OR with logical-OR (hebasto)
- mytherra/mytherra#21297 `feature_blockfilterindex_prune.py` improvements (jonatack)
- mytherra/mytherra#21310 zmq test: fix sync-up by matching notification to generated block (theStack)
- mytherra/mytherra#21334 Additional BIP9 tests (Sjors)
- mytherra/mytherra#21338 Add functional test for anchors.dat (brunoerg)
- mytherra/mytherra#21345 Bring `p2p_leak.py` up to date (mzumsande)
- mytherra/mytherra#21357 Unconditionally check for fRelay field in test framework (jarolrod)
- mytherra/mytherra#21358 fuzz: Add missing include (`test/util/setup_common.h`) (MarcoFalke)
- mytherra/mytherra#21371 fuzz: fix gcc Woverloaded-virtual build warnings (jonatack)
- mytherra/mytherra#21373 Generate fewer blocks in `feature_nulldummy` to fix timeouts, speed up (jonatack)
- mytherra/mytherra#21390 Test improvements for UTXO set hash tests (fjahr)
- mytherra/mytherra#21410 increase `rpc_timeout` for fundrawtx `test_transaction_too_large` (jonatack)
- mytherra/mytherra#21411 add logging, reduce blocks, move `sync_all` in `wallet_` groups (jonatack)
- mytherra/mytherra#21438 Add ParseUInt8() test coverage (jonatack)
- mytherra/mytherra#21443 fuzz: Implement `fuzzed_dns_lookup_function` as a lambda (practicalswift)
- mytherra/mytherra#21445 cirrus: Use SSD cluster for speedup (MarcoFalke)
- mytherra/mytherra#21477 Add test for CNetAddr::ToString IPv6 address formatting (RFC 5952) (practicalswift)
- mytherra/mytherra#21487 fuzz: Use ConsumeWeakEnum in addrman for service flags (MarcoFalke)
- mytherra/mytherra#21488 Add ParseUInt16() unit test and fuzz coverage (jonatack)
- mytherra/mytherra#21491 test: remove duplicate assertions in util_tests (jonatack)
- mytherra/mytherra#21522 fuzz: Use PickValue where possible (MarcoFalke)
- mytherra/mytherra#21531 remove qt byteswap compattests (fanquake)
- mytherra/mytherra#21557 small cleanup in RPCNestedTests tests (fanquake)
- mytherra/mytherra#21586 Add missing suppression for signed-integer-overflow:txmempool.cpp (MarcoFalke)
- mytherra/mytherra#21592 Remove option to make TestChain100Setup non-deterministic (MarcoFalke)
- mytherra/mytherra#21597 Document `race:validation_chainstatemanager_tests` suppression (MarcoFalke)
- mytherra/mytherra#21599 Replace file level integer overflow suppression with function level suppression (practicalswift)
- mytherra/mytherra#21604 Document why no symbol names can be used for suppressions (MarcoFalke)
- mytherra/mytherra#21606 fuzz: Extend psbt fuzz target a bit (MarcoFalke)
- mytherra/mytherra#21617 fuzz: Fix uninitialized read in i2p test (MarcoFalke)
- mytherra/mytherra#21630 fuzz: split FuzzedSock interface and implementation (vasild)
- mytherra/mytherra#21634 Skip SQLite fsyncs while testing (achow101)
- mytherra/mytherra#21669 Remove spurious double lock tsan suppressions by bumping to clang-12 (MarcoFalke)
- mytherra/mytherra#21676 Use mocktime to avoid intermittent failure in `rpc_tests` (MarcoFalke)
- mytherra/mytherra#21677 fuzz: Avoid use of low file descriptor ids (which may be in use) in FuzzedSock (practicalswift)
- mytherra/mytherra#21678 Fix TestPotentialDeadLockDetected suppression (hebasto)
- mytherra/mytherra#21689 Remove intermittently failing and not very meaningful `BOOST_CHECK` in `cnetaddr_basic` (practicalswift)
- mytherra/mytherra#21691 Check that no versionbits are re-used (MarcoFalke)
- mytherra/mytherra#21707 Extend functional tests for addr relay (mzumsande)
- mytherra/mytherra#21712 Test default `include_mempool` value of gettxout (promag)
- mytherra/mytherra#21738 Use clang-12 for ASAN, Add missing suppression (MarcoFalke)
- mytherra/mytherra#21740 add new python linter to check file names and permissions (windsok)
- mytherra/mytherra#21749 Bump shellcheck version (hebasto)
- mytherra/mytherra#21754 Run `feature_cltv` with MiniWallet (MarcoFalke)
- mytherra/mytherra#21762 Speed up `mempool_spend_coinbase.py` (MarcoFalke)
- mytherra/mytherra#21773 fuzz: Ensure prevout is consensus-valid (MarcoFalke)
- mytherra/mytherra#21777 Fix `feature_notifications.py` intermittent issue (MarcoFalke)
- mytherra/mytherra#21785 Fix intermittent issue in `p2p_addr_relay.py` (MarcoFalke)
- mytherra/mytherra#21787 Fix off-by-ones in `rpc_fundrawtransaction` assertions (jonatack)
- mytherra/mytherra#21792 Fix intermittent issue in `p2p_segwit.py` (MarcoFalke)
- mytherra/mytherra#21795 fuzz: Terminate immediately if a fuzzing harness tries to perform a DNS lookup (belt and suspenders) (practicalswift)
- mytherra/mytherra#21798 fuzz: Create a block template in `tx_pool` targets (MarcoFalke)
- mytherra/mytherra#21804 Speed up `p2p_segwit.py` (jnewbery)
- mytherra/mytherra#21810 fuzz: Various RPC fuzzer follow-ups (practicalswift)
- mytherra/mytherra#21814 Fix `feature_config_args.py` intermittent issue (MarcoFalke)
- mytherra/mytherra#21821 Add missing test for empty P2WSH redeem (MarcoFalke)
- mytherra/mytherra#21822 Resolve bug in `interface_mytherra_cli.py` (klementtan)
- mytherra/mytherra#21846 fuzz: Add `-fsanitize=integer` suppression needed for RPC fuzzer (`generateblock`) (practicalswift)
- mytherra/mytherra#21849 fuzz: Limit toxic test globals to their respective scope (MarcoFalke)
- mytherra/mytherra#21867 use MiniWallet for `p2p_blocksonly.py` (theStack)
- mytherra/mytherra#21873 minor fixes & improvements for files linter test (windsok)
- mytherra/mytherra#21874 fuzz: Add `WRITE_ALL_FUZZ_TARGETS_AND_ABORT` (MarcoFalke)
- mytherra/mytherra#21884 fuzz: Remove unused --enable-danger-fuzz-link-all option (MarcoFalke)
- mytherra/mytherra#21890 fuzz: Limit ParseISO8601DateTime fuzzing to 32-bit (MarcoFalke)
- mytherra/mytherra#21891 fuzz: Remove strprintf test cases that are known to fail (MarcoFalke)
- mytherra/mytherra#21892 fuzz: Avoid excessively large min fee rate in `tx_pool` (MarcoFalke)
- mytherra/mytherra#21895 Add TSA annotations to the WorkQueue class members (hebasto)
- mytherra/mytherra#21900 use MiniWallet for `feature_csv_activation.py` (theStack)
- mytherra/mytherra#21909 fuzz: Limit max insertions in timedata fuzz test (MarcoFalke)
- mytherra/mytherra#21922 fuzz: Avoid timeout in EncodeBase58 (MarcoFalke)
- mytherra/mytherra#21927 fuzz: Run const CScript member functions only once (MarcoFalke)
- mytherra/mytherra#21929 fuzz: Remove incorrect float round-trip serialization test (MarcoFalke)
- mytherra/mytherra#21936 fuzz: Terminate immediately if a fuzzing harness tries to create a TCP socket (belt and suspenders) (practicalswift)
- mytherra/mytherra#21941 fuzz: Call const member functions in addrman fuzz test only once (MarcoFalke)
- mytherra/mytherra#21945 add P2PK support to MiniWallet (theStack)
- mytherra/mytherra#21948 Fix off-by-one in mockscheduler test RPC (MarcoFalke)
- mytherra/mytherra#21953 fuzz: Add `utxo_snapshot` target (MarcoFalke)
- mytherra/mytherra#21970 fuzz: Add missing CheckTransaction before CheckTxInputs (MarcoFalke)
- mytherra/mytherra#21989 Use `COINBASE_MATURITY` in functional tests (kiminuo)
- mytherra/mytherra#22003 Add thread safety annotations (ajtowns)
- mytherra/mytherra#22004 fuzz: Speed up transaction fuzz target (MarcoFalke)
- mytherra/mytherra#22005 fuzz: Speed up banman fuzz target (MarcoFalke)
- mytherra/mytherra#22029 [fuzz] Improve transport deserialization fuzz test coverage (dhruv)
- mytherra/mytherra#22048 MiniWallet: introduce enum type for output mode (theStack)
- mytherra/mytherra#22057 use MiniWallet (P2PK mode) for `feature_dersig.py` (theStack)
- mytherra/mytherra#22065 Mark `CheckTxInputs` `[[nodiscard]]`. Avoid UUM in fuzzing harness `coins_view` (practicalswift)
- mytherra/mytherra#22069 fuzz: don't try and use fopencookie() when building for Android (fanquake)
- mytherra/mytherra#22082 update nanobench from release 4.0.0 to 4.3.4 (martinus)
- mytherra/mytherra#22086 remove BasicTestingSetup from unit tests that don't need it (fanquake)
- mytherra/mytherra#22089 MiniWallet: fix fee calculation for P2PK and check tx vsize (theStack)
- mytherra/mytherra#21107, mytherra/mytherra#22092 Convert documentation into type annotations (fanquake)
- mytherra/mytherra#22095 Additional BIP32 test vector for hardened derivation with leading zeros (kristapsk)
- mytherra/mytherra#22103 Fix IPv6 check on BSD systems (n-thumann)
- mytherra/mytherra#22118 check anchors.dat when node starts for the first time (brunoerg)
- mytherra/mytherra#22120 `p2p_invalid_block`: Check that a block rejected due to too-new tim… (willcl-ark)
- mytherra/mytherra#22153 Fix `p2p_leak.py` intermittent failure (mzumsande)
- mytherra/mytherra#22169 p2p, rpc, fuzz: various tiny follow-ups (jonatack)
- mytherra/mytherra#22176 Correct outstanding -Werror=sign-compare errors (Empact)
- mytherra/mytherra#22180 fuzz: Increase branch coverage of the float fuzz target (MarcoFalke)
- mytherra/mytherra#22187 Add `sync_blocks` in `wallet_orphanedreward.py` (domob1812)
- mytherra/mytherra#22201 Fix TestShell to allow running in Jupyter Notebook (josibake)
- mytherra/mytherra#22202 Add temporary coinstats suppressions (MarcoFalke)
- mytherra/mytherra#22203 Use ConnmanTestMsg from test lib in `denialofservice_tests` (MarcoFalke)
- mytherra/mytherra#22210 Use MiniWallet in `test_no_inherited_signaling` RBF test (MarcoFalke)
- mytherra/mytherra#22224 Update msvc and appveyor builds to use Qt5.12.11 binaries (sipsorcery)
- mytherra/mytherra#22249 Kill process group to avoid dangling processes when using `--failfast` (S3RK)
- mytherra/mytherra#22267 fuzz: Speed up crypto fuzz target (MarcoFalke)
- mytherra/mytherra#22270 Add mytherra-util tests (+refactors) (MarcoFalke)
- mytherra/mytherra#22271 fuzz: Assert roundtrip equality for `CPubKey` (theStack)
- mytherra/mytherra#22279 fuzz: add missing ECCVerifyHandle to `base_encode_decode` (apoelstra)
- mytherra/mytherra#22292 bench, doc: benchmarking updates and fixups (jonatack)
- mytherra/mytherra#22306 Improvements to `p2p_addr_relay.py` (amitiuttarwar)
- mytherra/mytherra#22310 Add functional test for replacement relay fee check (ariard)
- mytherra/mytherra#22311 Add missing syncwithvalidationinterfacequeue in `p2p_blockfilters` (MarcoFalke)
- mytherra/mytherra#22313 Add missing `sync_all` to `feature_coinstatsindex` (MarcoFalke)
- mytherra/mytherra#22322 fuzz: Check banman roundtrip (MarcoFalke)
- mytherra/mytherra#22363 Use `script_util` helpers for creating P2{PKH,SH,WPKH,WSH} scripts (theStack)
- mytherra/mytherra#22399 fuzz: Rework CTxDestination fuzzing (MarcoFalke)
- mytherra/mytherra#22408 add tests for `bad-txns-prevout-null` reject reason (theStack)
- mytherra/mytherra#22445 fuzz: Move implementations of non-template fuzz helpers from util.h to util.cpp (sriramdvt)
- mytherra/mytherra#22446 Fix `wallet_listdescriptors.py` if bdb is not compiled (hebasto)
- mytherra/mytherra#22447 Whitelist `rpc_rawtransaction` peers to speed up tests (jonatack)
- mytherra/mytherra#22742 Use proper target in `do_fund_send` (S3RK)

### Miscellaneous
- mytherra/mytherra#19337 sync: Detect double lock from the same thread (vasild)
- mytherra/mytherra#19809 log: Prefix log messages with function name and source code location if -logsourcelocations is set (practicalswift)
- mytherra/mytherra#19866 eBPF Linux tracepoints (jb55)
- mytherra/mytherra#20024 init: Fix incorrect warning "Reducing -maxconnections from N to N-1, because of system limitations" (practicalswift)
- mytherra/mytherra#20145 contrib: Add getcoins.py script to get coins from (signet) faucet (kallewoof)
- mytherra/mytherra#20255 util: Add assume() identity function (MarcoFalke)
- mytherra/mytherra#20288 script, doc: Contrib/seeds updates (jonatack)
- mytherra/mytherra#20358 src/randomenv.cpp: Fix build on uclibc (ffontaine)
- mytherra/mytherra#20406 util: Avoid invalid integer negation in formatmoney and valuefromamount (practicalswift)
- mytherra/mytherra#20434 contrib: Parse elf directly for symbol and security checks (laanwj)
- mytherra/mytherra#20451 lint: Run mypy over contrib/devtools (fanquake)
- mytherra/mytherra#20476 contrib: Add test for elf symbol-check (laanwj)
- mytherra/mytherra#20530 lint: Update cppcheck linter to c++17 and improve explicit usage (fjahr)
- mytherra/mytherra#20589 log: Clarify that failure to read/write `fee_estimates.dat` is non-fatal (MarcoFalke)
- mytherra/mytherra#20602 util: Allow use of c++14 chrono literals (MarcoFalke)
- mytherra/mytherra#20605 init: Signal-safe instant shutdown (laanwj)
- mytherra/mytherra#20608 contrib: Add symbol check test for PE binaries (fanquake)
- mytherra/mytherra#20689 contrib: Replace binary verification script verify.sh with python rewrite (theStack)
- mytherra/mytherra#20715 util: Add argsmanager::getcommand() and use it in mytherra-wallet (MarcoFalke)
- mytherra/mytherra#20735 script: Remove outdated extract-osx-sdk.sh (hebasto)
- mytherra/mytherra#20817 lint: Update list of spelling linter false positives, bump to codespell 2.0.0 (theStack)
- mytherra/mytherra#20884 script: Improve robustness of mytherrad.service on startup (hebasto)
- mytherra/mytherra#20906 contrib: Embed c++11 patch in `install_db4.sh` (gruve-p)
- mytherra/mytherra#21004 contrib: Fix docker args conditional in gitian-build (setpill)
- mytherra/mytherra#21007 mytherrad: Add -daemonwait option to wait for initialization (laanwj)
- mytherra/mytherra#21041 log: Move "Pre-allocating up to position 0x[…] in […].dat" log message to debug category (practicalswift)
- mytherra/mytherra#21059 Drop boost/preprocessor dependencies (hebasto)
- mytherra/mytherra#21087 guix: Passthrough `BASE_CACHE` into container (dongcarl)
- mytherra/mytherra#21088 guix: Jump forwards in time-machine and adapt (dongcarl)
- mytherra/mytherra#21089 guix: Add support for powerpc64{,le} (dongcarl)
- mytherra/mytherra#21110 util: Remove boost `posix_time` usage from `gettime*` (fanquake)
- mytherra/mytherra#21111 Improve OpenRC initscript (parazyd)
- mytherra/mytherra#21123 code style: Add EditorConfig file (kiminuo)
- mytherra/mytherra#21173 util: Faster hexstr => 13% faster blocktojson (martinus)
- mytherra/mytherra#21221 tools: Allow argument/parameter bin packing in clang-format (jnewbery)
- mytherra/mytherra#21244 Move GetDataDir to ArgsManager (kiminuo)
- mytherra/mytherra#21255 contrib: Run test-symbol-check for risc-v (fanquake)
- mytherra/mytherra#21271 guix: Explicitly set umask in build container (dongcarl)
- mytherra/mytherra#21300 script: Add explanatory comment to tc.sh (dscotese)
- mytherra/mytherra#21317 util: Make assume() usable as unary expression (MarcoFalke)
- mytherra/mytherra#21336 Make .gitignore ignore src/test/fuzz/fuzz.exe (hebasto)
- mytherra/mytherra#21337 guix: Update darwin native packages dependencies (hebasto)
- mytherra/mytherra#21405 compat: remove memcpy -> memmove backwards compatibility alias (fanquake)
- mytherra/mytherra#21418 contrib: Make systemd invoke dependencies only when ready (laanwj)
- mytherra/mytherra#21447 Always add -daemonwait to known command line arguments (hebasto)
- mytherra/mytherra#21471 bugfix: Fix `bech32_encode` calls in `gen_key_io_test_vectors.py` (sipa)
- mytherra/mytherra#21615 script: Add trusted key for hebasto (hebasto)
- mytherra/mytherra#21664 contrib: Use lief for macos and windows symbol & security checks (fanquake)
- mytherra/mytherra#21695 contrib: Remove no longer used contrib/mytherra-qt.pro (hebasto)
- mytherra/mytherra#21711 guix: Add full installation and usage documentation (dongcarl)
- mytherra/mytherra#21799 guix: Use `gcc-8` across the board (dongcarl)
- mytherra/mytherra#21802 Avoid UB in util/asmap (advance a dereferenceable iterator outside its valid range) (MarcoFalke)
- mytherra/mytherra#21823 script: Update reviewers (jonatack)
- mytherra/mytherra#21850 Remove `GetDataDir(net_specific)` function (kiminuo)
- mytherra/mytherra#21871 scripts: Add checks for minimum required os versions (fanquake)
- mytherra/mytherra#21966 Remove double serialization; use software encoder for fee estimation (sipa)
- mytherra/mytherra#22060 contrib: Add torv3 seed nodes for testnet, drop v2 ones (laanwj)
- mytherra/mytherra#22244 devtools: Correctly extract symbol versions in symbol-check (laanwj)
- mytherra/mytherra#22533 guix/build: Remove vestigial SKIPATTEST.TAG (dongcarl)
- mytherra/mytherra#22643 guix-verify: Non-zero exit code when anything fails (dongcarl)
- mytherra/mytherra#22654 guix: Don't include directory name in SHA256SUMS (achow101)

### Documentation
- mytherra/mytherra#15451 clarify getdata limit after #14897 (HashUnlimited)
- mytherra/mytherra#15545 Explain why CheckBlock() is called before AcceptBlock (Sjors)
- mytherra/mytherra#17350 Add developer documentation to isminetype (HAOYUatHZ)
- mytherra/mytherra#17934 Use `CONFIG_SITE` variable instead of --prefix option (hebasto)
- mytherra/mytherra#18030 Coin::IsSpent() can also mean never existed (Sjors)
- mytherra/mytherra#18096 IsFinalTx comment about nSequence & `OP_CLTV` (nothingmuch)
- mytherra/mytherra#18568 Clarify developer notes about constant naming (ryanofsky)
- mytherra/mytherra#19961 doc: tor.md updates (jonatack)
- mytherra/mytherra#19968 Clarify CRollingBloomFilter size estimate (robot-dreams)
- mytherra/mytherra#20200 Rename CODEOWNERS to REVIEWERS (adamjonas)
- mytherra/mytherra#20329 docs/descriptors.md: Remove hardened marker in the path after xpub (dgpv)
- mytherra/mytherra#20380 Add instructions on how to fuzz the P2P layer using Honggfuzz NetDriver (practicalswift)
- mytherra/mytherra#20414 Remove generated manual pages from master branch (laanwj)
- mytherra/mytherra#20473 Document current boost dependency as 1.71.0 (laanwj)
- mytherra/mytherra#20512 Add bash as an OpenBSD dependency (emilengler)
- mytherra/mytherra#20568 Use FeeModes doc helper in estimatesmartfee (MarcoFalke)
- mytherra/mytherra#20577 libconsensus: add missing error code description, fix NMytherra link (theStack)
- mytherra/mytherra#20587 Tidy up Tor doc (more stringent) (wodry)
- mytherra/mytherra#20592 Update wtxidrelay documentation per BIP339 (jonatack)
- mytherra/mytherra#20601 Update for FreeBSD 12.2, add GUI Build Instructions (jarolrod)
- mytherra/mytherra#20635 fix misleading comment about call to non-existing function (pox)
- mytherra/mytherra#20646 Refer to BIPs 339/155 in feature negotiation (jonatack)
- mytherra/mytherra#20653 Move addr relay comment in net to correct place (MarcoFalke)
- mytherra/mytherra#20677 Remove shouty enums in `net_processing` comments (sdaftuar)
- mytherra/mytherra#20741 Update 'Secure string handling' (prayank23)
- mytherra/mytherra#20757 tor.md and -onlynet help updates (jonatack)
- mytherra/mytherra#20829 Add -netinfo help (jonatack)
- mytherra/mytherra#20830 Update developer notes with signet (jonatack)
- mytherra/mytherra#20890 Add explicit macdeployqtplus dependencies install step (hebasto)
- mytherra/mytherra#20913 Add manual page generation for mytherra-util (laanwj)
- mytherra/mytherra#20985 Add xorriso to macOS depends packages (fanquake)
- mytherra/mytherra#20986 Update developer notes to discourage very long lines (jnewbery)
- mytherra/mytherra#20987 Add instructions for generating RPC docs (ben-kaufman)
- mytherra/mytherra#21026 Document use of make-tag script to make tags (laanwj)
- mytherra/mytherra#21028 doc/bips: Add BIPs 43, 44, 49, and 84 (luke-jr)
- mytherra/mytherra#21049 Add release notes for listdescriptors RPC (S3RK)
- mytherra/mytherra#21060 More precise -debug and -debugexclude doc (wodry)
- mytherra/mytherra#21077 Clarify -timeout and -peertimeout config options (glozow)
- mytherra/mytherra#21105 Correctly identify script type (niftynei)
- mytherra/mytherra#21163 Guix is shipped in Debian and Ubuntu (MarcoFalke)
- mytherra/mytherra#21210 Rework internal and external links (MarcoFalke)
- mytherra/mytherra#21246 Correction for VerifyTaprootCommitment comments (roconnor-blockstream)
- mytherra/mytherra#21263 Clarify that squashing should happen before review (MarcoFalke)
- mytherra/mytherra#21323 guix, doc: Update default HOSTS value (hebasto)
- mytherra/mytherra#21324 Update build instructions for Fedora (hebasto)
- mytherra/mytherra#21343 Revamp macOS build doc (jarolrod)
- mytherra/mytherra#21346 install qt5 when building on macOS (fanquake)
- mytherra/mytherra#21384 doc: add signet to mytherra.conf documentation (jonatack)
- mytherra/mytherra#21394 Improve comment about protected peers (amitiuttarwar)
- mytherra/mytherra#21398 Update fuzzing docs for afl-clang-lto (MarcoFalke)
- mytherra/mytherra#21444 net, doc: Doxygen updates and fixes in netbase.{h,cpp} (jonatack)
- mytherra/mytherra#21481 Tell howto install clang-format on Debian/Ubuntu (wodry)
- mytherra/mytherra#21567 Fix various misleading comments (glozow)
- mytherra/mytherra#21661 Fix name of script guix-build (Emzy)
- mytherra/mytherra#21672 Remove boostrap info from `GUIX_COMMON_FLAGS` doc (fanquake)
- mytherra/mytherra#21688 Note on SDK for macOS depends cross-compile (jarolrod)
- mytherra/mytherra#21709 Update reduce-memory.md and mytherra.conf -maxconnections info (jonatack)
- mytherra/mytherra#21710 update helps for addnode rpc and -addnode/-maxconnections config options (jonatack)
- mytherra/mytherra#21752 Clarify that feerates are per virtual size (MarcoFalke)
- mytherra/mytherra#21811 Remove Visual Studio 2017 reference from readme (sipsorcery)
- mytherra/mytherra#21818 Fixup -coinstatsindex help, update mytherra.conf and files.md (jonatack)
- mytherra/mytherra#21856 add OSS-Fuzz section to fuzzing.md doc (adamjonas)
- mytherra/mytherra#21912 Remove mention of priority estimation (MarcoFalke)
- mytherra/mytherra#21925 Update bips.md for 0.21.1 (MarcoFalke)
- mytherra/mytherra#21942 improve make with parallel jobs description (klementtan)
- mytherra/mytherra#21947 Fix OSS-Fuzz links (MarcoFalke)
- mytherra/mytherra#21988 note that brew installed qt is not supported (jarolrod)
- mytherra/mytherra#22056 describe in fuzzing.md how to reproduce a CI crash (jonatack)
- mytherra/mytherra#22080 add maxuploadtarget to mytherra.conf example (jarolrod)
- mytherra/mytherra#22088 Improve note on choosing posix mingw32 (jarolrod)
- mytherra/mytherra#22109 Fix external links (IRC, …) (MarcoFalke)
- mytherra/mytherra#22121 Various validation doc fixups (MarcoFalke)
- mytherra/mytherra#22172 Update tor.md, release notes with removal of tor v2 support (jonatack)
- mytherra/mytherra#22204 Remove obsolete `okSafeMode` RPC guideline from developer notes (theStack)
- mytherra/mytherra#22208 Update `REVIEWERS` (practicalswift)
- mytherra/mytherra#22250 add basic I2P documentation (vasild)
- mytherra/mytherra#22296 Final merge of release notes snippets, mv to wiki (MarcoFalke)
- mytherra/mytherra#22335 recommend `--disable-external-signer` in OpenBSD build guide (theStack)
- mytherra/mytherra#22339 Document minimum required libc++ version (hebasto)
- mytherra/mytherra#22349 Repository IRC updates (jonatack)
- mytherra/mytherra#22360 Remove unused section from release process (MarcoFalke)
- mytherra/mytherra#22369 Add steps for Transifex to release process (jonatack)
- mytherra/mytherra#22393 Added info to mytherra.conf doc (bliotti)
- mytherra/mytherra#22402 Install Rosetta on M1-macOS for qt in depends (hebasto)
- mytherra/mytherra#22432 Fix incorrect `testmempoolaccept` doc (glozow)
- mytherra/mytherra#22648 doc, test: improve i2p/tor docs and i2p reachable unit tests (jonatack)

Credits
=======

Thanks to everyone who directly contributed to this release:

- Aaron Clauson
- Adam Jonas
- amadeuszpawlik
- Amiti Uttarwar
- Andrew Chow
- Andrew Poelstra
- Anthony Towns
- Antoine Poinsot
- Antoine Riard
- apawlik
- apitko
- Ben Carman
- Ben Woosley
- benk10
- Bezdrighin
- Block Mechanic
- Brian Liotti
- Bruno Garcia
- Carl Dong
- Christian Decker
- coinforensics
- Cory Fields
- Dan Benjamin
- Daniel Kraft
- Darius Parvin
- Dhruv Mehta
- Dmitry Goncharov
- Dmitry Petukhov
- dplusplus1024
- dscotese
- Duncan Dean
- Elle Mouton
- Elliott Jin
- Emil Engler
- Ethan Heilman
- eugene
- Evan Klitzke
- Fabian Jahr
- Fabrice Fontaine
- fanquake
- fdov
- flack
- Fotis Koutoupas
- Fu Yong Quah
- fyquah
- glozow
- Gregory Sanders
- Guido Vranken
- Gunar C. Gessner
- h
- HAOYUatHZ
- Hennadii Stepanov
- Igor Cota
- Ikko Ashimine
- Ivan Metlushko
- jackielove4u
- James O'Beirne
- Jarol Rodriguez
- Joel Klabo
- John Newbery
- Jon Atack
- Jonas Schnelli
- João Barbosa
- Josiah Baker
- Karl-Johan Alm
- Kiminuo
- Klement Tan
- Kristaps Kaupe
- Larry Ruane
- lisa neigut
- Lucas Ontivero
- Luke Dashjr
- Maayan Keshet
- MarcoFalke
- Martin Ankerl
- Martin Zumsande
- Michael Dietz
- Michael Polzer
- Michael Tidwell
- Niklas Gögge
- nthumann
- Oliver Gugger
- parazyd
- Patrick Strateman
- Pavol Rusnak
- Peter Bushnell
- Pierre K
- Pieter Wuille
- PiRK
- pox
- practicalswift
- Prayank
- R E Broadley
- Rafael Sadowski
- randymcmillan
- Raul Siles
- Riccardo Spagni
- Russell O'Connor
- Russell Yanofsky
- S3RK
- saibato
- Samuel Dobson
- sanket1729
- Sawyer Billings
- Sebastian Falbesoner
- setpill
- sgulls
- sinetek
- Sjors Provoost
- Sriram
- Stephan Oeste
- Suhas Daftuar
- Sylvain Goumy
- t-bast
- Troy Giorshev
- Tushar Singla
- Tyler Chambers
- Uplab
- Vasil Dimov
- W. J. van der Laan
- willcl-ark
- William Bright
- William Casarin
- windsok
- wodry
- Yerzhan Mazhkenov
- Yuval Kogman
- Zero

As well as to everyone that helped with translations on
[Transifex](https://www.transifex.com/mytherra/mytherra/).
