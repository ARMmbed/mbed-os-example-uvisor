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
    *uvisor_ctx->led = !*uvisor_ctx->led;
}

static void my_box_main(const void *)
{
    /* allocate serial port to ensure that code in this secure box
     * won't touch handle in the default security context when printing */
    RawSerial *pc;    
    if(!(pc = new RawSerial(USBTX, USBRX)))
        return;

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
