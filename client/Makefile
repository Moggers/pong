COMPILE=gcc
RUN=./client 127.0.0.1 7778 matthew
MAIN_REQ=clean main.o ../general/net_structs.o ../general/ts_queue.o connect.o map.o 
MAIN_LINK=main.o ../general/net_structs.o ../general/ts_queue.o connect.o map.o -lpthread -lm -lglfw -lGL
MAIN_ARG=-o client
debug: DEBUG=-g -D DEBUG
all: DEBUG=

all: $(MAIN_REQ)
	$(COMPILE) $(MAIN_LINK) $(MAIN_ARG)

debug: $(MAIN_REQ)
	$(COMPILE) $(MAIN_LINK) $(MAIN_ARG)

test: debug
	$(RUN)

main.o: main.c
	$(COMPILE) $(DEBUG) main.c -c -o main.o

../general/net_structs.o: ../general/net_structs.c
	$(COMPILE) $(DEBUG) ../general/net_structs.c -c -o ../general/net_structs.o

../general/ts_queue.o: ts_queue.c
	$(COMPILE) $(DEBUG) ../general/ts_queue.c -c -o ../general/ts_queue.o

connect.o: connect.c
	$(COMPILE) $(DEBUG) connect.c -c -o connect.o

map.o: map.c
	$(COMPILE) $(DEBUG) map.c -c -o map.o

clean:
	rm -rf server *.o ../general/*.o
