COMPILE=gcc
RUN=./server 7777
MAIN_REQ=clean main.o net_structs.o accept_thread.o ts_queue.o player.o
MAIN_LINK=net_structs.o accept_thread.o ts_queue.o player.o main.o -lpthread -lm
MAIN_ARG=-o server
debug: DEBUG=-g -D DEBUG
all: DEBUG=

all: $(MAIN_REQ)
	$(COMPILE) $(MAIN_LINK) $(MAIN_ARG)

debug: $(MAIN_REQ)
	$(COMPILE) $(MAIN_LINK) $(MAIN_ARG)

test: debug
	$(RUN)

main.o: main.c
	$(COMPILE) $(DEBUG) main.c -c

net_structs.o: net_structs.c
	$(COMPILE) $(DEBUG) net_structs.c -c

accept_thread.o: accept_thread.c
	$(COMPILE) $(DEBUG) accept_thread.c -c

ts_queue.o: ts_queue.c
	$(COMPILE) $(DEBUG) ts_queue.c -c

player.o: player.h
	$(COMPILE) $(DEBUG) player.c -c

clean:
	rm -rf server *.o
