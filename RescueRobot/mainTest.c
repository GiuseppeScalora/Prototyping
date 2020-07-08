#include <stdio.h>
#include "components.h"
#include "unitTests.h"


struct DHTSensor htSensor;
struct DistanceSensor distSensor;

int main() {
    printf("isInWaterTEST: %s\n", isInWaterTEST(&htSensor) ? "FAILED" : "PASSED");
    printf("getObstaclesPositionTEST: %s\n", getObstaclesPositionTEST(&distSensor) ? "FAILED" : "PASSED");
    return 0;
}
