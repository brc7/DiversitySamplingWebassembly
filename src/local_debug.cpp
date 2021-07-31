#include "RACE.h"
#include "hashes.h"
#include "io.h"
#include <cstring>

#include <fstream>
#include <stdio.h>
#include <iostream>

int hashes[SKETCH_REPS];

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

	std::clog<<"A"<<std::endl;
	unsigned char* inbuf_start = getInputBuffer();
	unsigned char* outbuf_start = getOutputBuffer();

	std::clog<<"B"<<std::endl;
	// 1. Determine how big inbuf is
	int inbuf_valid_bytes = getInputBufferBytes();
	int outbuf_valid_bytes = 0;
	unsigned char* inbuf_end = inbuf_start + inbuf_valid_bytes;

	std::clog<<"C"<<std::endl;
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

int main(int argc, char **argv){
    if (argc < 4){
    	std::cerr<<"tool input.fastq tau k"<<std::endl;
    	return -1;
    }
	std::ifstream infile(argv[1]);
	infile.seekg(0, std::ios::end);
	size_t file_length = infile.tellg();
	infile.seekg(0, std::ios::beg);

	float tau = std::stof(argv[2]);
	int fastwhat = 0;
	int k = std::stoi(argv[3]);

	setup();

	unsigned char* inbuf_ptr = getInputBuffer();
	unsigned char* outbuf_ptr = getOutputBuffer();
	unsigned char* head = inbuf_ptr;

	setInputBufferBytes(0);
	setOutputBufferBytes(0);

	size_t offset = 0;
	size_t chunkSize = INPUT_BUFFER_SIZE_BYTES - getInputBufferBytes();
	size_t chunkdata_length = 0;
	size_t inbuffer_num_valid_bytes = 0;
	size_t outbuffer_num_valid_bytes = 0;


	while(offset < file_length){

		inbuffer_num_valid_bytes = getInputBufferBytes();
		chunkSize = INPUT_BUFFER_SIZE_BYTES - inbuffer_num_valid_bytes;
		std::clog<<offset<<" "<<chunkSize<<" "<<file_length<<" "<<inbuffer_num_valid_bytes<<std::endl;

		
		infile.read( (char*)(inbuf_ptr + inbuffer_num_valid_bytes) , chunkSize);
		chunkdata_length = infile.gcount();
		setInputBufferBytes(inbuffer_num_valid_bytes + chunkdata_length);

		process(tau, fastwhat, k);

		outbuffer_num_valid_bytes = getOutputBufferBytes();
		for (int i = 0; i < outbuffer_num_valid_bytes; i++){
			std::cout<<outbuf_ptr[i];
		}
		offset += chunkSize;
	}
	return 0;
}

