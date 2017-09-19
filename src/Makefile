
OBJ_DIR = ../obj
SOURCES = fasta.cpp reference.cpp
OBJECTS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
INCLUDE = -I ../ -I ../lib/htslib/htslib/

all: $(OBJECTS)

$(OBJECTS): $(SOURCES)
	@echo "  * compiling" $(*F).cpp
	@$(CXX) -c $(CXXFLAGS) $(*F).cpp $(INCLUDE) -o $@


.PHONY: all
