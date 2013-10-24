# Define variables
CFLAGS = -g

EMULATOR_OBJECTS = Emulator.o packet.o IP_packet.o sending_socket.o listening_socket.o \
                    utility.o UDP_packet.o ICMP_Packet_Factory.o \
                    RouteTrace_Packet.o LinkState_Packet.o ICMP_Packet.o \
                    RoutingTable.o HostAndPort.o Hello_Packet.o Timer.o

ROUTE_TRACE_OBJECTS = routetrace.o IP_packet.o sending_socket.o listening_socket.o utility.o \
			RouteTrace_Packet.o ICMP_Packet.o UDP_packet.o ICMP_Packet_Factory.o \
			LinkState_Packet.o HostAndPort.o Hello_Packet.o

#all: This the old make all
#	g++ $(CFLAGS) requester.cpp tracker.cpp packet.cpp UDP_packet.cpp sending_socket.cpp listening_socket.cpp utility.cpp -o requester
#	g++ $(CFLAGS) sender.cpp packet.cpp UDP_packet.cpp sending_socket.cpp listening_socket.cpp utility.cpp -o sender

all: $(EMULATOR_OBJECTS) $(ROUTE_TRACE_OBJECTS) 
#	g++ $(REQUESTER_OBJECTS) -o requester
#	g++ $(SENDER_OBJECTS) -o sender
	g++ $(EMULATOR_OBJECTS) -o emulator
	g++ $(ROUTE_TRACE_OBJECTS) -o trace

Emulator.o: Emulator.cpp sending_socket.h listening_socket.h utility.h \
		UDP_packet.h IP_packet.h packet.h ICMP_Packet.h LinkState_Packet.h \
		RouteTrace_Packet.h HostAndPort.h RoutingTable.h Hello_Packet.h \
		Timer.h
	g++ $(CFLAGS) Emulator.cpp -c

routetrace.o: routetrace.cpp sending_socket.h listening_socket.h utility.h ICMP_Packet.h \
		packet.h IP_packet.h RouteTrace_Packet.h UDP_packet.h LinkState_Packet.h \
		HostAndPort.h Hello_Packet.h
	g++ $(CFLAGS) routetrace.cpp -c
	
tracker.o: tracker.h tracker.cpp
	g++ $(CFLAGS) tracker.cpp -c
	
packet.o: packet.h packet.cpp
	g++ $(CFLAGS) packet.cpp -c
	
UDP_packet.o: UDP_packet.h UDP_packet.cpp packet.h
	g++ $(CFLAGS) UDP_packet.cpp -c
	
IP_packet.o: IP_packet.h IP_packet.cpp packet.h UDP_packet.h
	g++ $(CFLAGS) IP_packet.cpp -c
	
ICMP_Packet.o: packet.h ICMP_Packet.h ICMP_Packet.cpp
	g++ $(CFLAGS) ICMP_Packet.cpp -c
	
LinkState_Packet.o: ICMP_Packet.h LinkState_Packet.cpp LinkState_Packet.h HostAndPort.h
	g++ $(CFLAGS) LinkState_Packet.cpp -c
	
RouteTrace_Packet.o: ICMP_Packet.h RouteTrace_Packet.cpp RouteTrace_Packet.h	
	g++ $(CFLAGS) RouteTrace_Packet.cpp -c
	
Hello_Packet.o: ICMP_Packet.h Hello_Packet.cpp Hello_Packet.h HostAndPort.h
	g++ $(CFLAGS) Hello_Packet.cpp -c
	
HostAndPort.o: HostAndPort.h HostAndPort.cpp
	g++ $(CFLAGS) HostAndPort.cpp -c
	
RoutingTable.o: RoutingTable.h RoutingTable.cpp HostAndPort.h LinkState_Packet.h Hello_Packet.h
	g++ $(CFLAGS) RoutingTable.cpp -c
	
utility.o: utility.h utility.cpp
	g++ $(CFLAGS) utility.cpp -c
	
sending_socket.o: sending_socket.h sending_socket.cpp packet.h
	g++ $(CFLAGS) sending_socket.cpp -c
	
listening_socket.o: listening_socket.h listening_socket.cpp packet.h UDP_packet.h \
		ICMP_Packet_Factory.h
	g++ $(CFLAGS) listening_socket.cpp -c
	
ICMP_Packet_Factory.o: ICMP_Packet_Factory.h ICMP_Packet_Factory.cpp \
						LinkState_Packet.h RouteTrace_Packet.h utility.h \
						Hello_Packet.h
	g++ $(CFLAGS) ICMP_Packet_Factory.cpp -c
	
Timer.o: Timer.h Timer.cpp utility.h
	g++ $(CFLAGS) Timer.cpp -c
	
	
debug:
	g++ $(CFLAGS) Emulator.cpp packet.cpp IP_packet.cpp sending_socket.cpp listening_socket.cpp \
                    utility.cpp UDP_packet.cpp ICMP_Packet_Factory.cpp \
                    RouteTrace_Packet.cpp LinkState_Packet.cpp ICMP_Packet.cpp \
                    RoutingTable.cpp HostAndPort.cpp Hello_Packet.cpp Timer.cpp \
                    -o emulator	

clean:
	rm emulator trace *.o
