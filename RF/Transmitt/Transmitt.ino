
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN


//const byte address[6] = "00001";
const int SensorValue = A0;
const uint64_t address[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL};
int Value;
void setup()
{
  radio.begin();
  radio.openWritingPipe(address[0]);
  radio.openReadingPipe(0, address[1]);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop()
{
  Value = analogRead(SensorValue);
  radio.write(&Value, sizeof(Value));
  delay(1000);
}
