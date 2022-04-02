# HID Weigand/Pinpad Reader for Arduino
Arduino Keypad and RFID reading software for HID iClass, PivClass, and MultiClass/SE 
model readers. 

**Modified to contain scaffolding for keypress recognition.**

If the bits in this software's keypress readers cannot be decoded, log the raw
bits to the console and change them accordingly.

Tested on PIVClass Reader with while reading 8 bits from the key press.

## Important:
If the indicator color is green while in standby mode, switch the leads for 'green' and 'red'.
Normally, it should be read during standby, yellow (perodically) for a unrecognized keycard, and green (perodically) for a recognized keycard.

Connection configuration is listed in source code.
