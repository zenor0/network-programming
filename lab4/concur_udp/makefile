# 定义变量
CC = gcc
CFLAGS = -Wall -Wextra

.PHONY: server client


# 目标规则
server: server.o msg.o
	$(CC) $(CFLAGS) -o server server.o  && ./server 12345

client: client.o msg.o
	$(CC) $(CFLAGS) -o client client.o && ./client 127.0.0.1 12345
# 依赖关系
server.o: server.c msg.c
	$(CC) $(CFLAGS) -c server.c

client.o: client.c msg.c
	$(CC) $(CFLAGS) -c client.c

msg.o: msg.c
	$(CC) $(CFLAGS) -c msg.c

# 清理规则
clean:
	rm -f *.o server client
