#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const uint64_t address[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL};
const int SensorValue = A0;
int Value;

void setup() {
  radio.begin();
  radio.openWritingPipe(address[1]);
  radio.openReadingPipe(0, address[0]);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
void loop() {
  Value = analogRead(SensorValue);
  Serial.println(Value);
  if(Value>900)
  {
  radio.write(&Value, sizeof(Value));
  }
}
