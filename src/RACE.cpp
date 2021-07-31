#include "RACE.h"
#include <string.h>
// #include <iostream>

int sketch[SKETCH_RANGE*SKETCH_REPS];

void sketch_add(int *hashes){
	for (size_t r = 0; r < SKETCH_REPS; r++){
		size_t index = hashes[r] % SKETCH_RANGE; 
		sketch[r*SKETCH_RANGE + index] = sketch[r*SKETCH_RANGE + index] + 1; 
	}
}

float sketch_quad(int *hashes){
	/* query and add to sketch
	Input: A set of R integer hash values
	one hash value for each ACE repetition
	Performs: Update to RACE sketch
	Returns: query value of RACE sketch
	*/
	float mean = 0; 
	for (int r = 0; r < SKETCH_REPS; r++){
		// unsigned int index = (unsigned int)(hashes[r]) % SKETCH_RANGE;
		unsigned int index = hashes[r];
		index = index % SKETCH_RANGE;
		// std::clog<<index<<" "<<SKETCH_RANGE<<" "<<r*SKETCH_RANGE + index<<std::endl;
		mean = mean + sketch[r*SKETCH_RANGE + index];
		sketch[r*SKETCH_RANGE + index] = sketch[r*SKETCH_RANGE + index] + 1;
	}
	mean = mean / SKETCH_REPS; 
	return mean;
}

void sketch_clear(){
	memset(sketch, 0, SKETCH_REPS*SKETCH_RANGE*sizeof(int));
}


