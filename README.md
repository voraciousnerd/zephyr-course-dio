# Zephyr Training Environment

Welcome to the Zephyr RTOS training! This repository includes a ready-to-use
development environment based on Zephyr 4.3.0, which you can set up in one of
three ways:

---

## Manual Zephyr Setup

Follow the following guide:
- [Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#).

Make sure to select appropriate OS and to perform all steps till
[Build the Blinky Sample](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#build-the-blinky-sample).

---

## Hardware validation

The Blinky application was built and flashed on a Renesas EK-RA6B1 evaluation board.

The required board support is currently available through an internal Renesas Zephyr BSP and is not included in this repository.

---

## Flash to R7KA6B1BG using JLink
### In Terminal 
```bash
# Enter workspace to have access to west etc. 
cd  ~/zephyrproject
# Build target from the directory you want. 
west build -b ek_ra6b1 ~/zephyr-course-dio/app 
# Copy .elf file to Windows FS, to be later accessed by Jlink
cp /home/turing/zephyrproject/build/zephyr/zephyr.elf /mnt/c/Users/a5165721/
# Edit configuration (Kconfig) using UI
west build -t menuconfig
# Move on to Jlink.exe ...
```
### In JLink
```bash
# ... Open JLink.exe
# Initial configuration
>   connect
>   R7KA6B1BG
>   S (SWD)
>   4000 kHz
# Load the .elf file
>   loadfile C:\Users\a5165721\zephyr.elf
# Reset
>   r
# Execute code
>   g
```
---
### Creating an Application Level Overlay
* First, we create ```app.overlay``` where we add alias ```app-led``` pointing to my board's led0, which has the label ```user_led```
```
/ {
	aliases {
		app-led = &user_led;
	};
};
```

* We edit the Kconfig file with: ```int APP_HEARTBEAT_PERIOD_MS``` (default 500, range 100–2000). Then in C, we use ```DT_ALIAS(app_led)``` for the GPIO and ```CONFIG_APP_HEARTBEAT_PERIOD_MS``` for the sleep duration. 
```
config APP_HEARTBEAT_PERIOD_MS
    int "Heartbeat Period (ms)"
    range 100 2000
    default 500
    help
      Delay in milliseconds between LED toggles.

source "Kconfig.zephyr"
```

* Following the J-Link procedure above, we flash and verify by changing the period via menuconfig and observing the LED blink speed. It works!