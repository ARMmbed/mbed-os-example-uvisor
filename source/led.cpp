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

UVISOR_BOX_NAMESPACE(NULL);
UVISOR_BOX_HEAPSIZE(8192);
UVISOR_BOX_MAIN(my_box_main, osPriorityNormal, UVISOR_BOX_STACK_SIZE);
UVISOR_BOX_CONFIG(my_box, acl, UVISOR_BOX_STACK_SIZE, my_box_context);

static void my_box_switch_irq(void)
{
    /* flip LED state */
    *uvisor_ctx->led = !*uvisor_ctx->led;

    /* print LED state on serial port */
    uvisor_ctx->pc->printf(
        "\nPressed SW2, printing from interrupt - LED changed to %i\n\n",
        (int)(*uvisor_ctx->led));
}

static void my_box_main(const void *)
{
    /* allocate serial port to ensure that code in this secure box
     * won't touch handle in the default security context when printing */
    RawSerial *pc;    
    if(!(pc = new RawSerial(USBTX, USBRX)))
        return;
    /* remember serial driver for IRQ routine */
    uvisor_ctx->pc = pc;

    /* allocate a box-specific LED */
    if(!(uvisor_ctx->led = new DigitalOut(SECURE_LED)))
        pc->printf("ERROR: failed to allocate memories for LED\n");
    else
    {
        /* turn LED off by default */
        *uvisor_ctx->led = LED_OFF;

        /* allocate a box-specific switch handler */
        if(!(uvisor_ctx->sw = new InterruptIn(SW2)))
            pc->printf("ERROR: failed to allocate memories for SW1\n");
        else
        {
            /* register handler for switch SW1 */
            uvisor_ctx->sw->mode(PullUp);
            uvisor_ctx->sw->fall(my_box_switch_irq);

            /* no problem to return here as everything is initialized */
            return;
        }

        delete uvisor_ctx->led;
    }
    delete pc;
}
