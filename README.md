# vostok-2.0-dsgn

### Prerequisites

1. DirectX SDK June 2010
    * Install from here: https://www.microsoft.com/en-us/download/details.aspx?id=6812
    * If you get S1023 error, uninstall all packages on the system related to VS2010 and try again.

2. VS2008 SP1 Professional
    * Install VS2008 from here: https://archive.org/details/en_visual_studio_2008_professional_x86_dvd_x14-26326_202310
    * Install SP1 patch from here: https://www.microsoft.com/en-us/download/details.aspx?id=13276
    * DirectX needs to be configured in VS2008:
        - Tools > Options > Projects and Solutions > VC++ Directories > Include files > C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include
        - Tools > Options > Projects and Solutions > VC++ Directories > Library files > C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86

3. Ghidra 11.4.0
    * Used by VS build scripts to generate diffable `.obj` files.
    * Requires OpenJDK to be installed also.  (Just install the newest one)
    * Install from here: https://github.com/NationalSecurityAgency/ghidra/releases/tag/Ghidra_11.4_build
    * Install Delinker Extension from here: https://github.com/boricj/ghidra-delinker-extension/releases
    * `GHIDRA_HOME` global environment variable needs to be set for `.bat` scripts, the default value is `GHIDRA_HOME=C:\Program Files\ghidra_11.4_PUBLIC`

4. Misc. tools
    * `objdiff` - Install newest version from here: https://github.com/encounter/objdiff
    * `python` - Install newest version from here: https://www.python.org/downloads/
    * `rustc` - Install nightly Rust toolchain like so: `rustup default nightly`
    * `IDA Free` - Install from here (requires getting free license): https://hex-rays.com/ida-free

5. Misc. projects
    * Other projects need to be cloned at the same level this project is cloned.
    * Before you do that, make sure long paths on Windows are enabled
        - In Registry (requires administrator privilidges):
            ```
            reg add "HKLM\SYSTEM\CurrentControlSet\Control\FileSystem" /v LongPathsEnabled /t REG_DWORD /d 1 /f
            ```
        - In Git (requires administrator privilidges):
            ```
            git config --system core.longpaths true
            ```
    * At the root level (which is a level where all Git repositories will be stored, including this one) clone:
        ```
        git clone git@github.com:srp-survarium/vostok.git
        git clone git@github.com:srp-survarium/vostok-structure.git
        git clone git@github.com:srp-survarium/vostok-coff-delinker.git
        git clone git@github.com:srp-survarium/xray-structure.git
        ```
    * Download proprietary libraries and unpack them at the root level: https://drive.google.com/file/d/1ShmDiJArDcOiuPA0DyswEzhXHeRa0Psx/view?usp=sharing
    * Then install them by running this command:
        ```terminal
        python ./scripts/copy_lib_files.py
        ```

## Setting up

1. Build `survarium - PC - DirectX 11` solution in VS2008 in `Debug` (not `Debug(static)`) configuration for `Win32`. It will fail, but this is fine, we only need `zlibn` to be built.
    * TODO: This step should be removed and be set as a dependency in `survarium - PC - DirectX 11`.
2. Build `survarium - PC - DirectX 11` solutin in VS2008 in `Master Gold` configuration for `Win32`. Build should succeeded.

3. Setup `objdiff`:
    * Open Project `Settings`.
    * Set project directory to cloned `vostok-coff-delinker` project.
    * Set target build directory to `target`.
    * Set base build directory to `base`.
    * Disable relocation diffs: Diff Options > Function relocation diffs > None.
    * Now you can choose any object file (e.g. `vostok\collision\box_geometry_instance.obj`) and start matching assembly!


## Quick guide on matching assembly

https://gist.github.com/sushi-shi/8bf16f82c3b1c65fd357d73ecfda909e
