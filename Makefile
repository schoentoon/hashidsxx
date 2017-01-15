override CXXFLAGS += -g -Wall -O2 -pipe -Wextra -Wno-sign-compare
INC               += -I.
LDFLAGS           :=
CC                := cc
CXX               := c++

DEPS     := hashids.o
BINARIES := hashidsxx

all: $(BINARIES)

%.o: %.cpp %.h
	$(CXX) -std=c++11 $(CXXFLAGS) $(INC) -c $< -o $@

hashidsxx: hashidsxx.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) $(INC) $< -o $@ $(DEPS)

test: test.out
	./test.out

test.out: test.cpp libgtest.a $(DEPS)
	$(CXX) -std=c++11 $(CXXFLAGS) $(INC) -Igtest-1.7.0/include -o $@ $< libgtest.a $(DEPS) -pthread

gtest-1.7.0:
	wget -q https://github.com/google/googletest/archive/release-1.7.0.zip
	unzip -qq release-1.7.0.zip
	rm release-1.7.0.zip
	mv googletest-release-1.7.0 gtest-1.7.0

libgtest.a: gtest-1.7.0
	$(CXX) -std=c++11 -I gtest-1.7.0/include -I gtest-1.7.0 -c gtest-1.7.0/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

.PHONY: clean

clean:
	rm -rf $(BINARIES) $(DEPS) test.out
