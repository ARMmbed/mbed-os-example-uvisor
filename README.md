# uVisor IRQ blinky example

[![Build Status](https://travis-ci.org/ARMmbed/mbed-os-example-uvisor.svg?branch=master)](https://travis-ci.org/ARMmbed/mbed-os-example-uvisor)

<span class="warnings">**Warning**: uVisor is superseded by the Secure Partition Manager (SPM) defined in the ARM Platform Security Architecture (PSA). uVisor is deprecated as of Mbed OS 5.10, and being replaced by a native PSA-compliant implementation of SPM.</span>

This is a simple example to show how to write a uVisor-secured threaded application with IRQ support. One LED blinks periodically from the public box main thread. The second LED is exclusively owned by a secure box and toggles when the user button is pressed.

Supported devices:

| Target            | Toolchain | Public box LED | Secure box LED | User button   | Baud rate |
|-------------------|-----------|----------------|----------------|---------------|-----------|
| `K64F`            | `GCC_ARM` | `LED_BLUE`     | `LED_RED`      | `SW2`         | 9600      |
| `DISCO_F429ZI`    | `GCC_ARM` | `LED1`         | `LED2`         | `USER_BUTTON` | 9600      |
| `EFM32GG_STK3700` | `GCC_ARM` | `LED1`         | `LED2`         | `SW0`         | 9600      |

## Quickstart

The instructions here are specific to the `K64F` target, but can be easily extended to other supported targets as well.

For a release build, please enter:

```bash
$ mbed compile -m K64F -t GCC_ARM -c
```

You will find the resulting binary in `BUILD/K64F/GCC_ARM/mbed-os-example-uvisor.bin`. You can drag and drop it onto your board USB drive.

Press the reset button. You will see the public box LED blinking. Whenever you press the user button, the secure box LED will toggle. This interrupt is being served in the secure box and cannot be tampered with by the public box.

If you want, you can also read the output on the serial port. The example below is using the program "screen", and is connected to port tty.usbmodem1422, with a baud rate of 9600.  To determine the proper port your board is connected to, you can run the command 'mbed detect':

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

Pressed switch, printing from interrupt - LED now on.

Main loop count: 4
Main loop count: 5
...
```

### Debug

When a debugger is connected, you can observe debug output from uVisor. Please note that these messages are sent through semihosting, which halts the program execution if a debugger is not connected. For more information please read the [Debugging uVisor on mbed OS](https://github.com/ARMmbed/uvisor/blob/master/docs/lib/DEBUGGING.md) guide. To build a debug version of this example, please enter:

```bash
$ mbed compile -m K64F -t GCC_ARM --profile mbed-os/tools/profiles/debug.json -c
```

## Known issues

- Use of exporters for multiple IDEs is not supported at the moment.
- uVisor will halt the system on unregistered interrupts arrival. Do not attempt to push any SW buttons not listed in this document.

## Troubleshooting

If you have problems, you can review the [documentation](https://os.mbed.com/docs/latest/tutorials/debugging.html) for suggestions on what could be wrong and how to fix it.
