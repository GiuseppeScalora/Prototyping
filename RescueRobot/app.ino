#include <SimpleDHT.h>
#include <MQ2.h>

//MOTORS
#define F_IN_1 11 //left forward, EN_A  not used
#define F_IN_2 10
#define F_IN_3 9  //right forward, EN_B not used
#define F_IN_4 8
 
#define B_IN_1 14 //left forward, EN_A not used
#define B_IN_2 6
#define B_IN_3 9  //right forward, EN_B not used
#define B_IN_4 10
 
//DISTANCE SENSORS
#define LEFT_TRIG 7
#define LEFT_ECHO 8
 
#define CENTR_TRIG 12
#define CENTR_ECHO 13
 
#define RIGHT_TRIG 4
#define RIGHT_ECHO 11
 
//HUMIDITY SENSORS
#define HMDT_IN 22
 
//MICROPHONE
#define A_MIC_1 A1
#define A_MIC_2 A2
#define A_MIC_3 A3 
#define A_MIC_4 A4
 
 
 
//GAS SENSORS
#define GAS_IN 26
MQ2 mq2(GAS_IN);

 
// DISTANCE SNSR
enum {
   NO_OBSTACLE = 0,                       //000
   RIGHT       = (1 << 0),                //001
   FRONT       = (1 << 1),                //010
   LEFT        = (1 << 2),                //100
   FRONT_RIGHT = (FRONT | RIGHT),         //011
   LEFT_RIGHT  = (LEFT | RIGHT),          //101
   FRONT_LEFT  = (LEFT | FRONT),          //110  
   ALL_SIDES   = (LEFT | FRONT | RIGHT),  //111
};
 
struct DistanceSensorModule {
   unsigned int left;
   unsigned int center;
   unsigned int right;
 
   unsigned int l_trig;
   unsigned int c_trig;
   unsigned int r_trig;
};
DistanceSensorModule distModule;
 
//MOTORS
enum {
   BACK,
   FORWARD,
};
 
// MICROPHONES
enum {
	NORTH = 1,
	SOUTH,
	EAST,
	WEST,
};
 
 
SimpleDHT11 dht11(HMDT_IN);
struct DHTSensorModule {
   byte humidity;
   byte temp;
}
DHTSensorModule htModule;
 
 
void setup() {
   pinMode(F_IN_1, OUTPUT);
   pinMode(F_IN_2, OUTPUT);
   pinMode(F_IN_3, OUTPUT);
   pinMode(F_IN_4, OUTPUT);
   pinMode(B_IN_1, OUTPUT);
   pinMode(B_IN_2, OUTPUT);
   pinMode(B_IN_3, OUTPUT);
   pinMode(B_IN_4, OUTPUT);
 
   pinMode(LEFT_TRIG, OUTPUT);
   pinMode(LEFT_ECHO, INPUT);
   pinMode(CENTR_TRIG, OUTPUT);
   pinMode(CENTR_ECHO, INPUT);
   pinMode(RIGHT_TRIG, OUTPUT);
   pinMode(RIGHT_TRIG, INPUT);
 
   pinMode(HMDT_IN, INPUT);
 
   pinMode(A_MIC_1, INPUT);
   pinMode(A_MIC_2, INPUT);
   pinMode(A_MIC_3, INPUT);
   pinMode(A_MIC_4, INPUT);
 
   pinMode(GAS_IN, INPUT);
 
 
   updateDistSensors(distModule);
   changeTriggerVals(distModule, 25, 20, 25);
}
 
void loop() {
   updateDistSensors(distModule);
   currObstacleState = getObstaclesPosition(distModule);

   updateHumidityAndTemp(htModule);
   int lpg = getLpgLevel();
   int co = getCoLevel();
   int smoke = getSmokeLevel();
   int soundDirection = getSoundDirection();
   //fetch info from camera
   switch (currObstacleState)
   {
        case NO_OBSTACLE:
           followSound();
           break;

        case RIGHT:
            driveFrontMotors(FORWARD, FORWARD, 100, 25);
            driveBackMotors(FORWARD, FORWARD, 100, 25);
            break;

        case LEFT:
            driveFrontMotors(FORWARD, FORWARD, 25, 100);
            driveBackMotors(FORWARD, FORWARD, 25, 100);
            break;

        case FRONT_RIGHT:
            driveFrontMotors(FORWARD, FORWARD, 100, 0);
            driveBackMotors(FORWARD, FORWARD, 100, 0);
            break;

        case FRONT_LEFT:
            driveFrontMotors(FORWARD, FORWARD, 0, 100);
            driveBackMotors(FORWARD, FORWARD, 0, 100);
            break;

        case FRONT:
        case LEFT_RIGHT:
        case ALL_SIDES:
            reverseProcedure();
            break;
    
    
        default:
           break;
   }
}
 
 
void driveFrontMotors(bool lm_dir, bool rm_dir, uint8_t lv, uint8_t rv) {
   uint8_t lvTemp = map(lv, 0, 100, 50, 255);
   uint8_t rvTemp= map(rv, 0, 100, 50, 255);
 
   analogWrite(F_IN_1, lm_dir * lvTemp);
   analogWrite(F_IN_2, (!lm_dir) * lvTemp);
 
   analogWrite(F_IN_3, rm_dir * rvTemp);
   analogWrite(F_IN_4, (!rm_dir) * rvTemp);
}
 
