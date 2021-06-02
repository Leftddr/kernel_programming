#! /bin/bash

cd /sys/bus/usb/drivers/usbhid
echo -n "1-1.2:1.0" > /sys/bus/usb/drivers/usbhid/unbind
echo 0x046d 0xc05b > /sys/bus/usb/drivers/myusbms/new_id
