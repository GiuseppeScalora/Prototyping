#include "components.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//global variable for testing purposes; do not touch
int STATE_COUNTER[3] = {0,0,0};

int map(int input, int in_min, int in_max, int out_min, int out_max) {
    return (input - in_min) * (out_max - out_min ) / (in_max - in_min) + out_min;
}

void driveFrontMotors(int lm_dir, int rm_dir,
                      unsigned char lv, unsigned char rv)
{
    unsigned char lvTemp = map(lv, 0, 100, 50, 255);
    unsigned char rvTemp= map(rv, 0, 100, 50, 255);

    //analogWrite(F_IN_1, lm_dir * lvTemp);
    //analogWrite(F_IN_2, (!lm_dir) * lvTemp);

    //analogWrite(F_IN_3, rm_dir * rvTemp);
    //analogWrite(F_IN_4, (!rm_dir) * rvTemp);
}

void driveBackMotors(int lm_dir, int rm_dir,
                      unsigned char lv, unsigned char rv)
{
    unsigned char lvTemp = map(lv, 0, 100, 50, 255);
    unsigned char rvTemp= map(rv, 0, 100, 50, 255);

    //analogWrite(B_IN_1, lm_dir * lvTemp);
    //analogWrite(B_IN_2, (!lm_dir) * lvTemp);

    //analogWrite(B_IN_3, rm_dir * rvTemp);
    //analogWrite(B_IN_4, (!rm_dir) 1;
}

void reverseProcedure() {
    unsigned long start = 0; //= millis();
    while ((1000 - start) < 1000) {
        driveFrontMotors(BACK, BACK, 100, 100);
        driveBackMotors(BACK, BACK, 100, 100);
    }

    while ((1000 - start) < 500) {
        driveFrontMotors(BACK, FORWARD, 100, 100);
        driveBackMotors(BACK, FORWARD, 100, 100);
    }
}

unsigned int getDistance(unsigned char trigPin, unsigned char echoPin) {
   unsigned long time;
   unsigned int distance;
 
   //digitalWrite(trigPin, LOW);
   //delayMicroseconds(2);
   //digitalWrite(trigPin, HIGH);
   //delayMicroseconds(10);
   //digitalWrite(trigPin, LOW);
 
   //time = pulseIn(echoPin, HIGH);
   //distance = time / 58; //0.0343 / 2;
   return 1;
}

unsigned int GEN_putState(int sNum) {
    int result;
    int seqArr[8][3] = {{0,0,0},
                        {0,0,1},
                        {0,1,0},
                        {0,1,1},
                        {1,0,0},
                        {1,0,1},
                        {1,1,0},
                        {1,1,1},
    };

    result = seqArr[STATE_COUNTER[sNum]++][sNum];
    if (STATE_COUNTER[sNum] > 7)
        STATE_COUNTER[sNum] = 0;
    return result;
}

void changeTriggerVals(struct DistanceSensor *triplet,
   int lt, int ct, int rt)
{
   triplet->l_trig = lt;
   triplet->c_trig = ct;
   triplet->r_trig = rt;
}

unsigned int getObstaclesPosition(struct DistanceSensor *triplet) {
    int bufSumL = 0;
    int bufSumC = 0;
    int bufSumR = 0;
    unsigned int R, C, L, result;

    for (int i = 0; i < 10; ++i) {
        bufSumL += getDistance(LEFT_TRIG, LEFT_ECHO) < triplet->l_trig;
        bufSumC += getDistance(CENTR_TRIG, CENTR_ECHO) < triplet->c_trig;
        bufSumR += getDistance(RIGHT_TRIG, RIGHT_ECHO) < triplet->r_trig;
    }

  //L = bufSumL > 5 ? 1 : 0;
  //C = bufSumC > 5 ? 1 : 0;
  //R = bufSumR > 5 ? 1 : 0;
    
    //comment out if not testing
    L = GEN_putState(0);
    C = GEN_putState(1);
    R = GEN_putState(2);

    result = ((L << 2) | (C << 1) | R);
    return result;
}

unsigned int updateHumidityAndTemp(struct DHTSensor *couple) {
    //return dht11.read(&couple.temp, &couple.humidity, NULL);
    return 1;
}

int isInWater(struct DHTSensor *couple) {
    return couple->humidity > 100 ? 1 : 0;
}

int readMic(int microphoneId) { // enum parameter: [1 = NORTH, 2 = SOUTH, 3 = EAST, 4 = WEST]
    int readValue = rand()%256;
    switch(microphoneId) {
        case NORTH:
        	printf("Generated random value (MICROPHONE NORTH): %d\n", readValue);
            //readValue = analogRead (A1);
            break;
        case SOUTH:
        	printf("Generated random value (MICROPHONE SOUTH): %d\n", readValue);
            //readValue = analogRead (A2);
            break;
        case EAST:
        	printf("Generated random value (MICROPHONE EAST): %d\n", readValue);
            //readValue = analogRead (A3);
            break;
        case WEST:
        	printf("Generated random value (MICROPHONE WEST): %d\n", readValue);
            //readValue = analogRead (A4);
            break;	
        }
        return readValue;
}

int getSoundDirection() {
	for (int i=1; i<=4; i++) {
		if (soundFiltering(i) > 0) {
			return i; // enum parameter: [1 = NORTH, 2 = SOUTH, 3 = EAST, 4 = WEST]
      }
	}
	return -1; // when no sound detected;
}

int soundFiltering(int microphoneId) { 
	int bufSum = 0;
	for (int i = 0; i < 10; ++i) {
		if(readMic(microphoneId) > 127) {
			bufSum++;
		}
	}
	if(bufSum > 5) {
		return 1;
	} else {
		return 0;
	}
}

void followSound(int soundDirection) {
    switch(soundDirection) {
            case NORTH:
                driveFrontMotors(FORWARD, FORWARD, 100, 100);
                driveBackMotors(FORWARD, FORWARD, 100, 100);
                break;
            case SOUTH:
                reverseProcedure();
                break;
            case EAST:
                driveFrontMotors(FORWARD, FORWARD, 100, 0);
                driveBackMotors(FORWARD, FORWARD, 100, 0);
                break;
            case WEST:
                driveFrontMotors(FORWARD, FORWARD, 0, 100);
                driveBackMotors(FORWARD, FORWARD, 0, 100);
                break;
            default:
                reverseProcedure();
        }
}



int getLpgLevel() {
	//float* values= mq2.read(true);
	float lpg = 1;
	return lpg;
}

int getCoLevel() {
	//float* values= mq2.read(true);
	float co = 1;
	return co;
}

int getSmokeLevel() {
	//float* values= mq2.read(true);
	float smoke = 1;
	return smoke;
}
