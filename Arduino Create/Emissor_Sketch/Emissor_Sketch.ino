#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const byte x = A0;
const byte y = A1;
const byte z = A2;

RF24 radio(8, 10);
const byte address[6] = "00001";

struct data
{
    // The max value of this variables is more than 255 (Max value of "byte")
    int x;
    int y;
    int z;
};

data read_data;

void setup()
{
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.setDataRate(RF24_250KBPS);
    radio.stopListening();
}

void loop()
{
    read_data.x = analogRead(x);
    read_data.y = analogRead(y);
    read_data.z = analogRead(z);
    radio.write(&read_data, sizeof(data));
}