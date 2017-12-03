
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
	@$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/SeqExtractor examples/SeqExtractor.cpp $(OBJ_DIR)/*.o $(HTS_LIB) -lz $(INCLUDE)

$(BIN_DIR)/CountKmer: $(HTS_LIB) AUX
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/CountKmer examples/CountKmer.cpp $(OBJ_DIR)/*.o $(HTS_LIB) -lz $(INCLUDE)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

AUX: $(OBJ_DIR)
	@for dir in $(SUB_DIRS); do \
		$(MAKE) --no-print-directory --directory=$$dir; \
	done

$(HTS_LIB):
	@echo "- Building in htslib"
	@rm -f $(LIB)/htslib/configure
	@rm -rf $(LIB)/htslib/autom4te.cache
	@cp $(LIB)/htslib/configure.ac $(LIB)/htslib/configure.ac~
	@sed -i 's#m4_esyscmd_s(\[make print-version\]),##g' $(LIB)/htslib/configure.ac
	@cd $(LIB)/htslib && $(AUTOHEADER) && $(AUTOCONF) && ./configure --disable-lzma
	$(MAKE) --no-print-directory -C $(LIB)/htslib
	@test -f $(LIB)/htslib/configure.ac~ && mv $(LIB)/htslib/configure.ac~ $(LIB)/htslib/configure.ac
