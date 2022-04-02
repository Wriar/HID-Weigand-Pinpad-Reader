# HID Weigand/Pinpad Reader for Arduino
Arduino Keypad and RFID reading software for HID iClass, PivClass, and MultiClass/SE 
model readers. 

**Modified to contain scaffolding for keypress recognition.**

If the bits in this software's keypress readers cannot be decoded, log the raw
bits to the console and change them accordingly.

Tested on PIVClass Reader with while reading 8 bits from the key press.

## Important:
1. Scan your card. Will turn yellow and back red if card is not accepted. Will turn steady yellow if card is accepted.
2. Enter password on keypad and press # to accept password. (e.x 2580#). Press * to clear password (you will have to reswipe keycard after clearing passcode).
3. Keypad Led will turn green if passcode is accepted. Red if it is not accepted. You will have to reswipe your card if you fail a password input.
