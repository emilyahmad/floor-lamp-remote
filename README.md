### Floor Lamp Remote

#### I have a lamp with a dimmer switch you have to slide on/off, but I wanted to be able to control it from a distance, so I desoldered the lamp's input and output wires and connected them to a relay, then wired it and an IR sensor to my Arduino UNO. I wrote some code to switch the relay on/off with input from the IR remote.

![Final diagram](/Images/Final.png)

You can see a demo [here](https://youtu.be/SGTEAsR3ht0)

BOM

| Item                    | Purpose            | Quantity | Cost | Source         |
| ----------------------- | ------------------ | -------- | ---- | -------------- |
| Arduino UNO             | Microcontroller    | 1        | ?    | Starter kit    |
| Relay/switch            | Switch on/off lamp | 1        | $6   | Amazon, 4 pack |
| TRIAC/Dimmer switch PCB | Powers lamp        | 1        | ?    | Came with lamp |
| Jumper/dupoint wires    | Wires              | ~7       | ?    | Starter kit    |
