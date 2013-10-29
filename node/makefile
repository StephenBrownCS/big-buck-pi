# Define variables
CFLAGS = -g

SENDER_OBJECTS = Packet.o UDPPacket.o BigBuckPacket.o SendingSocket.o ListeningSocket.o \
                   Utility.o\
                   LinkStatePacket.o \
                   HostAndPort.o Timer.o \
                   TestSender.o
RECEIVER_OBJECTS = Packet.o UDPPacket.o BigBuckPacket.o SendingSocket.o ListeningSocket.o \
                   Utility.o \
                   LinkStatePacket.o \
                   HostAndPort.o Timer.o \
                   TestReceiver.o


#all: This the old make all
#	g++ $(CFLAGS) requester.cpp tracker.cpp Packet.cpp UDP_Packet.cpp SendingSocket.cpp ListeningSocket.cpp Utility.cpp -o requester
#	g++ $(CFLAGS) sender.cpp Packet.cpp UDP_Packet.cpp SendingSocket.cpp ListeningSocket.cpp Utility.cpp -o sender

all: $(SENDER_OBJECTS) $(RECEIVER_OBJECTS) 
	g++ $(CFLAGS) $(SENDER_OBJECTS) -o sender
	g++ $(CFLAGS) $(RECEIVER_OBJECTS) -o receiver

TestSender.o: TestSender.cpp SendingSocket.h ListeningSocket.h Utility.h \
		Packet.h BigBuckPacket.h UDPPacket.h \
		HostAndPort.h \
		Timer.h
	g++ $(CFLAGS) TestSender.cpp -c

TestReceiver.o: TestReceiver.cpp SendingSocket.h Utility.h \
		Packet.h BigBuckPacket.h UDPPacket.h \
		HostAndPort.h \
		Timer.h
	g++ $(CFLAGS) TestReceiver.cpp -c
	
Packet.o: Packet.h Packet.cpp
	g++ $(CFLAGS) Packet.cpp -c
	
UDPPacket.o: UDPPacket.h UDPPacket.cpp HostAndPort.h
	g++ $(CFLAGS) UDPPacket.cpp -c
	
BigBuckPacket.o: BigBuckPacket.h BigBuckPacket.cpp BigBuckPacket.h
	g++ $(CFLAGS) BigBuckPacket.cpp -c

LinkStatePacket.o: Packet.h LinkStatePacket.cpp LinkStatePacket.h HostAndPort.h
	g++ $(CFLAGS) LinkStatePacket.cpp -c
	
HelloPacket.o: Packet.h HelloPacket.cpp HelloPacket.h HostAndPort.h
	g++ $(CFLAGS) HelloPacket.cpp -c
	
HostAndPort.o: HostAndPort.h HostAndPort.cpp
	g++ $(CFLAGS) HostAndPort.cpp -c
	
Utility.o: Utility.h Utility.cpp
	g++ $(CFLAGS) Utility.cpp -c
	
SendingSocket.o: SendingSocket.h SendingSocket.cpp Packet.h
	g++ $(CFLAGS) SendingSocket.cpp -c
	
ListeningSocket.o: ListeningSocket.h ListeningSocket.cpp Packet.h
	g++ $(CFLAGS) ListeningSocket.cpp -c
	
PacketFactory.o: PacketFactory.h PacketFactory.cpp \
						LinkStatePacket.h Utility.h \
						HelloPacket.h
	g++ $(CFLAGS) PacketFactory.cpp -c
	
Timer.o: Timer.h Timer.cpp Utility.h
	g++ $(CFLAGS) Timer.cpp -c
	
	
debug:
	g++ $(CFLAGS) TestReceiver.cpp Packet.cpp SendingSocket.cpp ListeningSocket.cpp \
                    UDPPacket.cpp BigBuckPacket.cpp \
                    HostAndPort.cpp Utility.cpp \
                    -o receiver	
	g++ $(CFLAGS) TestSender.cpp Packet.cpp SendingSocket.cpp ListeningSocket.cpp \
                    UDPPacket.cpp BigBuckPacket.cpp \
                    HostAndPort.cpp Utility.cpp \
                    -o sender	

clean:
	rm sender receiver *.o
