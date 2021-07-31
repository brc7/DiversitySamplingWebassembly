#pragma once

unsigned int MurmurHash(const void * key, int len, unsigned int seed);
void sequence_minhash(int k, unsigned char* sequence, int sequence_len, int* hashes, int number_of_hashes);

