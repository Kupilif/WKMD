# Windows Kernel Mode Driver

## Description

Learning of how to create Windows kernel mode drivers, ways of intercepting API calls in kernel mode.

- Implement a virtual device driver that tracks the start of some process X. When this process starts the driver starts another process Y. The driver stops process Y as soon as process X terminates for some reason.

- Implement a virtual device driver that tracks changes in Windows registry by a given process and creates a log file.

## Build and run

### Host environment

- Install `Visual Studio` with driver project templates
- Instal `Windows Driver Kit`

### Visual Studio project settings

- Create project from `Empty WDM Driver` template
- Files with code should have `*.c` extension, not `*.cpp`
- `Project Settings` -> `C / C++` -> `Warnings Level` -> Set level `3`
- `Project Settings` -> `Linker` -> `Command Line` -> `Extra Parameters` -> Add `/INTEGRITYCHECK` option
- `Project Settings` -> `Driver Settings` -> `Target OS Version` -> Set bitness of target OS
- `Project Settings` -> `Inf2Cat` -> `Run Inf2Cat` - Set `No`
- `Project Settings` -> `Driver Signing` -> `Sign Mode` -> Set `Test Sign`

### Test environment

- Enable kernel debug with `bcdedit /debug on`
- Enable test certificates via `bcdedit.exe -set TESTSIGNING ON`
- Install Visual C++ Redistributable Packages for Visual Studio

### Deploy and run

- Use Visual Studio for live debug and automatic deployment of driver to test environment
- Use `DebugView` to observe driver logs without Visual Studio live debug
- Use `KmdManager` to manually control driver:
    - `Register` - create driver hive in Windows registry
    - `Run` - load driver (`DriverEntry` routine would be invoked)
    - `Stop` - unload driver (`DriverUnload` routine would be invoked)
    - `Unregister` - delete driver hive from Windows registry
