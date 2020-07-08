#include "components.h"
#include <stdio.h>

int isInWaterTEST(struct DHTSensor *couple) {
    couple->humidity = 0;
    couple->temp = 0;
    for (int i = 0; i < 256; ++i){
        couple->humidity = i;
        if ((i > 100) == isInWater(couple))
            continue;
        else
            return 1;
    } 
    return 0;
}

int getObstaclesPositionTEST(struct DistanceSensor *triplet) {
    int position = 0;
    for (int i = 0; i < 8; ++i) {
        position = getObstaclesPosition(triplet);
        if (position == i)
            continue;
        else
            return 1;
    }
    return 0;
}
