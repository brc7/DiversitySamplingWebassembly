#include "hashes.h"
#include <limits>

// MurmurHash2, by Austin Appleby
unsigned int MurmurHash (const void * key, int len, unsigned int seed)
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value

	unsigned int h = seed ^ len;

	// Mix 4 bytes at a time into the hash

	const unsigned char * data = (const unsigned char *)key;

	while(len >= 4)
	{
		unsigned int k = *(unsigned int *)data;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h *= m; 
		h ^= k;

		data += 4;
		len -= 4;
	}
	
	// Handle the last few bytes of the input array

	switch(len)
	{
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
	        h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
} 


void sequence_minhash(int k, unsigned char* sequence, int len_sequence, int* hashes, int len_hashes){

	// note: this is needed to fix the bug where sequence_minhash will try to hash beyond the 
	// end of the array, if len_sequence < k
	if (len_sequence < k){
		k = len_sequence;
	}

	for (int n = 0; n < len_hashes; n++){

		unsigned int hashed_value; 
		unsigned int minhashed_value; 

		minhashed_value = std::numeric_limits<unsigned int>::max();
		hashes[n] = 0;

		// for each kmer in the sequence
		for (int start = 0; (start + k + 1) < len_sequence; start++){
			hashed_value = MurmurHash(sequence + start, sizeof(unsigned char)*k, n);

			if (hashed_value < minhashed_value){
				minhashed_value = hashed_value;
				// proxy for returning the string itself
				// might not be necessary - could probably just use minhashed_value instead?
				hashes[n] = MurmurHash(sequence + start, sizeof(unsigned char)*k, n*3);
			}
		}
	}
	return;
}
