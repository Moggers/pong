COMPILE=gcc
RUN=./server 7778
MAIN_REQ=clean main.o ../general/net_structs.o accept_thread.o ../general/ts_queue.o player.o
MAIN_LINK=../general/net_structs.o accept_thread.o ../general/ts_queue.o player.o main.o -lpthread -lm
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

../general/net_structs.o: ../general/net_structs.c
	$(COMPILE) $(DEBUG) ../general/net_structs.c -c -o ../general/net_structs.o

accept_thread.o: accept_thread.c
	$(COMPILE) $(DEBUG) accept_thread.c -c

../general/ts_queue.o: ../general/ts_queue.c
	$(COMPILE) $(DEBUG) ../general/ts_queue.c -c -o ../general/ts_queue.o

player.o: player.h
	$(COMPILE) $(DEBUG) player.c -c

clean:
	rm -rf server *.o
