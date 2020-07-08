#ifndef COMPONENTS_H
#define COMPONENTS_H
// global variable for printing status, do not touch
int STATE;
int MIC;
int TEMP;
int HMDT;
int FL[2];
int FR[2];
int BL[2];
int BR[2];
int LPG;
int CO;
int SMOKE;

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
 
//GAS SENSOR
#define GAS_IN 26

// MOTORS
enum {
   BACK = 0,
   FORWARD = 1,
};

// DISTANCE SNSR AND POSITIONS
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

struct DistanceSensor {
   unsigned char l_trig;
   unsigned char c_trig;
   unsigned char r_trig;
};

enum {
	NORTH = 1,
	SOUTH,
	EAST,
	WEST,
};

// TEMP AND HUMIDITY SENSOR
struct DHTSensor {
   unsigned char humidity;
   unsigned char temp;
};

int map(int input, int in_min, int in_max, int out_min, int out_max);

void driveFrontMotors(int lm_dir, int rm_dir,
                      unsigned char lv, unsigned char rv);

void driveBackMotors(int lm_dir, int rm_dir,
                      unsigned char lv, unsigned char rv);

void reverseProcedure();

unsigned int getDistance(unsigned char trigPin, unsigned char echoPin);

unsigned int GEN_getDistance(int amp, float prd);

unsigned int GEN_putState(int sNum);

void changeTriggerVals(struct DistanceSensor *triplet,
   int lt, int ct, int rt);

unsigned int getObstaclesPosition(struct DistanceSensor *triplet);

unsigned int updateHumidityAndTemp(struct DHTSensor *couple);

int isInWater(struct DHTSensor *couple);

int readMic(int micId);

int getSoundDirection();

void followSound(int soundDirection);

unsigned int getLpgLevel();

unsigned int getCoLevel();

unsigned int getSmokeLevel();


#endif
