/*
* BB-1_Remote.ino
*
* Created: 14.09.2018
* Author: Lukas
*
*/

// measured joystick limits
const uint16_t VR_X_MIN = 0;
const uint16_t VR_X_MAX = 1023;
const uint16_t VR_Y_MIN = 0;
const uint16_t VR_Y_MAX = 1023;

// joystick neutral
const uint8_t NEUTRAL = 128;

// measured joystick neutral
const uint16_t VR_X_NEUTRAL = 520;
const uint16_t VR_Y_NEUTRAL = 512;

// joystick neutral offsets
const int32_t VR_X_OFFSET = NEUTRAL - VR_X_NEUTRAL;
const int32_t VR_Y_OFFSET = NEUTRAL - VR_Y_NEUTRAL;

//String message; // string that stores the incoming message

// Arduino pin numbers
#define VR_X_PIN A0 // analog pin connected to joystick VRx
#define VR_Y_PIN A1 // analog pin connected to joystick VRy

// get calibrated, mapped and constrained joystick data
void joystick_update(uint8_t& x, uint8_t& y);

// NOTE! Enabling DEBUG adds about 3.3kB to the flash program size.
// Debug output is now working even on ATMega328P MCUs (e.g. Arduino Uno)
// after moving string constants to flash memory storage using the F()
// compiler macro (Arduino IDE 1.0+ required).
#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

void setup() {
	// initialize serial communication
	Serial.begin(115200);
	while (!Serial); // wait for Leonardo eNUMeration, others continue immediately
	
	// initialize serial1 communication
	Serial1.begin(115200);
	while (!Serial1); // wait for Leonardo eNUMeration, others continue immediately
}

void loop() {
	// calibrated, mapped and constrained joystick data
	static uint8_t x;
	static uint8_t y;
	
	// specifies the requested data from BB-1
	static char dataRequest;
	
	joystick_update(x, y);
	
	// check for BB1 data request
	if (Serial1.available()) {
		// clear serial input buffer if it contains more than one char and use last char in this case
		while (Serial1.available()) {	
			dataRequest = Serial1.read();
		}
		
		switch (dataRequest) {
			case 'j':			// send requested joystick data
				Serial1.print('j'); Serial1.print(x); Serial1.print(','); Serial1.print(y);
				DEBUG_PRINT('j'); DEBUG_PRINT(x); DEBUG_PRINT(','); DEBUG_PRINT(y);
				break;
			case 'c':			// send requested control data
				break;
			default:
				DEBUG_PRINT("Received unknown char.");
		}
	}
	
	/*
	DEBUG_PRINT("X-axis: ");
	DEBUG_PRINT(x);
	DEBUG_PRINT("\n");
	DEBUG_PRINT("Y-axis: ");
	DEBUG_PRINTLN(y);
	DEBUG_PRINT("\n\n");
	delay(500);
	*/
	
	/*
	// while there is data available on serial1
	while(Serial1.available()) {
	message+=char(Serial1.read());	//store string from serial command
	}
	
	if(!Serial1.available()) {
	// if data was received
	if(message!="") {
	// show data
	Serial.println(message);
	// clear data
	message="";
	}
	}
	
	delay(1000);	//delay
	*/

}

// get calibrated, mapped and constrained joystick measurements
void joystick_update(uint8_t& x, uint8_t& y) {
	// joystick measurements
	static uint16_t vr_x;
	static uint16_t vr_y;
	
	// read joystick measurements
	vr_x = analogRead(VR_X_PIN);
	//vr_y = analogRead(VR_Y_PIN);
	
	// calibrate, map and constrain joystick measurements
	if (vr_x <= VR_X_NEUTRAL) {
		x = constrain(map(vr_x + VR_X_OFFSET, VR_X_MIN + VR_X_OFFSET, VR_X_NEUTRAL + VR_X_OFFSET, 0, NEUTRAL), 0, NEUTRAL);
	}
	else {
		x = constrain(map(vr_x + VR_X_OFFSET, VR_X_NEUTRAL + VR_X_OFFSET, VR_X_MAX + VR_X_OFFSET, NEUTRAL, 255), NEUTRAL, 255);
	}

	if (vr_y <= VR_Y_NEUTRAL) {
		y = constrain(map(vr_y + VR_Y_OFFSET, VR_Y_MIN + VR_Y_OFFSET, VR_Y_NEUTRAL + VR_Y_OFFSET, 0, NEUTRAL), 0, NEUTRAL);
	}
	else {
		y = constrain(map(vr_y + VR_Y_OFFSET, VR_Y_NEUTRAL + VR_Y_OFFSET, VR_Y_MAX + VR_Y_OFFSET, NEUTRAL, 255), NEUTRAL, 255);
	}
}

