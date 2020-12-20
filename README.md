# Windows Kernel Mode Driver

Learning of how to create Windows kernel mode drivers, ways of intercepting API calls in kernel mode.

- Implement a virtual device driver that tracks the start of some process X. When this process starts the driver starts another process Y. The driver stops process Y as soon as process X terminates for some reason.

- Implement a virtual device driver that tracks changes in Windows registry by a given process and creates a log file.
