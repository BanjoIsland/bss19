/*
  This dummy console just randomly passes/fails any action reported.
  For better randomness, make sure A0 (seed source) is floating.
*/

const uint8_t CONSOLE_ID = 0x00;
enum States {IDLING, TRYHARD_A, TRYHARD_B, TRYHARD_C, SUCCESS};

bool debug_mode = false
;

void run_mode();
void read_serial();

volatile enum States state = IDLING;

void setup() {
  Serial.begin(9600);
  randomSeed(A0);
}

void loop() {
  read_serial();
  delay(10);
}

// Checks the serial port for a command from the main terminal
void read_serial() {
  uint8_t incoming_byte = 0xAA; //Unused value to prevent false flags
  if (Serial.available() > 0) {
    if (debug_mode) {
      incoming_byte = Serial.parseInt();
    } else {
      incoming_byte = Serial.read();
    }
  }
  switch (incoming_byte)
  {
    if (incoming_byte == (byte) 0x00) {        // State request; return 0x00:idling, 0x01:TRYING HARD
      if (state == IDLING) {
        if (debug_mode) Serial.println("I'm not trying");
        Serial.write((byte)0x01);       //I'm idling
      } else {
        if (debug_mode) Serial.println("I'm trying");
        Serial.write((byte)0x00);       //I'm trying hard
      }
    } else if (incoming_byte == (byte) 0x01) {        //Identify; return console ID
      Serial.write((byte)CONSOLE_ID);
      if (debug_mode) Serial.println("You requested my ID");
    } else if (incoming_byte == (byte) 0x09) {         //Reset
      state = IDLING;             //TODO: Think about possible consequences of this
    } else if (incoming_byte == (byte) 0xAA) {
    } else {
      long rn = random(10000);
      if (rn % 2 == 0) {   // CONTROL PROBABILITY WITH MODULUS
      }
        delay (1000);
        Serial.write((byte) 0xFF);
      }
  }
}
