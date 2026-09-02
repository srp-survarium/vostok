# Windows / VS2008 setup (legacy)

> **This is the original, fully manual Windows workflow.** The supported path is
> now the Nix/Linux one in the [top-level README](../README.md), which builds the
> exact VS2008 toolchain under Wine for you. Keep this around for working natively
> on Windows or for reference on what the toolchain actually contains.

Decompilation of the Vostok Engine used by Survarium (version v0.100b).

### Prerequisites

1. DirectX SDK June 2010
    * Install from here: https://www.microsoft.com/en-us/download/details.aspx?id=6812
    * If you get S1023 error, uninstall all packages on the system related to VS2010 and try again.

2. VS2008 SP1 Professional
    * Install VS2008 from here: https://archive.org/details/en_visual_studio_2008_professional_x86_dvd_x14-26326_202310
    * Install SP1 patch from here: https://www.microsoft.com/en-us/download/details.aspx?id=13276
    * Install patch enabling macros: https://www.microsoft.com/en-US/download/details.aspx?id=42540
    * DirectX needs to be configured in VS2008:
        - Tools > Options > Projects and Solutions > VC++ Directories > Include files > C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include
        - Tools > Options > Projects and Solutions > VC++ Directories > Library files > C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86
    * Load VS2008 macros:
        - Tools > Macros > Load Macro Project... > {REPOSITORY}/scripts/vs/macro/macro.vsmacros > Allow

3. Tooling
    * `objdiff` - Install newest version from here: https://github.com/encounter/objdiff
    * `python` - Install newest version from here: https://www.python.org/downloads/
    * `rustc` - Install nightly Rust toolchain like so: `rustup default nightly`
    * `IDA Free` - Install from here (requires getting free license): https://hex-rays.com/ida-free

4. Git projects
    * Other projects need to be cloned at the same level this project is cloned.
    * Before you do that, make sure long paths on Windows are enabled
        - In Registry (requires administrator privileges):
            ```
            reg add "HKLM\SYSTEM\CurrentControlSet\Control\FileSystem" /v LongPathsEnabled /t REG_DWORD /d 1 /f
            ```
        - In Git (requires administrator privileges):
            ```
            git config --system core.longpaths true
            ```
    * At the root level (which is a level where all Git repositories will be stored, including this one) clone:
        ```
        git clone git@github.com:srp-survarium/vostok.git
        git clone git@github.com:srp-survarium/vostok-structure.git
        git clone git@github.com:srp-survarium/vostok-delinker.git
        git clone git@github.com:srp-survarium/xray-structure.git
        ```
    * Download proprietary libraries and unpack them at the root level: https://drive.google.com/file/d/1ShmDiJArDcOiuPA0DyswEzhXHeRa0Psx/view?usp=sharing
    * Then install them by running this command:
        ```terminal
        set PYTHONPATH=%CD%\scripts
        python -m vostok.tool.libs
        ```

6. Game builds
    * Download and install the game from here: https://archive.org/details/vostok_engine_v0.1_build_802_internal_id_489_may_9_2013
    * Other builds are available here: https://archive.org/search?query=creator%3A%22Vostok+Games%22


7. Environment variables
    * Environment variables should be set up for a local user for scripts to work correctly. Otherwise you can install required tooling to the default paths.
    * `IDA_HOME` - Path to installed IDA. Default value: `IDA_HOME=C:\Program Files\IDA Free 9.1`.
    * `OBJDIFF_HOME` - Path to installed objdiff. Default value: `OBJDIFF_HOME=C:\Program Files\scripts`.
    * `SURVARIUM_BIN` - Path to the game executable. Default value: `D:\Projects\Survarium\binaries\win32`.

8. Recommendations
    * On Windows 11 it is recommended to set up Dev Drive. This will speed up filesystem operations significantly.
    * `ripgrep` helps when searching for engine functions, classes, arguments, and other code references across all repositories.

## Setting up

1. Build `survarium - PC - DirectX 11` solution in VS2008 in `Master Gold` configuration for `Win32`. Build should succeed.

2. Setup `objdiff`:
    * Run `.\build_target.bat` placed in `vostok-delinker` repository.
    * Now you can choose any object file (e.g. `vostok\collision\box_geometry_instance.obj`) and start matching assembly!


## Quick guide on matching assembly

See [the matching guide](binary_matching/matching_guide.md).
