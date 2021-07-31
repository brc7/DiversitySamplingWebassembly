#pragma once

#define SKETCH_RANGE 1000000
#define SKETCH_REPS 10

void sketch_add(int *hashes);
float sketch_quad(int *hashes);
void sketch_clear();
