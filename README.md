# uVisor IRQ blinky example

This is a simple example to show how to write a uVisor-secured threaded application with IRQ support. One LED blinks periodically from the public box main thread. The second LED is exclusively owned by a secure box and toggles when the user button is pressed.

Supported devices:

| Target         | Toolchain | Public box LED | Secure box LED | User button   | Baud rate |
|----------------|-----------|----------------|----------------|---------------|-----------|
| `K64F`         | `GCC_ARM` | `LED_BLUE`     | `LED_RED`      | `SW2`         | 9600      |
| `DISCO_F429ZI` | `GCC_ARM` | `LED1`         | `LED2`         | `USER_BUTTON` | 9600      |

Latest release: [mbed-os-5.3.x](https://github.com/ARMmbed/mbed-os-example-uvisor/releases/latest). Tested with [mbed-cli v1.0.0](https://github.com/ARMmbed/mbed-cli/releases/tag/1.0.0).

## Quickstart

The instructions here are specific to the `K64F` target, but can be easily extended to other supported targets as well.

For a release build, please enter:

```bash
$ mbed compile -m K64F -t GCC_ARM -c
```

You will find the resulting binary in `BUILD/K64F/GCC_ARM/mbed-os-example-uvisor.bin`. You can drag and drop it onto your board USB drive.

Press the reset button. You will see the public box LED blinking. Whenever you press the user button, the secure box LED will toggle. This interrupt is being served in the secure box and cannot be tampered with by the public box.

If you want, you can also read the output on the serial port:

```bash
$ screen /dev/tty.usbmodem1422 9600
```

You will see an output similar to the following one (assuming you press the user button after 4 loops count):

```
**** IRQ blinky uVisor example *****
Main loop count: 0
Main loop count: 1
Main loop count: 2
Main loop count: 3

Pressed switch, printing from interrupt - LED changed to 0

Main loop count: 4
Main loop count: 5
...
```

### Debug

When a debugger is connected, you can observe debug output from uVisor. Please note that these messages are sent through semihosting, which halts the program execution if a debugger is not connected. For more information please read the [Debugging uVisor on mbed OS](https://github.com/ARMmbed/uvisor/blob/master/docs/api/DEBUGGING.md) guide. To build a debug version of this example, please enter:

```bash
$ mbed compile -m K64F -t GCC_ARM --profile mbed-os/tools/profiles/debug.json -c
```

do not merge change
