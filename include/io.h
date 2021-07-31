#pragma once

#define INPUT_BUFFER_SIZE_BYTES 5000000
#define OUTPUT_BUFFER_SIZE_BYTES 5000000

extern "C" {

	unsigned char* getInputBuffer();
	unsigned char* getOutputBuffer();
	void setInputBufferBytes(int bytes);
	int getInputBufferBytes();
	int getInputBufferSize();
	void setOutputBufferBytes(int bytes);
	int getOutputBufferBytes();
	int getOutputBufferSize();

}

int SequenceFeatures(int fastwhat,
	unsigned char* &pos, unsigned char* end,
	unsigned char* &chunk, int &chunk_len,
	unsigned char* &seq, int &seq_len);
