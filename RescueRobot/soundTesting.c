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
