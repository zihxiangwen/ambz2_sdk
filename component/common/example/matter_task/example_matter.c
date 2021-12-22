#include "FreeRTOS.h"
#include "task.h"
#include <platform/platform_stdlib.h>
#include "basic_types.h"
#include "platform_opts.h"
#include "section_config.h"
#include "example_matter.h"
#include "gpio_api.h"
#include "gpio_irq_api.h"
#include "gpio_irq_ex_api.h"
#include "us_ticker_api.h"
#include "wait_api.h"

#if defined(CONFIG_EXAMPLE_MATTER) && CONFIG_EXAMPLE_MATTER

#define GPIO_IRQ_PIN              PA_19
#define STATUS_MEROSS_POWER_GPIO_NUM PA_20
#define STATUS_MEROSS_RED_LED_GPIO_NUM PA_13
#define STATUS_MEROSS_GREEN_LED_GPIO_NUM PA_14

gpio_irq_t gpio_irq;
volatile char irq_rise;

extern void ChipTest(void);

static void example_matter_task_thread(void *pvParameters)
{
    vTaskDelay(5000);

    ChipTest();

    vTaskDelete(NULL);
    return;
}

void gpio_demo_irq_handler(uint32_t id, gpio_irq_event event)
{
#if CONFIG_EXAMPLE_WLAN_FAST_CONNECT
    static unsigned int rise_time;
    static unsigned int fall_time;
    
    if (!irq_rise) {
        fall_time = us_ticker_read();
        // Changed as Rising Edge Trigger
        gpio_irq_set_event(&gpio_irq, IRQ_RISE);
        irq_rise = 1;
    } else {
        rise_time = us_ticker_read();
        // Changed as Falling Edge Trigger
        gpio_irq_set_event(&gpio_irq, IRQ_FALL);
        irq_rise = 0;

        if  ((rise_time - fall_time) > 3000000) {
            int i, j;
            for (i=0; i<15; i++) {
                for (j=0; j<2000000; j++) asm("nop");
                gpio_write(&gpio_meross_red_LED, 0);
                gpio_write(&gpio_meross_green_LED, 0);
	        for (j=0; j<2000000; j++) asm("nop");
	        gpio_write(&gpio_meross_red_LED, 1);
                gpio_write(&gpio_meross_green_LED, 1);
	    }
            Erase_Fastconnect_data();
            printf("Erased Fast Connect data\r\n");
            sys_reset(); // reboot
        }
    }
#endif
}

void init_gpios(void)
{
    // Init IRQ
    gpio_irq_init(&gpio_irq, GPIO_IRQ_PIN, gpio_demo_irq_handler, (uint32_t)(&gpio_irq));
    gpio_irq_set(&gpio_irq, IRQ_FALL, 1);    // Falling Edge Trigger
    irq_rise = 0;
    gpio_irq_pull_ctrl(&gpio_irq, PullNone);
    gpio_irq_enable(&gpio_irq);
    
    // Init gpios    
    gpio_init(&gpio_meross_power, STATUS_MEROSS_POWER_GPIO_NUM);
    gpio_dir(&gpio_meross_power, PIN_OUTPUT);    // Direction: Output
    gpio_mode(&gpio_meross_power, PullNone);     // No pull
    gpio_write(&gpio_meross_power, 0);
    
    gpio_init(&gpio_meross_red_LED, STATUS_MEROSS_RED_LED_GPIO_NUM);
    gpio_dir(&gpio_meross_red_LED, PIN_OUTPUT);    // Direction: Output
    gpio_mode(&gpio_meross_red_LED, PullNone);     // No pull
    gpio_write(&gpio_meross_red_LED, 0); 
    
    gpio_init(&gpio_meross_green_LED, STATUS_MEROSS_GREEN_LED_GPIO_NUM);
    gpio_dir(&gpio_meross_green_LED, PIN_OUTPUT);    // Direction: Output
    gpio_mode(&gpio_meross_green_LED, PullNone);     // No pull
    gpio_write(&gpio_meross_green_LED, 1); 
}

void example_matter_task(void)
{
    init_gpios();
    if(xTaskCreate(example_matter_task_thread, ((const char*)"example_matter_task_thread"), 8192, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
        printf("\n\r%s xTaskCreate(example_matter_task_thread) failed", __FUNCTION__);
}

#endif // #if (CONFIG_EXAMPLE_MATTER)
