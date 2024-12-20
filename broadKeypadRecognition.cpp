/*
* NEW! Keypad bits can now be read, regardless of the model of reader. 
* Simply attach this function to the weigand decode method.
* Note: The Asterisk and Pound keys are usually decoded as 10 and 11.
*/
int keypadBitToInteger(String inputBits) {
    int decimalValue = 0;
    int length = inputBits.length();

    for (int i = 0; i < length; i++) {
        char bit = inputBits.charAt(i);
        if (bit == '1') {
            decimalValue = decimalValue * 2 + 1;
        } else if (bit == '0') {
            decimalValue = decimalValue * 2;
        } else {
            return -1; //Or whatever error code u want TBD
        }
    }

    return decimalValue;
}
