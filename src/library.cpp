#include "RACE.h"
#include "hashes.h"
#include "io.h"
#include <cstring>

#include <stdio.h>

int hashes[SKETCH_REPS];


extern "C" {

	void setup(){
		// 0. Set up RACE sketch 
		sketch_clear();
		return;
	}

	void process(float tau, int fastwhat, int k){
		// float tau, int fastwhat, int k
		// int fastwhat = 0;
		// int k = 14;
		// float tau = 10.0;

		unsigned char* inbuf_start = getInputBuffer();
		unsigned char* outbuf_start = getOutputBuffer();

		// 1. Determine how big inbuf is
		int inbuf_valid_bytes = getInputBufferBytes();
		int outbuf_valid_bytes = 0;
		unsigned char* inbuf_end = inbuf_start + inbuf_valid_bytes;

		// 2. Clear outbuf
		setOutputBufferBytes(0);

		// 3. initialize stream parameters
		unsigned char* pos = inbuf_start;
		unsigned char* chunk;
		unsigned char* seq;
		int chunk_len;
		int seq_len;

		int err = SequenceFeatures(fastwhat, pos, inbuf_end, chunk, chunk_len, seq, seq_len);
		while ( err != 1 ){
			// 4. process stuff
			// std::memcpy(outbuf_start + outbuf_valid_bytes, chunk, chunk_len);
			// outbuf_valid_bytes += chunk_len;
			// printf("HI\n");
			if (err == 0){
				sequence_minhash(k, seq, seq_len-1, hashes, SKETCH_REPS);
				float KDE = sketch_quad(hashes);
				if (KDE < tau){
					std::memcpy(outbuf_start + outbuf_valid_bytes, chunk, chunk_len);
					outbuf_valid_bytes += chunk_len;
					// std::memcpy(outbuf_start + outbuf_valid_bytes, seq, seq_len);
					// outbuf_valid_bytes += seq_len;
				}
			}
			err = SequenceFeatures(fastwhat, pos, inbuf_end, chunk, chunk_len, seq, seq_len);
		}

		// 5. now that we have run out of bufferized input
		// copy over any remaining text into the input buffer
		// to be re-used at the next step
		std::memcpy(inbuf_start, chunk, chunk_len);
		setInputBufferBytes(chunk_len);
		setOutputBufferBytes(outbuf_valid_bytes);
		return;
	}
}