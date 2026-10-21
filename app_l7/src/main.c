#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	const struct device *dev = DEVICE_DT_GET_ANY(our_driver);

	if (!device_is_ready(dev)) {
		LOG_ERR("our_driver not ready");
		return -ENODEV;
	}

	LOG_INF("Ready. Use the shell: sensor info|fetch|read|set <hold_ms>");

	return 0;
}