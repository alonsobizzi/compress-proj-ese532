CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2 -g -DNDEBUG -pthread
LDFLAGS = -lpthread -pthread
INCLUDES = -IshaLib

EXECUTABLES = shaComp SHADemoNoPad
SRCFiles = $(wildcard ./*.cpp ./shaLib/*.cpp)

#make all g++ -o myshavar -IshaLib SHADemoNoPad.c ./shaLib/shaComp.c

.PHONY: clean
demo: $(SRCFiles)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

clean:
	$(RM) -rf perf* *.bin *.txt *.dSYM *.out *.o *~ $(EXECUTABLES)