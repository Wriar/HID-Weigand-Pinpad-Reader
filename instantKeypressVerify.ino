/*
* HID RED PIN: D12
* HID GREEN PIN: D11
* BEEP PIN: D10

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
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
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

  digitalWrite(6, HIGH);
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


       String passwordKey = "12345";
       
       if (dbits == "11000010") {
         // do Thing A
         Serial.println("Registered Key Press: 1");
         keySequence += "1";

          if(keySequence == passwordKey && acceptCard == true) {
        //Code is correct
        Serial.println("### User Authenticated");
        digitalWrite(5, HIGH);
        keySequence = "";
        digitalWrite(BEEP_BEEP, LOW);
        //Make Color Green
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
      digitalWrite(6, LOW);

        
        delay(1000);
        digitalWrite(BEEP_BEEP, HIGH);
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        //Make Color Red
        digitalWrite(LED_GREEN, HIGH);  // Green Off
        digitalWrite(LED_RED, LOW);  // Red Back On
        
        acceptCard = false;
      }
      }
       else if (dbits == "10100100") {
      Serial.println("Registered Key Press: 2");
      keySequence += "2";

       if(keySequence == passwordKey && acceptCard == true) {
        //Code is correct
        Serial.println("### User Authenticated");
        digitalWrite(5, HIGH);
        keySequence = "";
        digitalWrite(BEEP_BEEP, LOW);
        //Make Color Green
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
      digitalWrite(6, LOW);

        
        delay(1000);
        digitalWrite(BEEP_BEEP, HIGH);
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        //Make Color Red
        digitalWrite(LED_GREEN, HIGH);  // Green Off
        digitalWrite(LED_RED, LOW);  // Red Back On
        
        acceptCard = false;
      }
      
      }
       else if (dbits == "10000110") {
       Serial.println("Registered Key Press: 3");
       keySequence += "3";

        if(keySequence == passwordKey && acceptCard == true) {
        //Code is correct
        Serial.println("### User Authenticated");
        digitalWrite(5, HIGH);
        keySequence = "";
        digitalWrite(BEEP_BEEP, LOW);
        //Make Color Green
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
      digitalWrite(6, LOW);

        
        delay(1000);
        digitalWrite(BEEP_BEEP, HIGH);
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        //Make Color Red
        digitalWrite(LED_GREEN, HIGH);  // Green Off
        digitalWrite(LED_RED, LOW);  // Red Back On
        
        acceptCard = false;
      }
      
      }
       else if (dbits == "01101000") {
     Serial.println("Registered Key Press: 4");
     keySequence += "4";

      if(keySequence == passwordKey && acceptCard == true) {
        //Code is correct
        Serial.println("### User Authenticated");
        digitalWrite(5, HIGH);
        keySequence = "";
        digitalWrite(BEEP_BEEP, LOW);
        //Make Color Green
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
      digitalWrite(6, LOW);

        
        delay(1000);
        digitalWrite(BEEP_BEEP, HIGH);
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        //Make Color Red
        digitalWrite(LED_GREEN, HIGH);  // Green Off
        digitalWrite(LED_RED, LOW);  // Red Back On
        
        acceptCard = false;
      }
      
      }
       else if (dbits == "01001010") {
       Serial.println("Registered Key Press: 5");
       keySequence += "5";

        if(keySequence == passwordKey && acceptCard == true) {
        //Code is correct
        Serial.println("### User Authenticated");
        digitalWrite(5, HIGH);
        keySequence = "";
        digitalWrite(BEEP_BEEP, LOW);
        //Make Color Green
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
      digitalWrite(6, LOW);

        
        delay(1000);
        digitalWrite(BEEP_BEEP, HIGH);
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        //Make Color Red
        digitalWrite(LED_GREEN, HIGH);  // Green Off
        digitalWrite(LED_RED, LOW);  // Red Back On
        
        acceptCard = false;
      }
      }
       else if (dbits == "00101100") {
       Serial.println("Registered Key Press: 6");
       keySequence += "6";

        if(keySequence == passwordKey && acceptCard == true) {
        //Code is correct
        Serial.println("### User Authenticated");
        digitalWrite(5, HIGH);
        keySequence = "";
        digitalWrite(BEEP_BEEP, LOW);
        //Make Color Green
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
      digitalWrite(6, LOW);

        
        delay(1000);
        digitalWrite(BEEP_BEEP, HIGH);
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        //Make Color Red
        digitalWrite(LED_GREEN, HIGH);  // Green Off
        digitalWrite(LED_RED, LOW);  // Red Back On
        
        acceptCard = false;
      }
      }
       else if (dbits == "00001110") {
     Serial.println("Registered Key Press: 7");
     keySequence += "7";
      }
       else if (dbits == "11110000") {
       Serial.println("Registered Key Press: 8");
       keySequence += "8";

        if(keySequence == passwordKey && acceptCard == true) {
        //Code is correct
        Serial.println("### User Authenticated");
        digitalWrite(5, HIGH);
        keySequence = "";
        digitalWrite(BEEP_BEEP, LOW);
        //Make Color Green
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
      digitalWrite(6, LOW);

        
        delay(1000);
        digitalWrite(BEEP_BEEP, HIGH);
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        //Make Color Red
        digitalWrite(LED_GREEN, HIGH);  // Green Off
        digitalWrite(LED_RED, LOW);  // Red Back On
        
        acceptCard = false;
      }
      }
       else if (dbits == "11010010") {
       Serial.println("Registered Key Press: 9");
       keySequence += "9";

        if(keySequence == passwordKey && acceptCard == true) {
        //Code is correct
        Serial.println("### User Authenticated");
        digitalWrite(5, HIGH);
        keySequence = "";
        digitalWrite(BEEP_BEEP, LOW);
        //Make Color Green
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
      digitalWrite(6, LOW);

        
        delay(1000);
        digitalWrite(BEEP_BEEP, HIGH);
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        //Make Color Red
        digitalWrite(LED_GREEN, HIGH);  // Green Off
        digitalWrite(LED_RED, LOW);  // Red Back On
        
        acceptCard = false;
      }
      }
       else if (dbits == "11100000") {
       Serial.println("Registered Key Press: 0");
       keySequence += "0";

        if(keySequence == passwordKey && acceptCard == true) {
        //Code is correct
        Serial.println("### User Authenticated");
        digitalWrite(5, HIGH);
        keySequence = "";
        digitalWrite(BEEP_BEEP, LOW);
        //Make Color Green
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
      digitalWrite(6, LOW);

        
        delay(1000);
        digitalWrite(BEEP_BEEP, HIGH);
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        //Make Color Red
        digitalWrite(LED_GREEN, HIGH);  // Green Off
        digitalWrite(LED_RED, LOW);  // Red Back On
        
        acceptCard = false;
      }
      }
       else if (dbits == "10110100") {
       Serial.println("Registered Key Press: * ");
       acceptCard = false;
       keySequence = "";
        digitalWrite(LED_GREEN, HIGH);  // Green Off
        digitalWrite(LED_RED, LOW);  // Red Back On

        
       
      }
       else if (dbits == "10010110") {
      Serial.println("Registered Key Press: # ");
      keySequence += "#";

    
       if(keySequence == passwordKey && acceptCard == true) {
        //Code is correct
        Serial.println("### User Authenticated");
        digitalWrite(5, HIGH);
        keySequence = "";
        digitalWrite(BEEP_BEEP, LOW);
        //Make Color Green
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
      digitalWrite(6, LOW);

        
        delay(1000);
        digitalWrite(BEEP_BEEP, HIGH);
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        //Make Color Red
        digitalWrite(LED_GREEN, HIGH);  // Green Off
        digitalWrite(LED_RED, LOW);  // Red Back On
        
        acceptCard = false;
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
      Serial.print("FC = ");
      Serial.print(facilityCode);
      Serial.print(", CC = ");
      Serial.println(cardCode);

      // Now lets play with some LED's for fun:
      digitalWrite(LED_GREEN, LOW); // Red
      
      if(cardCode == 00000){
        // If this one "bad" card, turn off green
        // so it's just red. Otherwise you get orange-ish
        //digitalWrite(LED_GREEN, HIGH); 

        // digitalWrite(LED_GREEN, LOW); // Red
         
        acceptCard = true;
        Serial.println("Card Accepted");

        //Will stay yellow until pin is entered or reset (*) Key is pushed.
      } else {
        acceptCard = false;
        Serial.println("Card not accepted. Reset Vars");
        delay(500);
        digitalWrite(LED_GREEN, HIGH);  // Green Off
      digitalWrite(LED_RED, LOW);  // Red Back On
      }
     // delay(5500);
      //Yellow to Red
    //  digitalWrite(LED_GREEN, HIGH);  // Green Off
      //digitalWrite(LED_RED, LOW);  // Red Back On
 
      // Lets be annoying and beep more
      
}
