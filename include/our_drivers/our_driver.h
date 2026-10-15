#ifndef OUR_DRIVERS_OUR_DRIVER_H_
#define OUR_DRIVERS_OUR_DRIVER_H_

#include <zephyr/device.h>
#include <stdint.h>

/* Extension API: how long (ms) the LED stays on before channel_get turns it off */
int our_driver_set_hold_ms(const struct device *dev, uint32_t hold_ms);

#endif