# this repo is intent to store develop ESP32-WROOM. 
the setup-esp-idf.sh and setup-zephyr-dev-env.sh is for ubuntu 

## micro-python for ESP32
https://micropython.org/download/ESP32_GENERIC/ 
download latest micro python binary

from power shell
install esptool.py 
```bash
pip install esptool
```
add path to PATH. there is some issue I have to use
```bash 
python -m esptool --chip esp32 --port COM5 erase_flash
```
rather than
```bash 
esptool --chip esp32 --port COM5 erase_flash
```
flash micro-python binary
```bash
python -m esptool --chip esp32 --port COM5 write_flash -z 0x1000 firmware.bin
```
connect COM5 to a terminal(teraterm 115200), press reset button, you will see logs   
or just
```bash
python -m esptool --chip esp32 --port COM5 flash_id
```
## write application
main.py
```bash
#blink buildin LED for micro-python
from machine import Pin
import time

led = Pin(2, Pin.OUT)  # Assuming the LED is connected to GPIO2

while True:
    led.value(not led.value())  # Toggle LED
    time.sleep(0.5)  # Wait for half a second
```
 install ampy (Adafruit MicroPython Tool)
 ```bash
 pip install adafruit-ampy
```
```bash
ampy --port COM5 ls
```
you will see /boot.py
```bash
ampy --port COM5 put main.py
```
```bash
ampy --port COM5 ls
```
you will see /boot.py and /main.py   
Press reset button on esp board. LED is blinking
