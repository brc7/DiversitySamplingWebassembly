#include "io.h"

extern "C" {

	unsigned char inbuf[INPUT_BUFFER_SIZE_BYTES];
	unsigned char outbuf[OUTPUT_BUFFER_SIZE_BYTES];

	int byte_counts[2];

	unsigned char* getInputBuffer(){
		return &inbuf[0];
	}

	unsigned char* getOutputBuffer(){
		return &outbuf[0];
	}

	void setInputBufferBytes(int bytes){
		byte_counts[0] = bytes;
	}

	int getInputBufferBytes(){
		return byte_counts[0];
	}

	int getInputBufferSize(){
		return INPUT_BUFFER_SIZE_BYTES;
	}

	void setOutputBufferBytes(int bytes){
		byte_counts[1] = bytes;
	}

	int getOutputBufferBytes(){
		return byte_counts[1];
	}

	int getOutputBufferSize(){
		return OUTPUT_BUFFER_SIZE_BYTES;
	}

}




int SequenceFeatures(int fastwhat,
	unsigned char* &pos, unsigned char* end,
	unsigned char* &chunk, int &chunk_len,
	unsigned char* &seq, int &seq_len){

	/* Buffered sequence reader. Reads either a fastq or fasta 
	sequence beginning at position "pos" and ending at position "end"
	Returns pointers to the chunk and the sequence

	Input parameters
	--------------------
	fastwhat: 0 if fastq, 1 if fasta
	pos: current location in the data buffer
	end: end of data buffer

	Output parameters
	--------------------
	chunk and chunk_len: start of chunk and number of bytes in chunk
	seq and seq_len: start of sequence and number of bytes in sequence
	pos: location in the data buffer, after reading data
	
	Return value
	--------------------
	int: error code
		0: we read a whole chunk
		1: we reached the end of the buffer without finishing the chunk
		2: first line of chunk was empty
		3: first line of chunk had incorrect beginning character
		4: sequence was empty
	*/

	// determine type of input
	int chunksize = 2;
	char begin = '>';
	if (fastwhat == 0){ // fastq
		chunksize = 4;
		begin = '@';
	} else { // fasta
		chunksize = 2;
		begin = '>';
	}

	// read full chunk of data
	chunk = pos;
	int linesread = 0;
	chunk_len = 0;
	seq_len = 0;

	// check that first line is valid
	if (pos == end){ return 1; }
	if (*pos == '\n'){
		pos++;
		chunk_len++;
		return 2;
	} else if (*pos != begin){
		pos++;
		chunk_len++;
		return 3;
	}
	// read first line of chunk
	while (*pos != '\n'){
		if (pos == end){ return 1; }
		chunk_len++;
		pos++;
	}
	linesread++;
	pos++;
	chunk_len++;

	// read sequence
	seq = pos;
	if (*pos == '\n'){
		pos++;
		chunk_len++;
		return 4; // sequence is empty
	}
	while (*pos != '\n'){
		if (pos == end){ return 1; }
		chunk_len++; 
		seq_len++;
		pos++;
	}
	linesread++;
	pos++;
	chunk_len++;
	seq_len++;

	while(linesread < chunksize){
		while (*pos != '\n'){
			if (pos == end){ return 1; }
			chunk_len++;
			pos++;
		}
		linesread++;
		pos++;
		chunk_len++;
	}

	return 0;
}
