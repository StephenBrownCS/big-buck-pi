CFLAGS = -std=c++0x -O2 -Wall
LDFLAGS = -lwiringPi -I../../lib/wiringPi

all:
	g++ $(CFLAGS) TestHarness1.cpp $(LDFLAGS) -o harness1
	g++ $(CFLAGS) TestHarness2.cpp $(LDFLAGS) -o harness2
	g++ $(CFLAGS) TestHarness3.cpp $(LDFLAGS) -o harness3
	g++ $(CFLAGS) TestHarness4.cpp $(LDFLAGS) -o harness4

