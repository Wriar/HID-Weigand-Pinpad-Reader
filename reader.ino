
/*

You may need to change the bits for keypresses for your model.

* HID RED PIN: D12
* HID GREEN PIN: D11
* BEEP PIN: D10
D0: Digital2
D1: Digital3
VCC: 5v
GND: GND

1. Scan a valid card. If card is valid, LED will be steady yellow. If card is invalid, LED will be yellow for a sec then
turn red again.

2. Enter password (default 2580). Press the # Key to accept password. Press the * Key to reset password or entire sequence.

3. Reader will turn GREEN and LONG BEEP and automatically reset.

*/
 
 
#define MAX_BITS 100                 // max number of bits 
#define WEIGAND_WAIT_TIME  3000      // time to wait for another weigand pulse.  
 
unsigned char databits[MAX_BITS];    // stores all of the data bits
unsigned char bitCount;              // number of bits currently captured
unsigned char flagDone;              // goes low when data is currently being captured
unsigned int weigand_counter;        // countdown until we assume there are no more bits
 
unsigned long facilityCode=0;        // decoded facility code
unsigned long cardCode=0;            // decoded card code

int LED_GREEN = 11;
int LED_RED = 12;
int BEEP_BEEP = 10;

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
  Serial.println("RFID Readers");
 
  // binds the ISR functions to the falling edge of INTO and INT1
  attachInterrupt(0, ISR_INT0, FALLING);  
  attachInterrupt(1, ISR_INT1, FALLING);
 
 
  weigand_counter = WEIGAND_WAIT_TIME;

  //Set Red
  digitalWrite(LED_GREEN, HIGH);  // Green Off
  digitalWrite(LED_RED, LOW);  // Red Back On
}

String keySequence;
bool acceptCard;
bool acceptKey;


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
  
       //Prepare switch statement
       enum PossibleBits { UNDEF, RED, ORANGE, YELLOW, GREEN,  BLUE, PURPLE };
       
       if (dbits == "00111100") {
         // do Thing A
         Serial.println("Registered Key Press: 1");
         keySequence += "1";
      }
       else if (dbits == "01011010") {
      Serial.println("Registered Key Press: 2");
      keySequence += "2";
      }
       else if (dbits == "01111000") {
       Serial.println("Registered Key Press: 3");
       keySequence += "3";
      }
       else if (dbits == "10010110") {
     Serial.println("Registered Key Press: 4");
     keySequence += "4";
      }
       else if (dbits == "10110100") {
       Serial.println("Registered Key Press: 5");
       keySequence += "5";
      }
       else if (dbits == "11010010") {
       Serial.println("Registered Key Press: 6");
       keySequence += "6";
      }
       else if (dbits == "11110000") {
     Serial.println("Registered Key Press: 7");
     keySequence += "7";
      }
       else if (dbits == "00001110") {
       Serial.println("Registered Key Press: 8");
       keySequence += "8";
      }
       else if (dbits == "00101100") {
       Serial.println("Registered Key Press: 9");
       keySequence += "9";
      }
       else if (dbits == "00011110") {
       Serial.println("Registered Key Press: 0");
       keySequence += "0";
      }
       else if (dbits == "01001010") {
       Serial.println("Registered Key Press: * ");
       acceptCard = false;
       keySequence = "";
        digitalWrite(LED_GREEN, HIGH);  // Green Off
        digitalWrite(LED_RED, LOW);  // Red Back On
       
      }
       else if (dbits == "01101000") {
      Serial.println("Registered Key Press: # ");
      Serial.println("Key Sequence Entered:" + keySequence);

      if(keySequence == "2580" && acceptCard == true) {
        //Code is correct
        Serial.println("### User Authenticated");
        keySequence = "";
        digitalWrite(BEEP_BEEP, LOW);
        //Make Color Green
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);

        
        delay(1000);
        digitalWrite(BEEP_BEEP, HIGH);
        //Make Color Red
        digitalWrite(LED_GREEN, HIGH);  // Green Off
        digitalWrite(LED_RED, LOW);  // Red Back On
        
        acceptCard = false;
      } else {
        keySequence = "";
        Serial.println("Authentication Key Sequence Failed. Resetting");
        acceptCard = false;
         digitalWrite(LED_GREEN, HIGH);  // Green Off
        digitalWrite(LED_RED, LOW);  // Red Back On
      }
      
      }
      else {
      Serial.println("unrecognized pinpad press");
    }
    }
    else {
     // you can add other formats if you want!
     Serial.println("Unable to decode."); 
    }
 
     // cleanup and get ready for the next card
     bitCount = 0;
     facilityCode = 0;
     cardCode = 0;
     for (i=0; i<MAX_BITS; i++) 
     {
       databits[i] = 0;
     }
  }
}
 
void printBits() {
      Serial.print("FacCc = ");
      Serial.print(facilityCode);
      Serial.print(", CardCc = ");
      Serial.println(cardCode);


      digitalWrite(LED_GREEN, LOW); // Red
      
 //Set your accepted card code here
      if(cardCode == 00000){

         
        acceptCard = true;
        Serial.println("Card Accepted");

      } else {
        acceptCard = false;
        Serial.println("Card not accepted. Reset Vars");
        delay(500);
        digitalWrite(LED_GREEN, HIGH);  // Green Off
      digitalWrite(LED_RED, LOW);  // Red Back On
      }

      
}
