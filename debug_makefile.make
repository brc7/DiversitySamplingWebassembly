# How to use 
# To make all binaries: make binaries

CXX = /usr/local/Cellar/gcc/10.2.0/bin/g++-10
CFLAGS = -O3 -std=c++11 #-fopenmp

SRCS = RACE.cpp hashes.cpp io.cpp
SRCS_DIR = src/

BUILD_DIR = build/
BIN_DIR = bin/
INC := -I include

# List of target executables
TARGETS = local_debug.cpp
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
	$(CXX) $(INC) $(CFLAGS) $(OBJECTS) $(addsuffix .cpp,$(@:$(BIN_DIR)%=$(TARGETS_DIR)%)) -o $@

clean:
	rm -f $(OBJECTS); 
	rm -f $(BINARIES); 

.PHONY: clean targets binaries all



