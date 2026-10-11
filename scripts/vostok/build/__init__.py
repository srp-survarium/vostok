# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.build - driving the era toolchain.

    ninja_regen   .vcprojs + sources -> binaries/ninja/build.ninja, write-if-
                  changed so a no-op regen bumps no mtimes and rebuilds nothing.
                  Includes are tracked, so editing a header recompiles its TUs.
    ninja         run that graph under Wine, with the watchdog that stops a
                  stalled mspdbsrv from holding the build pipe for ten minutes.
    native_crt    native VC90 CRT for the Wine prefix
    rebuild       `vostok build`: toolchain setup, graph refresh, ninja.
"""
