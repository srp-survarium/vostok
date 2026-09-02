# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.tool - one module per external tool or operational helper.

    clangd            one-shot LSP queries over compile_commands.json: where is
                      this defined, who references it, what is its type. clang
                      is a READER of this MSVC 8 dialect - navigation is
                      reliable, its diagnostics are not build truth.
    toolchain         first dev-shell entry: wine prefix, staged libs, the ninja
                      graph, and the one-time target-side generation
    libs_release      cut a vostok-libs release: the GFx suite rebuilt from source
                      (vostok.build.gfx) dropped over the current package, zipped
    toolchain_release repackage a VS2008 install into the reproducible
                      vostok-toolchain tarball the flake pins
    libs              stage prebuilt third-party blobs into binaries.prebuilt/
    sizes             what the source tree costs, by extension
    breakpoints       find the target source line behind an address
"""
