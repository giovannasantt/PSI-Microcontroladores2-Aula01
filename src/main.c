#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define STACK_SIZE 1024
#define PRIORITY   5

#define PERIODO_A_MS 500
#define PERIODO_B_MS 500   // depois: 1000 / 500, e o desafio 1000 / 350

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)

static const struct gpio_dt_spec led_a = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led_b = GPIO_DT_SPEC_GET(LED1_NODE, gpios);

K_THREAD_STACK_DEFINE(stack_a, STACK_SIZE);
K_THREAD_STACK_DEFINE(stack_b, STACK_SIZE);
struct k_thread data_a;
struct k_thread data_b;

/* arg1 = ponteiro para o LED, arg2 = período de meia-onda em ms */
void blink_thread(void *arg1, void *arg2, void *arg3)
{
    const struct gpio_dt_spec *led = arg1;
    int intervalo = (int)(intptr_t)arg2;
    int64_t proximo = k_uptime_get();

    while (1) {
        gpio_pin_toggle_dt(led);
        proximo += intervalo;
        k_sleep(K_TIMEOUT_ABS_MS(proximo));
    }
}

int main(void)
{
    if (!gpio_is_ready_dt(&led_a) || !gpio_is_ready_dt(&led_b)) {
        return 0;
    }

    gpio_pin_configure_dt(&led_a, GPIO_OUTPUT_ACTIVE);
    gpio_pin_configure_dt(&led_b, GPIO_OUTPUT_INACTIVE);

    k_thread_create(&data_a, stack_a, K_THREAD_STACK_SIZEOF(stack_a),
                    blink_thread, (void *)&led_a, (void *)(intptr_t)1000, NULL,
                    PRIORITY, 0, K_NO_WAIT);

    k_thread_create(&data_b, stack_b, K_THREAD_STACK_SIZEOF(stack_b),
                    blink_thread, (void *)&led_b, (void *)(intptr_t)500, NULL,
                    PRIORITY, 0, K_NO_WAIT);

    return 0;
}

/* 
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
        int ret = set_rgb(1, 0);  vermelho + azul = magenta 
        if (ret < 0) {
            printk("Falha ao acender LEDs: %d\n", ret);
            return;
        }
        k_msleep(1000);

        ret = set_rgb(0, 1); /* apaga todos 
        if (ret < 0) {
            printk("Falha ao apagar LEDs: %d\n", ret);
            return;
        }
        k_msleep(350);
    }
} */




