# simple driver to be used for the hands-on IRQ workshop section

The driver does need device tree modification such as:

<pre><code>push_led_irq_device {
  compatible = "st,push_led_irq";
  status = "okay";
  pa13button-gpios = <&gpioa 13 GPIO_ACTIVE_LOW>;
  greenled-gpios = <&gpioa 14 GPIO_ACTIVE_LOW>;
};<pre><code>
