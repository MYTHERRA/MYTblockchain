#!/usr/bin/env python3
# Copyright (c) 2019-2022 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

from test_framework.test_framework import MytherraTestFramework

class TestShell:
    """Wrapper Class for MytherraTestFramework.

    The TestShell class extends the MytherraTestFramework
    rpc & daemon process management functionality to external
    python environments.

    It is a singleton class, which ensures that users only
    start a single TestShell at a time."""

    class __TestShell(MytherraTestFramework):
        def add_options(self, parser):
            self.add_wallet_options(parser)

        def set_test_params(self):
            pass

        def run_test(self):
            pass

        def setup(self, **kwargs):
            if self.running:
                print("TestShell is already running!")
                return

            # Num_nodes parameter must be set
            # by MytherraTestFramework child class.
            self.num_nodes = 1

            # User parameters override default values.
            for key, value in kwargs.items():
                if hasattr(self, key):
                    setattr(self, key, value)
                elif hasattr(self.options, key):
                    setattr(self.options, key, value)
                else:
                    raise KeyError(key + " not a valid parameter key!")

            super().setup()
            self.running = True
            return self

        def shutdown(self):
            if not self.running:
                print("TestShell is not running!")
            else:
                super().shutdown()
                self.running = False

        def reset(self):
            if self.running:
                print("Shutdown TestShell before resetting!")
            else:
                self.num_nodes = None
                super().__init__()

    instance = None

    def __new__(cls):
        # This implementation enforces singleton pattern, and will return the
        # previously initialized instance if available
        if not TestShell.instance:
            TestShell.instance = TestShell.__TestShell()
            TestShell.instance.running = False
        return TestShell.instance

    def __getattr__(self, name):
        return getattr(self.instance, name)

    def __setattr__(self, name, value):
        return setattr(self.instance, name, value)
