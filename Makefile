override CXXFLAGS += -std=c++11 -g -Wall -O2 -pipe -Wno-sign-compare
INC               += -I.
LDFLAGS           :=
CC                := cc
CXX               := c++

DEPS     := hashids.o
BINARIES := encrypt

all: $(BINARIES)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INC) -c $< -o $@

encrypt: encrypt.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INC) $< -o $@ $(DEPS)

.PHONY: clean

clean:
	rm -rf $(BINARIES) $(DEPS)