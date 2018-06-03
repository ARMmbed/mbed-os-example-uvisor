/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"
#include "main-hw.h"
#include "mem_map.h"

#warning "Warning: uVisor is superseded by the Secure Partition Manager (SPM) defined in the ARM Platform Security Architecture (PSA). \
          uVisor is deprecated as of Mbed OS 5.10, and being replaced by a native PSA-compliant implementation of SPM."

/* Create ACLs for main box. */
MAIN_ACL(g_main_acl);

/* Enable uVisor. */
UVISOR_SET_MODE_ACL(UVISOR_ENABLED, g_main_acl);
UVISOR_SET_PAGE_HEAP(8 * 1024, 5);

static void example_halt_error(THaltError reason, const THaltInfo *halt_info);

UVISOR_PUBLIC_BOX_DEBUG_DRIVER(example_halt_error);

static void example_halt_error(THaltError reason, const THaltInfo *halt_info) {

	const MemMap * map = NULL;
	if (halt_info->bfar) {
		map = memory_map_name(halt_info->bfar);
	}

	printf("\n");
	printf("  Address:           0x%08X\n", halt_info->bfar);
	printf("  Region/Peripheral: %s\n", (map ? map->name : "[not available]"));
	printf("    Base address:    0x%08X\n", (map ? map->base : halt_info->bfar));
	printf("    End address:     0x%08X\n", (map ? map->end : halt_info->bfar));

	return;
}

/* Targets with an ARMv7-M MPU needs this space adjustment to prevent a runtime
 * memory overflow error. The code below has been output directly by uVisor. */
#if defined(TARGET_EFM32GG_STK3700) || defined(TARGET_DISCO_F429ZI)
uint8_t __attribute__((section(".keep.uvisor.bss.boxes"), aligned(32))) __boxes_overhead[8064];
#endif

int main(void)
{
    DigitalOut led(MAIN_LED);

    printf("\r\n***** IRQ blinky uVisor example *****\r\n");

    size_t count = 0;

    while (1) {
        printf("Main loop count: %d\r\n", count++);
        led = !led;

        /* Blink once per second. */
        Thread::wait(500);
    }

    return 0;
}
