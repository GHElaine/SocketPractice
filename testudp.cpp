#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <libgen.h>
#include <iostream>
using namespace std;

#define BUFFER_SIZE 2048

int main(int argc, char* argv[]) {
	if (argc <= 2) {
		printf("usage:%s ip_address, port number\n", basename(argv[0]));
		return 1;
	}
	const char* ip = argv[1];
	int port = atoi(argv[2]);

	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip , &address.sin_addr);
	address.sin_port = htons(port);

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	assert(sock >= 0);

	int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
	assert(ret != -1);

	//ret = listen(sock, 5);

	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof(client);
	// int connfd = accept(sock, (struct sockaddr*)&client, &client_addrlength);
	// if (connfd < 0) {
	// 	printf("errno is %d\n", errno);
	// }
	//else {
		char buffer[BUFFER_SIZE];
		memset(buffer,'\0',sizeof(buffer));
		ret = recvfrom(sock, buffer, BUFFER_SIZE-1, 0, (struct sockaddr*)&client, &client_addrlength);
		printf("got %d bytes of udp data '%s' \n", ret, buffer);

	//}
	close(sock);
	return 0;


}