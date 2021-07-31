# How to use 
# To make all binaries: make binaries

CXX = /Users/sharkbait/Documents/code/tools/emsdk/upstream/emscripten/emcc
CFLAGS = -O3 -std=c++11 -gsource-map --source-map-base http://localhost:8000/ #-fopenmp

FN_EXPORTS = "['_setup','_process','_getOutputBufferBytes','_getInputBufferBytes','_getInputBuffer','_getOutputBuffer','_setInputBufferBytes','_setOutputBufferBytes','_getOutputBufferSize','_getInputBufferSize']"
MEMORY_BUDGET=150MB

SRCS = RACE.cpp hashes.cpp io.cpp
SRCS_DIR = src/

BUILD_DIR = build/
BIN_DIR = bin/
INC := -I include

# List of target executables
TARGETS = library.cpp
TARGETS_DIR = src/

# Everything beyond this point is determined from previous declarations, don't modify
OBJECTS = $(addprefix $(BUILD_DIR), $(SRCS:.cpp=.o))
BINARIES = $(addprefix $(BIN_DIR), $(TARGETS:.cpp=))

$(BUILD_DIR)%.o: $(SRCS_DIR)%.cpp | $(BUILD_DIR:/=)
	$(CXX) $(INC) -c $(CFLAGS) $< -o $@

binaries: $(BINARIES)
targets: $(BINARIES)
all: $(BINARIES)

$(BUILD_DIR:/=):
	mkdir -p $@
$(BIN_DIR:/=): 
	mkdir -p $@

$(BINARIES): $(addprefix $(TARGETS_DIR), $(TARGETS)) $(OBJECTS) | $(BIN_DIR:/=)
	$(CXX) $(INC) $(CFLAGS) $(OBJECTS) --no-entry -s TOTAL_MEMORY=$(MEMORY_BUDGET) -s EXPORTED_FUNCTIONS=$(FN_EXPORTS) $(addsuffix .cpp,$(@:$(BIN_DIR)%=$(TARGETS_DIR)%)) -o $(addsuffix .wasm,$@) 

clean:
	rm -f $(OBJECTS); 
	rm -f $(BINARIES); 

.PHONY: clean targets binaries all



