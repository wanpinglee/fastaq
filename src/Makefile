
OBJ_DIR = ../obj
SOURCES = fasta.cpp reference.cpp countKmer.cpp
OBJECTS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
INCLUDE = -I ../include/ -I ../lib/htslib/

CXXFLAGS = -std=c++11

all: $(OBJECTS)

$(OBJECTS): $(SOURCES)
	@echo "  * compiling" $(*F).cpp
	@$(CXX) -c $(CXXFLAGS) $(*F).cpp $(INCLUDE) -o $@ -L../lib/htslib/ -lhts


.PHONY: all
