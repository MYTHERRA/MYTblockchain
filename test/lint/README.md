This folder contains lint scripts.

Running locally
===============

To run linters locally with the same versions as the CI environment, use the included
Dockerfile:

```sh
cd ./ci/lint
docker build -t mytherra-linter .

cd /root/of/mytherra/repo
docker run --rm -v $(pwd):/mytherra -it mytherra-linter
```

After building the container once, you can simply run the last command any time you
want to lint.


check-doc.py
============
Check for missing documentation of command line options.

commit-script-check.sh
======================
Verification of [scripted diffs](/doc/developer-notes.md#scripted-diffs).
Scripted diffs are only assumed to run on the latest LTS release of Ubuntu. Running them on other operating systems
might require installing GNU tools, such as GNU sed.

git-subtree-check.sh
====================
Run this script from the root of the repository to verify that a subtree matches the contents of
the commit it claims to have been updated to.

```
Usage: test/lint/git-subtree-check.sh [-r] DIR [COMMIT]
       test/lint/git-subtree-check.sh -?
```

- `DIR` is the prefix within the repository to check.
- `COMMIT` is the commit to check, if it is not provided, HEAD will be used.
- `-r` checks that subtree commit is present in repository.

To do a full check with `-r`, make sure that you have fetched the upstream repository branch in which the subtree is
maintained:
* for `src/secp256k1`: https://github.com/mytherra-core/secp256k1.git (branch master)
* for `src/leveldb`: https://github.com/mytherra-core/leveldb-subtree.git (branch mytherra-fork)
* for `src/crypto/ctaes`: https://github.com/mytherra-core/ctaes.git (branch master)
* for `src/crc32c`: https://github.com/mytherra-core/crc32c-subtree.git (branch mytherra-fork)
* for `src/minisketch`: https://github.com/sipa/minisketch.git (branch master)

To do so, add the upstream repository as remote:

```
git remote add --fetch secp256k1 https://github.com/mytherra-core/secp256k1.git
```

all-lint.py
===========
Calls other scripts with the `lint-` prefix.
