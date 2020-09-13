Firmware for atmega32u4 based boards implementing a IIDX controller.<br> 
Written in C99, using LUFA library for USB implemenation.<br><br>
When plugged in, device will function as a composite USB Keyboard + Mouse.<br>
- Button presses will output a key, refer to the table below for button mapping to keys.
- Turning the encoder clockwise will move the mouse cursor down, counter clockwise up.


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
- $ ./leonardo.sh
- Reset the board with the reset button.


Hardware:
---
- 1x Atmega32u4 development board<br> (Arduino Leonardo will work fine)

- 7x Rectangle buttons (OBSA-45UK-W-LN)<br> https://item.rakuten.co.jp/sanwadenshi/ilumb_100/

- 7x 5v LEDs<br> https://www.aliexpress.com/item/32923591206.html?spm=a2g0s.9042311.0.0.2d494c4dJ2C9pA

- 2x Small square buttons (OBSF-24KK-S-K) <br> https://item.rakuten.co.jp/sanwadenshi/button_005/

- 1x 600ppr encoder <br> https://www.amazon.com/gp/product/B00UTIFCVA/ref=ppx_yo_dt_b_asin_title_o02_s01?ie=UTF8&psc=1


Pinout:
---
Encoder (Turntable)

| Encoder Pin | Atmega Pin 
| ------------- | ------------- 
| DT  (A)   | PB4 | 
| CLK (B)   | PB5 | 
| VCC       | (connect to 5v-24v source)   |
| GND       | (connect to ground)          |                    

Meta buttons (Start, VEFX)

| Square Button | Atmega Pin | Keycode Output
| ------------- | ------------- | ------------- |
| Start | PE6 | N 
| VEFX  | PC7 | Z


Gameplay Buttons 1-7 with LEDs <br>

| Rectangle Button   | Atmega Pin (Input)  | Atmega Pin (LED) | Keycode Output
| ------------- | ------------- | ------------- | ------------- |
| Button 1 | PD0 | PF0 | S
| Button 2 | PD1 | PF1 | Y
| Button 3 | PD2 | PF4 | A
| Button 4 | PD3 | PF5 | R
| Button 5 | PD4 | PF6 | O 
| Button 6 | PD6 | PB6 | C
| Button 7 | PD7 | PB7 | H
