# Define variables
CFLAGS = -g -std=c++0x
LDFLAGS = -I../lib/gnublin-api

SENDER_OBJECTS = Packet.o UDPPacket.o BigBuckPacket.o SendingSocket.o ListeningSocket.o \
                   Utility.o\
                   LinkStatePacket.o \
                   HostAndPort.o Timer.o \
                   PacketConstants.o \
                   TestSender.o Logger.o
RECEIVER_OBJECTS = Packet.o UDPPacket.o BigBuckPacket.o SendingSocket.o ListeningSocket.o \
                   Utility.o \
                   LinkStatePacket.o \
                   HostAndPort.o Timer.o \
                   TestReceiver.o \
                   PacketConstants.o Logger.o
BIG_BUCK_NODE_OBJECTS = Packet.o UDPPacket.o BigBuckPacket.o SendingSocket.o ListeningSocket.o \
                   Utility.o PacketConstants.o \
                   LinkStatePacket.o \
                   HostAndPort.o Timer.o \
                   PIRSensor.o WifiCommunicator.o BigBuckSensingNode.o \
                   Main.o Logger.o


#all: This the old make all
#	g++ $(CFLAGS) requester.cpp tracker.cpp Packet.cpp UDP_Packet.cpp SendingSocket.cpp ListeningSocket.cpp Utility.cpp -o requester
#	g++ $(CFLAGS) sender.cpp Packet.cpp UDP_Packet.cpp SendingSocket.cpp ListeningSocket.cpp Utility.cpp -o sender

all: $(SENDER_OBJECTS) $(RECEIVER_OBJECTS) $(BIG_BUCK_NODE_OBJECTS)
	g++ $(CFLAGS) $(SENDER_OBJECTS) -o sender
	g++ $(CFLAGS) $(RECEIVER_OBJECTS) -o receiver
	g++ $(CFLAGS) $(BIG_BUCK_NODE_OBJECTS) gnublin.a -o big-buck-node

TestSender.o: TestSender.cpp SendingSocket.h ListeningSocket.h Utility.h \
		Packet.h BigBuckPacket.h UDPPacket.h \
		HostAndPort.h
	g++ $(CFLAGS) TestSender.cpp -c

TestReceiver.o: TestReceiver.cpp SendingSocket.h Utility.h \
		Packet.h BigBuckPacket.h UDPPacket.h \
		HostAndPort.h
	g++ $(CFLAGS) TestReceiver.cpp -c
	
Main.o: Main.cpp Packet.h UDPPacket.h BigBuckPacket.h  PacketConstants.h\
		Utility.h HostAndPort.h PIRSensor.h WifiCommunicator.h BigBuckSensingNode.h
	g++ $(CFLAGS) Main.cpp -c

BigBuckSensingNode.o: BigBuckSensingNode.cpp BigBuckSensingNode.h \
		Sensor.h Communicator.h BigBuckPacket.o
	g++ $(CFLAGS) BigBuckSensingNode.cpp -c
	
WifiCommunicator.o: WifiCommunicator.cpp WifiCommunicator.h Communicator.h \
		ListeningSocket.h SendingSocket.h UDPPacket.h Utility.h HostAndPort.h
	g++ $(CFLAGS) WifiCommunicator.cpp -c

PIRSensor.o: PIRSensor.cpp PIRSensor.h Sensor.h
	g++ $(CFLAGS) gnublin.a PIRSensor.cpp -c $(LDFLAGS)
	
Packet.o: Packet.h Packet.cpp
	g++ $(CFLAGS) Packet.cpp -c
	
UDPPacket.o: UDPPacket.h UDPPacket.cpp HostAndPort.h
	g++ $(CFLAGS) UDPPacket.cpp -c
	
BigBuckPacket.o: BigBuckPacket.h BigBuckPacket.cpp BigBuckPacket.h
	g++ $(CFLAGS) BigBuckPacket.cpp -c

PacketConstants.o: PacketConstants.h PacketConstants.cpp
	g++ $(CFLAGS) PacketConstants.cpp -c

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
	
Logger.o: Logger.h Logger.cpp
	g++ $(CFLAGS) Logger.cpp -c
	
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
                    
	g++ $(CFLAGS) Packet.cpp SendingSocket.cpp ListeningSocket.cpp \
                    UDPPacket.cpp BigBuckPacket.cpp \
                    HostAndPort.cpp Utility.cpp \
                    BigBuckSensingNode.cpp WifiCommunicator.cpp \
                    Main.cpp PIRSensor.cpp \
                    -o sender	

test-receiver:
	g++ -g TestReceiver.cpp Packet.cpp SendingSocket.cpp ListeningSocket.cpp \
                    UDPPacket.cpp BigBuckPacket.cpp \
                    HostAndPort.cpp Utility.cpp PacketConstants.cpp \
                    -o receiver	
                    
test-sender:
	g++ -g TestSender.cpp Packet.cpp SendingSocket.cpp ListeningSocket.cpp \
                    UDPPacket.cpp BigBuckPacket.cpp \
                    HostAndPort.cpp Utility.cpp PacketConstants.cpp \
                    -o sender	
                    
test-info:  
	g++ -g TestInfo.cpp Packet.cpp SendingSocket.cpp ListeningSocket.cpp \
                    UDPPacket.cpp BigBuckPacket.cpp \
                    HostAndPort.cpp Utility.cpp PacketConstants.cpp \
                    -o info   
                    
name-server: NameServerMain.cpp Packet.cpp SendingSocket.cpp ListeningSocket.cpp UDPPacket.cpp BigBuckPacket.cpp HostAndPort.cpp Utility.cpp PacketConstants.cpp Packet.h SendingSocket.h ListeningSocket.h UDPPacket.h BigBuckPacket.h HostAndPort.h Utility.h PacketConstants.h
	g++ -g NameServerMain.cpp Packet.cpp SendingSocket.cpp ListeningSocket.cpp \
                    UDPPacket.cpp BigBuckPacket.cpp \
                    HostAndPort.cpp Utility.cpp PacketConstants.cpp \
                    -o name-server	     


clean:
	rm sender receiver name-server big-buck-node *.o