void driveBackMotors(bool lm_dir, bool rm_dir, uint8_t lv, uint8_t rv) {
   uint8_t lvTemp = map(lv, 0, 100, 50, 255);
   uint8_t rvTemp= map(rv, 0, 100, 50, 255);
 
   analogWrite(B_IN_1, lm_dir * lvTemp);
   analogWrite(B_IN_2, (!lm_dir) * lvTemp);
 
   analogWrite(B_IN_3, rm_dir * rvTemp);
   analogWrite(B_IN_4, (!rm_dir) * rvTemp);
}

void reverseProcedure() {
    unsigned long start = millis();
    while ((millis() - start) < 1000) {
        driveFrontMotors(BACK, BACK, 100, 100);
        driveBackMotors(BACK, BACK, 100, 100);
    }

    while ((millis() - start) < 500) {
        driveFrontMotors(BACK, FORWARD, 100, 100);
        driveBackMotors(BACK, FORWARD, 100, 100);
    }
}
 
unsigned int getDistance(uint8_t trigPin, uint8_t echoPin) {
   unsigned long time;
   unsigned int distance;
 
   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
 
   time = pulseIn(echoPin, HIGH);
   distance = time / 58; //0.0343 / 2;
   return distance;
}
 
void changeTriggerVals(struct DistanceSensorModule &triplet,
                        int lt, int ct, int rt)
{
   triplet.l_trig = lt;
   triplet.c_trig = ct;
   triplet.r_trig = rt;
}
 
void updateDistSensors(struct DistanceSensorModule &triplet) {
   triplet.left = getDistance(LEFT_TRIG, LEFT_ECHO);
   triplet.center = getDistance(CENTR_TRIG, CENTR_ECHO);
   triplet.right = getDistance(RIGHT_TRIG, RIGHT_ECHO);
}
 
unsigned int getObstaclesPosition(struct DistanceSensorModule &triplet) {
   unsigned int R, C, L, result;
   L = (triplet.left < triplet.l_trig);
   C = (triplet.center < triplet.c_trig);
   R = (triplet.right < triplet.r_trig);
  
   result = ((L << 2) | (C << 1) | R);
   return result;
}
 
int updateHumidityAndTemp(struct DHTSensorModule &couple) {
   return dht11(&couple.humidity, &couple.temp, NULL)
}
 
bool isInWater(struct DHTSensorModule &couple) {
   retrun couple.humidity > 100 ? true : false;
}
 
int readMic(int micId) {
	switch(micId) {
			case NORTH:
				readValue = analogRead (A1);
				break;
			case SOUTH:
				readValue = analogRead (A2);
				break;
			case EAST:
				readValue = analogRead (A3);
				break;
			case WEST:
				readValue = analogRead (A4);
				break;	
		}
		return readValue;
}

int getSoundDirection() {
	for (int i=0; i<4; i++) {
		if (readMic(i) > 0) {
			return i; // 0 = NORTH, 1 = SOUTH, 2 = EAST, 3 = WEST
      }
	}
	return -1; // when no sound detected;
}


void followSound() {
   switch(soundDirection) {
			case 0: // NORTH
            driveFrontMotors(FORWARD, FORWARD, 100, 100);
            driveBackMotors(FORWARD, FORWARD, 100, 100);
				break;
			case 1: // SOUTH
				reverseProcedure();
				break;
			case 3: // EAST
				driveFrontMotors(FORWARD, FORWARD, 100, 0);
          	driveBackMotors(FORWARD, FORWARD, 100, 0);
				break;
			case 4: // WEST
				driveFrontMotors(FORWARD, FORWARD, 0, 100);
          	driveBackMotors(FORWARD, FORWARD, 0, 100);
				break;
		}
}

/*
		values is an array which contains 3 values.
		* 1 = LPG in ppm
		* 2 = CO in ppm
		* 3 = SMOKE in ppm
*/
int getLpgLevel() {
	float* values= mq2.read(true);
	lpg = values[0];
	return lpg;

int getCoLevel() {
	float* values= mq2.read(true);
	co = values[1];
	return co;
}

int getSmokeLevel() {
	float* values= mq2.read(true);
	smoke = values[2];
	return smoke;
}
