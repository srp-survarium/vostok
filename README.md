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


## Setting up

1. Missing

1. To get missing libraries clone `vostok-libs` project and then run this command:
```terminal
python ./scripts/copy_lib_files.py ../vostok-libs/sources/ sources/
```

2. You can download VS2008 from here: https://archive.org/details/en_visual_studio_2008_professional_x86_dvd_x14-26326_202310
3. The hash can be verified here: https://files.rg-adguard.net/search
