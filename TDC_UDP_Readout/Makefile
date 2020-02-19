ALL_C    = $(wildcard *.cxx)
ALL_O    = $(ALL_C:.cxx=.o)

WD       = "./    :"

CXX		= gcc
LD		= gcc

DEBUGFLAG	= -g
STDLIBS		= -lpthread -lstdc++
INCFLAGS	= -I.
DEFFLAGS	=
CXXFLAGS	= $(DEBUGFLAG) -Wall -O -fexceptions $(INCFLAGS) -Wno-deprecated $(DEFFLAGS)
LDFLAGS		= $(STDLIBS)

default:	TDC_Readout

show:		
		@echo -en "OBJ files: \033[41;1;30m $(ALL_O)\033[0m\n"
		@echo "CXX flags: " $(CXXFLAGS)
		@echo "LD  flags: " $(LDFLAGS)
		@echo -en "\033[37;1;41m Внимание \033[0m\n"

depend.mk:
		@echo $(WD) "Create dependencies file"
		@$(CXX) $(CXXFLAGS) *.cxx  -MM > depend.mk

./%.o:          ./%.cxx depend.mk
		@echo "Compilation: "$<
		@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
		@echo $(WD) "Cleaning ..."
		@rm -f ./*.o  ./depend.mk ./*~ TDC_Readout

-include depend.mk

TDC_Readout:	main.cpp UDP_Readout.h $(ALL_O)
		@$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) main.cpp $(ALL_O)
		@echo "$@ Done..."

