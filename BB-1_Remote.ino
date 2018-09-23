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

// measured joystick neutral
const uint16_t VR_X_NEUTRAL = 512;
const uint16_t VR_Y_NEUTRAL = 512;

// Arduino pin numbers
#define VR_X_PIN A0 // analog pin connected to joystick VRx
#define VR_Y_PIN A1 // analog pin connected to joystick VRy

// Orders for communication
#define HELLO_BB1 'B'
#define HELLO_BB1_REMOTE 'R'
#define VELOCITY 'V'
#define CONTROL 'C'

// get calibrated, mapped and constrained joystick data
void joystick_update(int8_t& x, int8_t& y);

// NOTE! Enabling DEBUG adds about 3.3kB to the flash program size.
// Debug output is now working even on ATMega328P MCUs (e.g. Arduino Uno)
// after moving string constants to flash memory storage using the F()
// compiler macro (Arduino IDE 1.0+ required).
//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

void setup() {
	// initialize serial communication
	#ifdef DEBUG
	Serial.begin(115200);
	while (!Serial); // wait for Leonardo eNUMeration, others continue immediately
	#endif
	
	// initialize serial1 communication
	Serial1.begin(115200);
	while (!Serial1); // wait for Leonardo eNUMeration, others continue immediately
}

void loop() {
	// calibrated, mapped and constrained joystick data
	static int8_t x;
	static int8_t y;
	
	// Order received from BB-1
	static char receivedOrder;
	
	// get calibrated, mapped and constrained joystick measurements
	joystick_update(x, y);
	
	// check for BB1 data request
	if (Serial1.available() > 0) {
		receivedOrder = Serial1.read();
		
		switch (receivedOrder) {
			case HELLO_BB1_REMOTE:
				// send confirmation for established communication
				Serial1.print(HELLO_BB1);
				DEBUG_PRINT(HELLO_BB1);
				break;
			case VELOCITY:
				// send requested joystick data
				Serial1.print('<'); Serial1.print(VELOCITY); Serial1.print(','); Serial1.print(x); Serial1.print(','); Serial1.print(y); Serial1.print('>');
				DEBUG_PRINT('<'); DEBUG_PRINT(VELOCITY); DEBUG_PRINT(','); DEBUG_PRINT(x); DEBUG_PRINT(','); DEBUG_PRINT(y); DEBUG_PRINTLN('>');
				break;
			case CONTROL:
				// send requested control data
				break;
			default:
				DEBUG_PRINT("Received unknown order: ");
				DEBUG_PRINTLN(receivedOrder);
		}
	}
}

// get calibrated, mapped and constrained joystick measurements
void joystick_update(int8_t& x, int8_t& y) {
	// joystick measurements
	static uint16_t vr_x;
	static uint16_t vr_y;
	
	// read joystick values
	vr_x = analogRead(VR_X_PIN);
	vr_y = analogRead(VR_Y_PIN);
	
	// calibrate, map and constrain joystick values
	if (vr_x <= VR_X_NEUTRAL) {
		x = constrain(map(vr_x, VR_X_MIN, VR_X_NEUTRAL, -128, 0), -128, 0);
	}
	else {
		x = constrain(map(vr_x, VR_X_NEUTRAL, VR_X_MAX, 0, 127), 0, 127);
	}

	if (vr_y <= VR_Y_NEUTRAL) {
		y = constrain(map(vr_y, VR_Y_MIN, VR_Y_NEUTRAL, -128, 0), -128, 0);
	}
	else {
		y = constrain(map(vr_y, VR_Y_NEUTRAL, VR_Y_MAX, 0, 127), 0, 127);
	}
}

