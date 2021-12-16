#include "FreeRTOS.h"
#include "task.h"
#include <platform/platform_stdlib.h>
#include "basic_types.h"
#include "platform_opts.h"
#include "section_config.h"
#include "gpio_api.h"
#include "gpio_irq_api.h"

#if defined(CONFIG_EXAMPLE_MATTER) && CONFIG_EXAMPLE_MATTER

#define GPIO_IRQ_PIN        PA_19

gpio_irq_t gpio_btn;

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
    Erase_Fastconnect_data();
    printf("Erased Fast Connect data\r\n");
#endif

}

void init_irq(void)
{
    gpio_irq_init(&gpio_btn, GPIO_IRQ_PIN, gpio_demo_irq_handler, NULL);
    gpio_irq_set(&gpio_btn, IRQ_FALL, 1);    // Falling Edge Trigger
    gpio_irq_enable(&gpio_btn);
}

void example_matter_task(void)
{
    init_irq();
    if(xTaskCreate(example_matter_task_thread, ((const char*)"example_matter_task_thread"), 8192, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
        printf("\n\r%s xTaskCreate(example_matter_task_thread) failed", __FUNCTION__);
}

#endif // #if (CONFIG_EXAMPLE_MATTER)
