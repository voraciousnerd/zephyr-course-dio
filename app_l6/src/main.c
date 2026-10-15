#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	const struct device *dev = DEVICE_DT_GET_ANY(our_driver);
	struct sensor_value val;

	if (!device_is_ready(dev)) {
		LOG_ERR("our_driver not ready");
		return -ENODEV;
	}

	while (1) {
		sensor_sample_fetch(dev);                        /* LED on  */
		k_msleep(500);
		sensor_channel_get(dev, SENSOR_CHAN_PROX, &val); /* LED off */
		LOG_INF("state when read = %d", val.val1);
		k_msleep(500);
        LOG_INF("state when read = %d", val.val1);
	}
	return 0;
}