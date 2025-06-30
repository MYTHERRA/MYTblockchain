# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libmytherra_cli*         | RPC client functionality used by *mytherra-cli* executable |
| *libmytherra_common*      | Home for common functionality shared by different executables and libraries. Similar to *libmytherra_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libmytherra_consensus*   | Stable, backwards-compatible consensus functionality used by *libmytherra_node* and *libmytherra_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libmytherraconsensus*    | Shared library build of static *libmytherra_consensus* library |
| *libmytherra_kernel*      | Consensus engine and support library used for validation by *libmytherra_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libmytherraqt*           | GUI functionality used by *mytherra-qt* and *mytherra-gui* executables |
| *libmytherra_ipc*         | IPC functionality used by *mytherra-node*, *mytherra-wallet*, *mytherra-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libmytherra_node*        | P2P and RPC server functionality used by *mytherrad* and *mytherra-qt* executables. |
| *libmytherra_util*        | Home for common functionality shared by different executables and libraries. Similar to *libmytherra_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libmytherra_wallet*      | Wallet functionality used by *mytherrad* and *mytherra-wallet* executables. |
| *libmytherra_wallet_tool* | Lower-level wallet functionality used by *mytherra-wallet* executable. |
| *libmytherra_zmq*         | [ZeroMQ](../zmq.md) functionality used by *mytherrad* and *mytherra-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libmytherra_consensus* and *libmytherra_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libmytherra_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libmytherra_node* code lives in `src/node/` in the `node::` namespace
  - *libmytherra_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libmytherra_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libmytherra_util* code lives in `src/util/` in the `util::` namespace
  - *libmytherra_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

mytherra-cli[mytherra-cli]-->libmytherra_cli;

mytherrad[mytherrad]-->libmytherra_node;
mytherrad[mytherrad]-->libmytherra_wallet;

mytherra-qt[mytherra-qt]-->libmytherra_node;
mytherra-qt[mytherra-qt]-->libmytherraqt;
mytherra-qt[mytherra-qt]-->libmytherra_wallet;

mytherra-wallet[mytherra-wallet]-->libmytherra_wallet;
mytherra-wallet[mytherra-wallet]-->libmytherra_wallet_tool;

libmytherra_cli-->libmytherra_util;
libmytherra_cli-->libmytherra_common;

libmytherra_common-->libmytherra_consensus;
libmytherra_common-->libmytherra_util;

libmytherra_kernel-->libmytherra_consensus;
libmytherra_kernel-->libmytherra_util;

libmytherra_node-->libmytherra_consensus;
libmytherra_node-->libmytherra_kernel;
libmytherra_node-->libmytherra_common;
libmytherra_node-->libmytherra_util;

libmytherraqt-->libmytherra_common;
libmytherraqt-->libmytherra_util;

libmytherra_wallet-->libmytherra_common;
libmytherra_wallet-->libmytherra_util;

libmytherra_wallet_tool-->libmytherra_wallet;
libmytherra_wallet_tool-->libmytherra_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class mytherra-qt,mytherrad,mytherra-cli,mytherra-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libmytherra_wallet* and *libmytherra_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libmytherra_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libmytherra_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libmytherra_common* should serve a similar function as *libmytherra_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libmytherra_util* and *libmytherra_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for mytherra-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libmytherra_kernel* is not supposed to depend on *libmytherra_common*, only *libmytherra_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libmytherra_kernel* should only depend on *libmytherra_util* and *libmytherra_consensus*.

- The only thing that should depend on *libmytherra_kernel* internally should be *libmytherra_node*. GUI and wallet libraries *libmytherraqt* and *libmytherra_wallet* in particular should not depend on *libmytherra_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libmytherra_consensus*, *libmytherra_common*, and *libmytherra_util*, instead of *libmytherra_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libmytherraqt*, *libmytherra_node*, *libmytherra_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libmytherra_node* to *libmytherra_kernel* as part of [The libmytherrakernel Project #24303](https://github.com/mytherra/mytherra/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/mytherra/mytherra/issues/15732)
