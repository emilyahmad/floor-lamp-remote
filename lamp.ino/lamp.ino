#include <IRremote.hpp>

const int IR_RECEIVE_PIN = 2;
const int redPin = 10;
const int relayPin = 7;

bool turnedOff = true;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
  pinMode(redPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
}

void loop(){
  if (IrReceiver.decode())
  {
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    IrReceiver.printIRResultShort(&Serial);

    switch(IrReceiver.decodedIRData.decodedRawData)
    {
      case 0xBA45FF00: // off btn
        if (turnedOff == true) {
          Serial.println("Turning on..");
          digitalWrite(redPin, HIGH);
          digitalWrite(relayPin, HIGH);
          turnedOff = false;
        } else {
          Serial.println("Turning off..");
          digitalWrite(redPin, LOW);
          digitalWrite(relayPin, LOW);
          turnedOff = true;
        }
        break;
      default:
        break;
    }

    IrReceiver.resume(); // Enable receiving of the next value
  }
}
