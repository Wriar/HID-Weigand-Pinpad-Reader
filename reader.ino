
/*
 * HID RFID Reader for HID PIVclass recognition with 
 * scaffolding to read keypad code. Modified to include
 * keypad reading.
 * 
 * Please change the bits to match your model if the below example
 * does not correlate to your reader's keys.
*/
 
 
#define MAX_BITS 100                 
#define WEIGAND_WAIT_TIME  3000      // time to wait for another weigand pulse.  
 
unsigned char databits[MAX_BITS];    // stores all of the data bits
unsigned char bitCount;              // number of bits currently captured
unsigned char flagDone;              // goes low when data is currently being captured
unsigned int weigand_counter;        // countdown until we assume there are no more bits
 
unsigned long facilityCode=0;        
unsigned long cardCode=0;            

int LED_GREEN = 11; //Connect RED to LED_GREEN
int LED_RED = 12; // CONNECT GREEN to LED_RED
int BEEP_BEEP = 10; //Buzzer

// interrupt that happens when INTO goes low (0 bit)
void ISR_INT0() {
  //Serial.print("0");   // uncomment this line to display raw binary
  bitCount++;
  flagDone = 0;
  weigand_counter = WEIGAND_WAIT_TIME;  
 
}
 
// interrupt that happens when INT1 goes low (1 bit)
void ISR_INT1() {
  //Serial.print("1");   // uncomment this line to display raw binary
  databits[bitCount] = 1;
  bitCount++;
  flagDone = 0;
  weigand_counter = WEIGAND_WAIT_TIME;  
}
 
void setup() {
  pinMode(LED_RED, OUTPUT);  
  pinMode(LED_GREEN, OUTPUT);  
  pinMode(BEEP_BEEP, OUTPUT);  
  digitalWrite(LED_RED, HIGH); // High = Off
  digitalWrite(BEEP_BEEP, HIGH); // High = off
  digitalWrite(LED_GREEN, LOW);  // Low = On
  pinMode(2, INPUT);     // DATA0 (INT0)
  pinMode(3, INPUT);     // DATA1 (INT1)
 
  Serial.begin(9600);
  Serial.println("Loaded RFID Reader");
 
  // binds the ISR functions to the falling edge of INTO and INT1
  attachInterrupt(0, ISR_INT0, FALLING);  
  attachInterrupt(1, ISR_INT1, FALLING);
 
 
  weigand_counter = WEIGAND_WAIT_TIME;
}
 
void loop()
{
  // This waits to make sure that there have been no more data pulses before processing data
  if (!flagDone) {
    if (--weigand_counter == 0)
      flagDone = 1;  
  }
 
  // if we have bits and we the weigand counter went out
  if (bitCount > 0 && flagDone) {
    unsigned char i;
 
    Serial.print("Read ");
    Serial.print(bitCount);
    Serial.print(" bits. ");
 
    if (bitCount == 35) {
      // 35 bit HID Corporate 1000 format
      // facility code = bits 2 to 14
      for (i=2; i<14; i++) {
         facilityCode <<=1;
         facilityCode |= databits[i];
      }
 
      // card code = bits 15 to 34
      for (i=14; i<34; i++) {
         cardCode <<=1;
         cardCode |= databits[i];
      }
 
      printBits();
    }
    else if (bitCount == 26) {
      // standard 26 bit format
      // facility code = bits 2 to 9
      for (i=1; i<9; i++) {
         facilityCode <<=1;
         facilityCode |= databits[i];
      }
 
      // card code = bits 10 to 23
      for (i=9; i<25; i++) {
         cardCode <<=1;
         cardCode |= databits[i];
      }
 
      printBits();  
    }
    else if (bitCount == 8) {
      Serial.println("Detected Key Press");
      String dbits;
      
     
      //Concat String
      for (i=1; i<9; i++) {
        dbits += databits[i];
        // Serial.println(databits[i]);
      }
       Serial.println("Registered Key: " + dbits);
  
      
       
       if (dbits == "00111100") {
         // do Thing A
         Serial.println("Registered Key Press: 1");
      }
       else if (dbits == "01011010") {
      Serial.println("Registered Key Press: 2");
      }
       else if (dbits == "01111000") {
       Serial.println("Registered Key Press: 3");
      }
       else if (dbits == "10010110") {
     Serial.println("Registered Key Press: 4");
      }
       else if (dbits == "10110100") {
       Serial.println("Registered Key Press: 5");
      }
       else if (dbits == "11010010") {
       Serial.println("Registered Key Press: 6");
      }
       else if (dbits == "11110000") {
     Serial.println("Registered Key Press: 7");
      }
       else if (dbits == "00001110") {
       Serial.println("Registered Key Press: 8");
      }
       else if (dbits == "00101100") {
       Serial.println("Registered Key Press: 9");
      }
       else if (dbits == "00011110") {
       Serial.println("Registered Key Press: 0");
      }
       else if (dbits == "01001010") {
       Serial.println("Registered Key Press: * ");
      }
       else if (dbits == "01101000") {
      Serial.println("Registered Key Press: # ");
      }
      else {
      Serial.println("unrecognized pinpad press");
    }
    }
    else {

     Serial.println("Unable to decode input. Please reswipe card"); 
    }
 
     // cleanup and get ready for the next card
     bitCount = 0;
     facilityCode = 0;
     cardCode = 0;
     for (i=0; i<MAX_BITS; i++) 
     {
      //RESET DATABITS
       databits[i] = 0;
     }
  }
}
 
void printBits() {
      Serial.print("Facility Code = ");
      Serial.print(facilityCode);
      Serial.print(", Card Code = ");
      Serial.println(cardCode);


      digitalWrite(LED_RED, LOW); // Red

      //Set an accepted card code here.
      if(cardCode == 000000){

        digitalWrite(LED_GREEN, HIGH); 

        //Do whatever, open servo, etc.
      }
      delay(500);
      digitalWrite(LED_RED, HIGH);  // Red Off
      digitalWrite(LED_GREEN, LOW);  // Green back on

      
}
