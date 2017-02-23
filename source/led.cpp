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

typedef struct {
    InterruptIn * sw;
    DigitalOut * led;
    RawSerial * pc;
} my_box_context;

static const UvisorBoxAclItem acl[] = {
};

static void my_box_main(const void *);

/* Box configuration
 * We need 1kB of stack both in the main and interrupt threads as both of them
 * use printf. */
UVISOR_BOX_NAMESPACE(NULL);
UVISOR_BOX_HEAPSIZE(3072);
UVISOR_BOX_MAIN(my_box_main, osPriorityNormal, 1024);
UVISOR_BOX_CONFIG(my_box, acl, 1024, my_box_context);

/* FIXME: The guard is needed for backwards-compatibility reasons. Remove it
 *        when mbed OS is updated. */
#ifdef __uvisor_ctx
#define uvisor_ctx ((my_box_context *) __uvisor_ctx)
#endif

static void my_box_switch_irq(void)
{
    /* Flip LED state. */
    *uvisor_ctx->led = !*uvisor_ctx->led;

    /* Print LED state on serial port. */
    uvisor_ctx->pc->printf("\r\nPressed switch, printing from interrupt - LED changed to %i\r\n\r\n",
                           (int) (*uvisor_ctx->led));
}

static void my_box_main(const void *)
{
    /* Allocate the serial port to ensure that code in this secure box won't
     * touch handles in the default security context when printing. */
    RawSerial * pc;
    if (!(pc = new RawSerial(USBTX, USBRX))) {
        return;
    }

    /* Remember serial driver for IRQ routine. */
    uvisor_ctx->pc = pc;

    /* Allocate a box-specific LED. */
    if (!(uvisor_ctx->led = new DigitalOut(SECURE_LED))) {
        pc->printf("ERROR: failed to allocate memories for LED\r\n");
    } else {
        /* Turn LED off by default */
        *uvisor_ctx->led = LED_OFF;

        /* Allocate a box-specific switch handler. */
        if (!(uvisor_ctx->sw = new InterruptIn(SECURE_SWITCH))) {
            pc->printf("ERROR: failed to allocate memories for switch\r\n");
        } else {
            /* Register handler for switch. */
            uvisor_ctx->sw->mode(SECURE_SWITCH_PULL);
            uvisor_ctx->sw->fall(my_box_switch_irq);

            /* No problem to return here as everything is initialized. */
            return;
        }
        delete uvisor_ctx->led;
    }
    delete pc;
}
