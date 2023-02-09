#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/of_address.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

struct gpio_desc *pa13button, *greenled;
int irq;
unsigned long irqflags;


static irqreturn_t isr_top(int irq, void *dev_id)
{
	int value;

	pr_debug("openstlinux-irq-test-driver-top - %s: IRQ %d received\n",__func__,irq);

	/* Get current greenled value */
	value=gpiod_get_value(greenled);

	/* Set new complementary value */
	pr_debug("openstlinux-irq-test-driver-top - %s: Set green led to %d\n",__func__,!value);
	gpiod_set_value(greenled, !value);

	return IRQ_WAKE_THREAD;	
}

static irqreturn_t isr_bottom(int irq, void *dev_id)
{

	pr_debug("openstlinux-irq-test-driver-bottom - %s: IRQ %d received\n",__func__,irq);

	return IRQ_HANDLED;
}


static int gpio_init_probe(struct platform_device *pdev)
{
	int error;

	printk("openstlinux-irq-test-driver init\n");

	/* "pa13button" and "greenled" labels are matching the device tree declaration */
	pa13button = devm_gpiod_get(&pdev->dev, "pa13button", GPIOD_IN);
	greenled = devm_gpiod_get(&pdev->dev, "greenled", GPIOD_OUT_LOW);

	/* Get irq number from gpiolib framework */
	irq = gpiod_to_irq(pa13button);
	if (irq < 0) return irq;

	/* We want to generate an irq when the pa13 button will be pressed */
	irqflags = IRQF_TRIGGER_FALLING;

	/* Use irqchip framework to associate isr to irq */
	error = devm_request_threaded_irq(&pdev->dev, irq, isr_top, isr_bottom, irqflags, pdev->name, 0);

	if (error < 0) {
	dev_err(&pdev->dev, "Unable to claim irq %d; error %d\n",irq, error);
	return error;
	}

	return(0);
}

static int gpio_exit_remove(struct platform_device *pdev)
{
	printk("openstlinux-irq-test-driver exit\n");

	return(0);
}

/* this structure will do the matching with the device tree */
/* if it is not matching the compatible field of DT, nothing will happen */
static struct of_device_id push_led_match[] = {
	{.compatible = "st,openstlinux-irq-test-driver"},
	{/* end node */}
};

static struct platform_driver openstlinux_irq_test_driver = {
	.probe = gpio_init_probe,
	.remove = gpio_exit_remove,
	.driver = {
		.name = "openstlinux-irq-test-driver",
		.owner = THIS_MODULE,
		.of_match_table = push_led_match,
    }
};

module_platform_driver(openstlinux_irq_test_driver);

MODULE_AUTHOR("Martin Lesniak");
MODULE_DESCRIPTION("openstlinux-irq-test-driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("openstlinux-irq-test-driver");
