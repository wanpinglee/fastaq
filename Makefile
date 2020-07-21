
MASTER_DIR=$(shell pwd)
OBJ_DIR=$(MASTER_DIR)/obj
BIN_DIR=$(MASTER_DIR)/bin
LIB=$(MASTER_DIR)/lib

AUTOCONF = autoconf
AUTOHEADER = autoheader

CFLAGS:=
ifeq ($(mode), debug)
	CFLAGS:=$(CFLAGS) -O0 -g -DDEBUG
else
	CFLAGS:=$(CFLAGS) -O3
endif

CXXFLAGS:=-std=c++11 $(CFLAGS)
export $(CXXFLAGS)


SUB_DIRS = src

HTS_LIB:=$(LIB)/htslib/libhts.a
INCLUDE=-I include/
LIBRARY=-lz -lbz2 -lcurl -lpthread -lssl -lcrypto


PROGRAM=$(BIN_DIR)/SeqExtractor

all: $(BIN_DIR)/SeqExtractor $(BIN_DIR)/CountKmer
.PHONY: all

clean:
	@echo "- Clean up"
	$(MAKE) -C $(LIB)/htslib clean
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
.PHONY: clean


$(BIN_DIR)/SeqExtractor: $(HTS_LIB) AUX
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/SeqExtractor examples/SeqExtractor.cpp $(OBJ_DIR)/*.o $(HTS_LIB) $(LIBRARY) $(INCLUDE)

$(BIN_DIR)/CountKmer: $(HTS_LIB) AUX
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/CountKmer examples/CountKmer.cpp $(OBJ_DIR)/*.o $(HTS_LIB) $(LIBRARY) $(INCLUDE)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

AUX: $(OBJ_DIR)
	@for dir in $(SUB_DIRS); do \
		$(MAKE) --no-print-directory --directory=$$dir; \
	done

$(HTS_LIB):
	@echo "- Building in htslib"
	$(MAKE) --no-print-directory -C $(LIB)/htslib
