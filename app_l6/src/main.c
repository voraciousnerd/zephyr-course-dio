#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <our_drivers/our_driver.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	const struct device *dev = DEVICE_DT_GET_ANY(our_driver);
	struct sensor_value val;

	if (!device_is_ready(dev)) {
		LOG_ERR("our_driver not ready");
		return -ENODEV;
	}

	uint32_t hold = 2000; 

	while (1) {
		our_driver_set_hold_ms(dev, hold);
		sensor_sample_fetch(dev);                        /* LED on  */
		sensor_channel_get(dev, SENSOR_CHAN_PROX, &val); /* waits hold ms, LED off */
		LOG_INF("hold=%u ms, state=%d", hold, val.val1);
		k_msleep(1000);
		hold = (hold == 100) ? 800 : 100; /* toggle hold time */
	}
	return 0;
}