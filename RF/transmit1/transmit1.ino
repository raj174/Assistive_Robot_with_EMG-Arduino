#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(3,4); // CE, CSN


//const byte address[6] = "00001";
const int SensorValue = A0;
const int threshold = 800;
const uint64_t address[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL};
int Value, previousValue;
bool power = true;
bool counter = true;
volatile byte count;
unsigned long previousMillis = 0; 
const long interval = 10000;
void setup()
{
  radio.begin();
  Serial.begin(9600);
  radio.setRetries(15,10);
  radio.openWritingPipe(address[1]);
  radio.openReadingPipe(0, address[3]);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop()
{
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
    }
  }

  if(counter == false && previousValue <400){
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
  
  if (power == true) //if the rf module has powered up and if the sensor vlaue if above the threshold then send it to the receiver
  { 
    //Serial.println(Value);
    //radio.write(&Value, sizeof(Value));
    if(Value>threshold)
    {
      Serial.println("sending");
      previousMillis = currentMillis;
      radio.write(&Value, sizeof(Value));
      delay(400);
    }
  }
    
}
