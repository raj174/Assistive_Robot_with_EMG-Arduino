#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(3,4); // CE, CSN


//const byte address[6] = "00001";
const int SensorValue = A0;
int threshold = 700;
const uint64_t address[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL};
int Value, previousValue;
bool power = true;
bool counter = true;
int prevvalue =0;
int countertwo=0;
volatile byte count;
unsigned long previousMillis = 0; 
const long interval = 30000;

void setup()
{
  radio.begin();
  Serial.begin(9600);
  radio.setRetries(15,10);
  radio.openWritingPipe(address[0]);
  //radio.openReadingPipe(0, address[3]);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop(){
  unsigned long currentMillis = millis();
  Value = analogRead(SensorValue);
  
  //Serial.println(Value);
  if (currentMillis - previousMillis >= interval) //makes the rf module to sleep if the sensor value hasnt got any signal for a given amount of time
  {
    previousMillis = currentMillis;
    if (power == true)
    {
      Serial.print("stop");
      radio.powerDown();
      power = false;
      count = 0;
      countertwo =0;
    }
  }

  if(counter == false && previousValue <200){
    counter = true;
  }
  previousValue = Value;
  if(Value>threshold && count < 5 && power == false && counter == true) // To power up and start sending signals to receiver, need to clench fist five times 
  {
      count++;
      Serial.println("counting");
      //delay(1000);
      counter = false;

      
  }
  else if (count >= 5 && power == false) // if user clenched fist five times power up the rf module and restart the timer
  {
    Serial.println("turnon");
    radio.powerUp();
    power = true;
    previousMillis = currentMillis;
  }
  /*
  if (Value > threshold - 200 && Value < threshold && power ==false && counter == true)
  {
    Serial.println("counting2");
    Serial.println("turnon_new");
    countertwo++;
    prevvalue = Value + prevvalue;
    counter == false;

  }
  if (countertwo >= 5 && power == false){
    threshold = prevvalue/countertwo;
    prevvalue = 0;
    countertwo = 0;
    previousMillis = currentMillis;
    radio.powerUp();
    power = true;
  }
  */
  if (power == true) //if the rf module has powered up and if the sensor vlaue if above the threshold then send it to the receiver
  { 
    //Serial.println(Value);
    //radio.write(&Value, sizeof(Value));
    if(Value>threshold)
    {
      Serial.println("sending");
      previousMillis = currentMillis;
      radio.write(&Value, sizeof(Value));
      delay(1000);
    }
  }
    
}
