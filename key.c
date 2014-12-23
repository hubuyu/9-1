#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/input.h>

#define PIN	25

struct input_dev* button_devp;

struct button_irq_desc{
  int irq;
  int pin;
  int number;
  char *name;
 };

static struct button_irq_desc button_irqs[] = {
  {gpio_to_irq(PIN), PIN, 0, "KEY0"},
 };

static irqreturn_t  button_interrupt(int irq, void* dev_id)
{
	struct button_irq_desc *button_irqs = (struct button_irq_desc*)dev_id;
	int down; 
  
	down = gpio_get_value(PIN);

    input_report_key(button_devp, BTN_0, down);
    input_sync(button_devp);
	return IRQ_RETVAL(IRQ_HANDLED);
 }

static int __init button_init(void)
 {
    int i = 0 ;
	int err;
  
	button_devp = input_allocate_device();

	err = request_irq(button_irqs[i].irq, button_interrupt, IRQ_TYPE_EDGE_BOTH, button_irqs[i].name, (void *)&button_irqs[i]);
  
   set_bit(EV_KEY, button_devp->evbit);
   set_bit(BTN_0, button_devp->keybit);

  err = input_register_device(button_devp);
   
   return err;
 }

static void __exit button_exit(void)
{
	int i = 0 ;
	input_unregister_device(button_devp);
	disable_irq(button_irqs[i].irq);
	free_irq(button_irqs[i].irq, (void*)&button_irqs[i]);
 }

module_init(button_init);
module_exit(button_exit);


MODULE_AUTHOR("hubin");
MODULE_LICENSE("GPL");

