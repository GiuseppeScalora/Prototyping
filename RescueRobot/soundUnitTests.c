void soundFilteringTEST(int microphoneId) {
	printf("Starting soundFilteringTEST function...\n\n");
	int returnedValue = soundFiltering(microphoneId);
	printf("\nReturned value by filter (1 = signal passed, 0 = signal ignored): %d\n", returnedValue);
	if(returnedValue == 1) {
		printf("Sinal passed.\n");
	} else {
		printf("Sinal ignored.\n");
	}
}

void followSoundTEST() {
	printf("Starting followSoundTEST function...\n\n");
	
	while(soundFiltering(NORTH)==1) {
		printf("\nFollowing sound coming from NORTH direction\n\n");
		followSound(NORTH);
		printf("Repeating fetching NORTH microphone\n\n");
	}
	printf("\nNo more accepted signal from NORTH detected.\n\n");
	
	while(soundFiltering(SOUTH)==1) {
		printf("\nFollowing sound coming from SOUTH direction\n\n");
		followSound(SOUTH);
		printf("Repeating fetching SOUTH microphone\n\n");
	}
	printf("\nNo more accepted signal from SOUTH detected.\n\n");
	
	while(soundFiltering(EAST)==1) {
		printf("\nFollowing sound coming from EAST direction\n\n");
		followSound(EAST);
		printf("Repeating fetching EAST microphone\n\n");
	}
	printf("\nNo more accepted signal from EAST detected.\n\n");
	
	while(soundFiltering(WEST)==1) {
		printf("\nFollowing sound coming from WEST direction\n\n");
		followSound(WEST);
		printf("Repeating fetching WEST microphone\n\n");
	}
	printf("\nNo more accepted signal from WEST detected.\n\n");
	
	printf("Stopping followSoundTEST function...\n\n");
}


void followSoundTEST2() {
	while(soundFiltering(NORTH) == 1) {
		followSound(NORTH);
	}

	while(soundFiltering(SOUTH) == 1) {
		followSound(SOUTH);
	}
	
	while(soundFiltering(EAST) == 1) {
		followSound(EAST);
	}
	
	while(soundFiltering(WEST) == 1) {
		followSound(WEST);
	}
}