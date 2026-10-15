#define DT_DRV_COMPAT our_driver

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(our_driver, CONFIG_SENSOR_LOG_LEVEL);

struct our_driver_config {
	struct gpio_dt_spec led;   /* read-only, from DTS */
};

struct our_driver_data {
	bool led_on;               /* runtime state */
};

/* sample_fetch: turns the LED ON */
static int our_driver_sample_fetch(const struct device *dev,
				   enum sensor_channel chan)
{
	const struct our_driver_config *cfg = dev->config;
	struct our_driver_data *data = dev->data;
	int ret;

	if (chan != SENSOR_CHAN_ALL && chan != SENSOR_CHAN_PROX) {
		return -ENOTSUP;
	}

	ret = gpio_pin_set_dt(&cfg->led, 1);
	if (ret < 0) {
		return ret;
	}
	data->led_on = true;
	return 0;
}

/* channel_get: reports the state, then turns the LED OFF */
static int our_driver_channel_get(const struct device *dev,
				  enum sensor_channel chan,
				  struct sensor_value *val)
{
	const struct our_driver_config *cfg = dev->config;
	struct our_driver_data *data = dev->data;
	int ret;

	if (chan != SENSOR_CHAN_PROX) {
		return -ENOTSUP;
	}

	val->val1 = data->led_on ? 1 : 0;
	val->val2 = 0;

	ret = gpio_pin_set_dt(&cfg->led, 0);
	if (ret < 0) {
		return ret;
	}
	data->led_on = false;
	return 0;
}

static DEVICE_API(sensor, our_driver_api) = {
	.sample_fetch = our_driver_sample_fetch,
	.channel_get = our_driver_channel_get,
};

static int our_driver_init(const struct device *dev)
{
	const struct our_driver_config *cfg = dev->config;

	if (!gpio_is_ready_dt(&cfg->led)) {
		LOG_ERR("LED GPIO not ready");
		return -ENODEV;
	}
	return gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
}

#define OUR_DRIVER_DEFINE(inst)                                              \
	static struct our_driver_data data_##inst;                           \
	static const struct our_driver_config cfg_##inst = {                 \
		.led = GPIO_DT_SPEC_GET(DT_INST_PHANDLE(inst, led), gpios),  \
	};                                                                   \
	DEVICE_DT_INST_DEFINE(inst, our_driver_init, NULL,                   \
			      &data_##inst, &cfg_##inst,                     \
			      POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,      \
			      &our_driver_api)

DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_DEFINE)