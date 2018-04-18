#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
byte pipeNum = 0;
//const byte address[6]= "00001";
const uint64_t address[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL};
unsigned long previousMillis = 0;
int prevpipeNum = 0;
const int interval = 200;
void setup() 
{
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address[0]);
  radio.openWritingPipe(address[1]);
  radio.openReadingPipe(1, address[1]);
  radio.openWritingPipe(address[0]);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  unsigned long currentMillis = millis();
  if (radio.available(&pipeNum)) 
  {
    int text;
    radio.read(&text, sizeof(text));
    if (pipeNum == 0)
    {
      Serial.print("next");
      prevpipeNum = pipeNum;
      previousMillis = currentMillis;
    }
    else if (pipeNum == 1)
    {
      Serial.println("select");
      prevpipeNum = pipeNum;
      previousMillis = currentMillis;
    }
    else
    {
      //pass
    }
    if (currentMillis - previousMillis > interval)
    {
      if (pipeNum != prevpipeNum)
      {
        Serial.println("back");
      }
    }
    Serial.print(pipeNum);
    Serial.println(text);
  }

}
