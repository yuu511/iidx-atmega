Firmware for IIDX controller for atmega32u4 based boards.<br> Written in C99, using LUFA library for USB implementation.

Dependencies:
---
- avr-gcc 
- avr-libc

Build:
---
$ git submodule update --init --recursive\
$ make

Flashing:
---
DFU bootloader
- Reset the board.
- $ sudo make dfu

Arduino bootloader (Arduino Leo)
- $ chmod +x ./leonardo.sh
- $ ./leonardo.sh
- Reset the board with the reset button.


Hardware:
---
- 1x Atmega32u4 development board<br> (Arduino Leonardo will work fine.)

- 7x Rectangle buttons (OBSA-45UK-W-LN)<br> https://item.rakuten.co.jp/sanwadenshi/ilumb_100/

- 7x 5v LEDs<br> https://www.aliexpress.com/item/32923591206.html?spm=a2g0s.9042311.0.0.2d494c4dJ2C9pA

- 2x Small square buttons (OBSF-24KK-S-K) <br> https://item.rakuten.co.jp/sanwadenshi/button_005/

- 1x 600ppr encoder (https://www.amazon.com/gp/product/B00UTIFCVA/ref=ppx_yo_dt_b_asin_title_o02_s01?ie=UTF8&psc=1)


Pinout:
---
Encoder (Turntable)

| Encoder Pin | Atmega Pin |
| ------------- | ------------- |
| DT  (A)   | PB4 |
| CLK (B)   | PB5 |
| VCC       | (connect to 5v-24v source)   |
| GND       | (connect to ground)          |                    

Meta buttons (Start, VEFX)

| Square Button | Atmega Pin |
| ------------- | ------------- |
| Start | PE6|
| VEFX  | PC7|


Gameplay Buttons 1-7<br>

| Rectangle Button   | Atmega Pin 
| ------------- | ------------- 
| Button 1|PD0| 
| Button 2|PD1|
| Button 3|PD2| 
| Button 4|PD3|
| Button 5|PD4|
| Button 6|PD6| 
| Button 7|PD7|
