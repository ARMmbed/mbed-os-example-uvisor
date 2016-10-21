# uVisor IRQ Blinky Example

This is a simple example to show how to write a uVisor-secured threaded application with IRQ support. Two LEDs will blink - one from the main thread (blue) and the other (red) from an interrupt connected to the developent board switch SW2 and change colour whenever the switch SW2 is pressed.

## Building

The example currently only works on K64F with the GCC_ARM toolchain.

### Release

For a release build, please enter:

```bash
$ mbed compile -m K64F -t GCC_ARM -c
```

You will find the resulting binary in `BUILD/K64F/GCC_ARM/mbed-os-example-uvisor.bin`. You can drag and drop it onto your board USB drive.

### Debug

When a debugger is connected, you can observe debug output from uVisor. Please note that these messages are sent through semihosting, which halts the program execution if a debugger is not connected. For more information please read the [Debugging uVisor on mbed OS](https://github.com/ARMmbed/uvisor/blob/master/docs/api/DEBUGGING.md) guide. To build a debug version of the program:

```bash
$ mbed compile -m K64F -t GCC_ARM --profile mbed-os/tools/profiles/debug.json -c
```
