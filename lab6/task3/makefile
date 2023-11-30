# 定义变量
CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: server client dealer
DELAY = 100


# 目标规则
server: server.o msg.o myproto.o game.o
	$(CC) $(CFLAGS) -o server server.o myproto.o game.o && ./server 12345

client: client.o msg.o myproto.o
	$(CC) $(CFLAGS) -o client client.o myproto.o && ./client 127.0.0.1 12345

dealer: dealer.o msg.o myproto.o
	$(CC) $(CFLAGS) -o dealer dealer.o myproto.o && ./dealer 127.0.0.1 12345

set-delay:
	sudo tc qdisc add dev lo root netem delay $(DELAY)ms 20ms distribution normal

unset-delay:
	sudo tc qdisc del dev lo root netem

run-server:
	docker build . -t net-server && docker run -p 12345:12345 -t net-server

# 依赖关系
server.o: server.c msg.c
	$(CC) $(CFLAGS) -c server.c

client.o: client.c msg.c
	$(CC) $(CFLAGS) -c client.c

game.o: game.c
	$(CC) $(CFLAGS) -c game.c

msg.o: msg.c
	$(CC) $(CFLAGS) -c msg.c

myproto.o: myproto.c myproto.h
	$(CC) $(CFLAGS) -c myproto.c


# 清理规则
clean:
	rm -f *.o server client
