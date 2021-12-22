#ifndef EXAMPLE_MATTER_H
#define EXAMPLE_MATTER_H
#include "gpio_api.h"

void example_matter_task(void);

#ifdef __cplusplus
extern "C"
{
#endif
    gpio_t gpio_meross_power;
    gpio_t gpio_meross_red_LED;
    gpio_t gpio_meross_green_LED;
#ifdef __cplusplus
}
#endif

#endif /* EXAMPLE_MATTER_H */
