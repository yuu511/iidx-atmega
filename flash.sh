#!/bin/bash

echo "Please reset Arduino Leo."
while :
do
  for sysdevpath in $(find /sys/bus/usb/devices/usb*/ -name dev); do
    DONE=0
    syspath="${sysdevpath%/dev}"
    devname="$(udevadm info -q name -p $syspath)"

    if [[ "$devname" == "bus/"* ]]; then
      continue
    fi

    eval "$(udevadm info -q property --export -p $syspath)"

    if [[ -z "$ID_SERIAL" ]]; then
      continue
    fi

    if [[ "$ID_SERIAL" == "Arduino_LLC_Arduino_Leonardo" ]]; then
      avrdude -pm32u4 -cavr109 -F -D -P/dev/$devname -Uflash:w:iidx.hex 
      DONE=1
    fi

    if [[ $DONE -eq 1 ]]; then 
      exit
    fi
  done
done
