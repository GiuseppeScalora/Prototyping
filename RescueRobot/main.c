#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "components.h"

//MQ2 mq2(GAS_IN);
 
struct DistanceSensor distModule;
 
//SimpleDHT11 dht11(HMDT_IN);

struct DHTSensor htModule;

 
void setup() {
   changeTriggerVals(&distModule, 25, 20, 25);
   for (int i = 0; i < 10; ++i)
       getObstaclesPosition(&distModule);
}

int main() {
    setup(); 

    for (;;) {
        int currObstacleState = getObstaclesPosition(&distModule);

        updateHumidityAndTemp(&htModule);
        int lpg = getLpgLevel();
        int co = getCoLevel();
        int smoke = getSmokeLevel();
        int soundDirection = getSoundDirection();
        //fetch info from camera
        switch (currObstacleState) {
            case NO_OBSTACLE:
               followSound(soundDirection);
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
        printf("STATE:    MIC:    TEMP:   HMDT:       FL:      FR:  \n"
               "%d         %d       %d     %d         %d %d     %d %d\n"
               "LPG:      CO:     SMOKE:              BL:      BR:  \n"
               "%u       %u       %u                  %d %d     %d %d\n"
               "                                                    \n",
               STATE, MIC, TEMP, HMDT, FL[0], FL[1], FR[0], FR[1], LPG, CO, SMOKE, BL[0], BL[1], BR[0], BR[1]);

        sleep(1);
    }
    return 0;
}
