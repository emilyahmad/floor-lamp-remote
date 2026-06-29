Check protocol ELEGOO remote uses: NEC infrared
[repo](https://github.com/Arduino-IRremote/Arduino-IRremote)

tried reciever dump & demo sketches

trying elegoo starter kit code & library

grabbed [code from this guy](https://www.youtube.com/watch?v=q97VE3oEwIc), spammed off button to get hex (0xBA45FF00) and made a case for it - will put relay functionality in there

going to wire up the relay but it's not like any components I've used before (no wires sticking out)

you just unscrew the top and insert the wire inside and rescrew it

wired 5V & GND on arduino to +/- rails on breadboard,
wired IC receiver and relay to +/- rails & IN to digital pin d7

types of contacts:
no = normally open, open in standard state, closed when activated
n.c = normally closed

when HIGH signal applied, NO & COM pins close; open when lOW

IR remote starter/slightly modified code, saving so I can experiment with relay code which'll probably end up in off case body

```
/*
    IR Remote

    Print out the received IR value. Turn on the red LED if received a specific code.
*/

// remove this line if using Arduino IDE
#include <Arduino.h>

#include <IRremote.hpp>

const int IR_RECEIVE_PIN = 2;
const int redPin = 10;

void setup()
{
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
  pinMode(redPin, OUTPUT);
}

void loop(){
  if (IrReceiver.decode())
  {
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    IrReceiver.printIRResultShort(&Serial);

    switch(IrReceiver.decodedIRData.decodedRawData)
    {
      case 0xBA45FF00: // off btn
        Serial.println("Off");
        digitalWrite(redPin, HIGH);
        delay(500);
        digitalWrite(redPin, LOW);
        break;
      default:
        break;
    }

    IrReceiver.resume(); // Enable receiving of the next value
  }
}
```

may have to be careful of maximum switching voltage/current (will it be able to handle lamp's) it'll probably be fine because its literally just lighting up a 120V light bulb

looks like its max load is AC 250V/10A, my lightbulb is 120V so should be fine

probably should model with an LED, so I'll wire up the relay to a regular LED then connect the on/off switch from the IR to turn on/off the LED

COM -> + rail
NC -> 220 ohm resistor (-> red LED)
NO -> 220 ohm resistor (-> green LED)

actually IN -> GND

ohh the LEDs should be connected to each other, nvm that dude wasn't using the relay at all and was just it as a wire basically.. LMFAO

```
const int relayPin = 7;

void setup() {
  pinMode(relayPin, OUTPUT);
}

void loop() {
  digitalWrite(relayPin, HIGH);
  delay(2000);
  digitalWrite(relayPin, LOW);
  delay(2000);
}
```

awesome, can control relay from code by wiring IN to a digital pin & using digitalWrite HIGH to turn on/off an LED, lets add the code to the off button switch in the IR code from earlier

```
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
        } else {
          Serial.println("Turning off..");
          digitalWrite(redPin, LOW);
          digitalWrite(relayPin, LOW);
        }
        break;
      default:
        break;
    }

    IrReceiver.resume(); // Enable receiving of the next value
  }
}
```

Now I just need to understand the floor lamp's wiring, disconnect it from its little board and put the appropriate wires into my relay. I'd be wiring the NO & NC relay terminals. I'll look up how the wiring for most floor lamps with sliders works and maybe the product description for the specific lamp. This is what it looks like

| Front                                         | Back                                              |
| --------------------------------------------- | ------------------------------------------------- |
| ![Old lamp's board](/Images/oldLampBoard.JPG) | ![Old lamp's board](/Images/oldLampBoardBack.JPG) |

Played with the images to try to understand the circuit better, talked to chatgpt about typical floor lamp circuits, ones with dimmers, I just don't want to die from electrocution.

Might have a dimmer circuit (outlet -> hot wire -> dimmer slider, neutral wire completes circuit & returns to outlet), I'll change it to just use a switch like most lamps. Might need to check if its an incandescent bulb/halogen lamp & uses a TRIAC dimmer. Based on images, chat thinks its a mains-powered TRIAC phase-angle dimmer my lamp's an incandescent and dimmable-halogen based on based on the TRIAC (Q2 on aluminum frame), slider potentiometer (VR1) + .. looks good. I'll make a plan for how I'll wire the lamp to the relay then sleep.

Traced, found the input wire, I guess that's what I'll put in the NO, output will go in NC and COM will be neutral is my initial thought.

Oh COM, NO and NC don't correspond to hot, neutral and output, we just need to dis/connect to the hot wire. NC is unused, COM is incoming hot and NO is hot going to the lamp. Hm but the lamp doesn't exactly have a neutral wire. Ohh is it just the input cord to COM and output to NO? I'll try tomorrow but check with someone first so I don't get electrocuted.

| ![1](/Images/PCB_Traced/1.png) | ![1](/Images/PCB_Traced/2.png) | ![1](/Images/PCB_Traced/3.png) | ![1](/Images/PCB_Traced/4.png) |
| ------------------------------ | ------------------------------ | ------------------------------ | ------------------------------ |
