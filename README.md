# vostok-2.0-dsgn

### Prerequisites

1. VS2008 SP1 Professional
    * Install VS2008 from here: https://archive.org/details/en_visual_studio_2008_professional_x86_dvd_x14-26326_202310
    * Install SP1 patch from here: https://www.microsoft.com/en-us/download/details.aspx?id=13276

2. Ghidra 11.4.0
    * Used by VS build scripts to generate diffable `.obj` files.
    * Requires OpenJDK to be installed also.  (Just install the newest one)
    * Install from here: https://github.com/NationalSecurityAgency/ghidra/releases/tag/Ghidra_11.4_build
    * Install Delinker Extension from here: https://github.com/boricj/ghidra-delinker-extension/releases
    * `GHIDRA_HOME` global environment variable needs to be set for `.bat` scripts, the default value is `GHIDRA_HOME=C:\Program Files\ghidra_11.4_PUBLIC`

3. DirectX SDK June 2010
    * Install from here: https://www.microsoft.com/en-us/download/details.aspx?id=6812
    * If you get S1023 error, uninstall all packages on the system related to VS2010 and try again.
    * Needs to be configured in VS2008:
        - Tools > Options > Projects and Solutions > VC++ Directories > Include files > C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include
        - Tools > Options > Projects and Solutions > VC++ Directories > Library files > C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86

4. Misc. tools
    * `objdiff` - Install newest version from here: https://github.com/encounter/objdiff
    * `python` - Install newest version from here: https://www.python.org/downloads/
    * `rustc` - Install nightly Rust toolchain like so: `rustup default nightly`

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
    * Then clone those repositories (at the same level)
    ```
    git clone git@github.com:srp-survarium/srp.git
    git clone git@github.com:srp-survarium/xray-structure.git
    git clone git@github.com:srp-survarium/vostok-structure.git
    git clone git@github.com:srp-survarium/vostok-coff-delinker.git
    git clone git@github.com:srp-survarium/vostok-libs.git
    ```

## Setting up

1. Install missing proprietary libraries by running this command from the root of the project
```terminal
python ./scripts/copy_lib_files.py ../vostok-libs/sources/ sources/
```

2. Build `survarium - PC - Windows` solution in VS2008 in `Debug` (not `Debug(static)`) configuration. It will fail, but this is fine, we only need `zlibn` to be built.
3. Build `game_core` solution in VS2008 in `Master Gold` configuration.
4. Build `survarium - PC - Windows` solutin in VS2008 in `Master Gold` configuration. Build should succeeded.

