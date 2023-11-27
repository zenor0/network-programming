#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#define PACKET_SIZE 64
#define ICMP_ECHO_REQUEST 8

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;

    if (len == 1)
        sum += *(unsigned char *)buf;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;

    return result;
}

void send_ping(int sockfd, struct sockaddr_in *target) {
    char packet[PACKET_SIZE];
    struct iphdr *ip_header = (struct iphdr *)packet;
    struct icmphdr *icmp_header = (struct icmphdr *)(packet + sizeof(struct iphdr));

    // 构建IP头部
    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->tot_len = sizeof(struct iphdr) + sizeof(struct icmphdr);
    ip_header->id = htons(0);
    ip_header->frag_off = 0;
    ip_header->ttl = 64;
    ip_header->protocol = IPPROTO_ICMP;
    ip_header->check = 0; // 计算校验和前需要将校验和字段置零
    ip_header->saddr = 0; // 源IP地址
    ip_header->daddr = target->sin_addr.s_addr;

    // 构建ICMP头部
    icmp_header->type = ICMP_ECHO_REQUEST;
    icmp_header->code = 0;
    icmp_header->checksum = 0;
    icmp_header->un.echo.id = getpid();
    icmp_header->un.echo.sequence = 0; // 可以递增以获得不同的标识符

    // 计算IP头部和ICMP头部的校验和
    ip_header->check = checksum(ip_header, sizeof(struct iphdr));
    icmp_header->checksum = checksum(icmp_header, sizeof(struct icmphdr));

    // 发送数据包
    if (sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)target, sizeof(struct sockaddr_in)) == -1) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
}

void receive_ping(int sockfd) {
    char buffer[PACKET_SIZE];
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);

    ssize_t bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&sender, &sender_len);
    if (bytes_received == -1) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }

    // 解析接收到的数据包
    struct iphdr *ip_header = (struct iphdr *)buffer;
    struct icmphdr *icmp_header = (struct icmphdr *)(buffer + sizeof(struct iphdr));

    // 输出解析结果
    printf("Received ICMP Echo Reply from %s\n", inet_ntoa(sender.sin_addr));
    printf("ICMP Type: %d\n", icmp_header->type);
    printf("ICMP Code: %d\n", icmp_header->code);
    printf("ICMP Checksum: 0x%04x\n", ntohs(icmp_header->checksum));
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <target_ip>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *target_ip = argv[1];

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in target;
    memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    if (inet_pton(AF_INET, target_ip, &target.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    // 发送ICMP Echo请求
    send_ping(sockfd, &target);

    // 接收ICMP Echo应答
    receive_ping(sockfd);

    // 关闭套接字
    close(sockfd);

    return 0;
}
