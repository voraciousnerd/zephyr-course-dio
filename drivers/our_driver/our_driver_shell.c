/*
 * L07 - Shell commands for the L06 LED-based sensor driver ("our_driver")
 *
 * Task 1: sensor fetch | read | info
 * Task 2: sensor set <hold_ms>   (validated, uses SHELL_CMD_ARG)
 *
 */
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>
#include <our_drivers/our_driver.h>   /* our_driver_set_hold_ms() */

/* First enabled node whose compatible is "our,driver" */
#define SENSOR_NODE DT_COMPAT_GET_ANY_STATUS_OKAY(our_driver)

/* The driver only supports this channel */
#define SENSOR_CHAN SENSOR_CHAN_PROX

/* Valid range for hold_ms. channel_get() sleeps for hold_ms on the shell
 * thread, so keep the maximum small. 2000 ms is an assumption - adjust.
 */
#define HOLD_MS_MIN 0
#define HOLD_MS_MAX 2000

static const struct device *const sensor_dev = DEVICE_DT_GET(SENSOR_NODE);

static int check_ready(const struct shell *sh)
{
	if (!device_is_ready(sensor_dev)) {
		shell_error(sh, "Sensor device is not ready");
		return -ENODEV;
	}
	return 0;
}

/* ---------- Task 1 ---------- */

/* fetch: driver turns the LED ON */
static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	if (check_ready(sh)) {
		return -ENODEV;
	}

	int ret = sensor_sample_fetch(sensor_dev);

	if (ret < 0) {
		shell_error(sh, "sensor_sample_fetch failed: %d", ret);
		return ret;
	}

	shell_print(sh, "Sample fetched (LED ON)");
	return 0;
}

/* read: driver reports LED state, waits hold_ms, turns the LED OFF */
static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	struct sensor_value val;

	if (check_ready(sh)) {
		return -ENODEV;
	}

	int ret = sensor_channel_get(sensor_dev, SENSOR_CHAN, &val);

	if (ret < 0) {
		shell_error(sh, "sensor_channel_get failed: %d", ret);
		return ret;
	}

	shell_print(sh, "PROX = %d (%s)", val.val1,
		    val.val1 ? "LED was on" : "LED was off");
	return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(sh, "Device: %s", sensor_dev->name);
	shell_print(sh, "Ready:  %s", device_is_ready(sensor_dev) ? "yes" : "no");
	return 0;
}

/* ---------- Registration ---------- */

SHELL_STATIC_SUBCMD_SET_CREATE(
	sub_sensor,
	SHELL_CMD(fetch, NULL, "Fetch a sample (LED on)", cmd_sensor_fetch),
	SHELL_CMD(read, NULL, "Read channel (LED off after hold)", cmd_sensor_read),
	SHELL_CMD(info, NULL, "Show device name and ready state", cmd_sensor_info),
	/* mandatory = 2 (the word "set" + <hold_ms>), optional = 0 */
	SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "Sensor driver commands", NULL);