
OBJ_DIR = ../obj
SOURCES = fasta.cpp reference.cpp
OBJECTS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
INCLUDE = -I ../include/ -I ../lib/htslib/

all: $(OBJECTS)

$(OBJECTS): $(SOURCES)
	@echo "  * compiling" $(*F).cpp
	@$(CXX) -c $(CXXFLAGS) $(*F).cpp $(INCLUDE) -o $@ -L../lib/htslib/ -lhts


.PHONY: all
