#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define SLEEP_TIME_MS 500

// Define o LED usando Device Tree
#define GREEN_LED_NODE DT_ALIAS(led0)
#define RED_LED_NODE DT_ALIAS(led1)

// Verifica se o LED está definido no Device Tree
#if !DT_NODE_HAS_STATUS(RED_LED_NODE, okay) || \
    !DT_NODE_HAS_STATUS(GREEN_LED_NODE, okay) 
   
#error "Os aliases dos LEDs RGB nao estao definidos no Device Tree"
#endif

static const struct gpio_dt_spec red_led =
    GPIO_DT_SPEC_GET(RED_LED_NODE, gpios);
static const struct gpio_dt_spec green_led =
    GPIO_DT_SPEC_GET(GREEN_LED_NODE, gpios);

static const struct gpio_dt_spec *const leds[] = {
    &red_led,
    &green_led,
};

static int set_rgb (int red, int green){
    int ret = gpio_pin_set_dt(&red_led, red);
    if (ret < 0){
        return ret;
    }

    ret = gpio_pin_set_dt(&green_led, green);
    if(ret < 0){
        return ret;
    }
};


void main(void)
{
  for (size_t i = 0; i < ARRAY_SIZE(leds); i++) {
        if (!gpio_is_ready_dt(leds[i])) {
            printk("Controlador do LED nao esta pronto\n");
            return;
        }

        int ret = gpio_pin_configure_dt(leds[i], GPIO_OUTPUT_INACTIVE);
        if (ret < 0) {
            printk("Falha ao configurar LED: %d\n", ret);
            return;
        }
    }

     while (1) {
        int ret = set_rgb(1, 0); /* vermelho + azul = magenta */
        if (ret < 0) {
            printk("Falha ao acender LEDs: %d\n", ret);
            return;
        }
        k_msleep(1000);

        ret = set_rgb(0, 1); /* apaga todos */
        if (ret < 0) {
            printk("Falha ao apagar LEDs: %d\n", ret);
            return;
        }
        k_msleep(500);
    }
}