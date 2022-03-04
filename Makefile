.PHONY : clean all

# Directories
BIN_DIR 	:= ./bin
SRC_DIR 	:= ./src
LIB_DIR 	:= ./lib
INC_DIR 	:= ./include

# Check ROOT version
ROOTVER     := $(shell root-config --version | head -c1)
ifeq ($(ROOTVER),5)
	ROOTDICT	:= rootcint
	DICTEXT		:= .h
else
	ROOTDICT	:= rootcling
	DICTEXT		:= _rdict.pcm
endif

# Shared switch
SHAREDSWITCH = -shared -Wl,-soname,# NO ENDING SPACE

# Specify flags for ROOT stuff
ROOTCFLAGS	:= $(shell root-config --cflags)#	# Specifies flags for compiler
ROOTLDFLAGS	:= $(shell root-config --ldflags)#	#  Specifies flags for linker
ROOTLIBS	:= $(shell root-config --glibs) -lRHTTP -lThread#	# Specifies flags for graphics
LIBS        := $(ROOTLIBS)

# Specify compiler
CC          = $(shell root-config --cxx)#		# Decides which compiler to use

# Flags for compiler
CFLAGS		= -c -Wall -Wextra $(ROOTCFLAGS) -g -fPIC#	# Debugging and compile-only
INCLUDES	+= -I$(INC_DIR) -I.#						# Determines which files to look at

# Linker
LD          = $(shell root-config --ld)#		# Decides which linker to use

# Flags for linker
LDFLAGS 	+= $(ROOTLDFLAGS)#

# Objects
OBJECTS = 	$(SRC_DIR)/AlphaSpectrumSimulatorGlobals.o \
			$(SRC_DIR)/AlphaNuclide.o \
			$(SRC_DIR)/AlphaSpectrumSimulator.o \
			$(SRC_DIR)/AlphaSource.o \
			$(SRC_DIR)/ProgressBar.o

# Dependencies
DEPENDENCIES = 	$(INC_DIR)/AlphaDecay.hh \
				$(INC_DIR)/AlphaNuclide.hh \
				$(INC_DIR)/AlphaSpectrumSimulator.hh \
				$(INC_DIR)/AlphaSource.hh \
				$(INC_DIR)/AlphaSpectrumSimulatorGlobals.hh \
				$(INC_DIR)/ProgressBar.hh
				
# RULES
# Make final executable and shared library
all : $(BIN_DIR)/alpha_spectrum_simulator $(LIB_DIR)/lib_alpha_spectrum_simulator.so

# Make shared library of all the objects linked together
$(LIB_DIR)/lib_alpha_spectrum_simulator.so: alpha_spectrum_simulator.o $(OBJECTS) alpha_spectrum_simulator_dict.o
	mkdir -p $(LIB_DIR)
	$(LD) alpha_spectrum_simulator.o $(OBJECTS) alpha_spectrum_simulator_dict.o $(SHAREDSWITCH)$@ $(LIBS) -o $@

# Final executable -- links all the prerequisite objects
$(BIN_DIR)/alpha_spectrum_simulator: alpha_spectrum_simulator.o $(OBJECTS) alpha_spectrum_simulator_dict.o
	mkdir -p $(BIN_DIR)
	$(LD) -o $@ $^ $(LDFLAGS) $(LIBS)

# Compile main object file
alpha_spectrum_simulator.o: alpha_spectrum_simulator.cc
	$(CC) $(CFLAGS) $(INCLUDES) $^

# Object file compilation
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cc $(INC_DIR)/%.hh
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Class dictionary generation
alpha_spectrum_simulator_dict.o: alpha_spectrum_simulator_dict.cc alpha_spectrum_simulator_dict$(DICTEXT) $(INC_DIR)/RootLinkDef.h
	mkdir -p $(BIN_DIR)
	mkdir -p $(LIB_DIR)
	$(CC) -fPIC $(CFLAGS) $(INCLUDES) -c $<
	cp alpha_spectrum_simulator_dict$(DICTEXT) $(BIN_DIR)/
	cp alpha_spectrum_simulator_dict$(DICTEXT) $(LIB_DIR)/

# Class dictionary source file generation
alpha_spectrum_simulator_dict.cc: $(DEPENDENCIES) $(INC_DIR)/RootLinkDef.h
	$(ROOTDICT) -f $@ -c $(INCLUDES) $(DEPENDENCIES) $(INC_DIR)/RootLinkDef.h

# Clean up files
clean:
	rm -vf $(BIN_DIR)/alpha_spectrum_simulator $(SRC_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*.gch *.o $(BIN_DIR)/*.pcm *.pcm $(BIN_DIR)/*dict* *dict* $(LIB_DIR)/* *.root *.png













































