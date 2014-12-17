#include <sys/socket.h> 
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

#define BUFFER_SIZE 512 

int main(int argc, char* argv[]) {
	if (argc <= 2) {
		printf("usage: %s ip_address port_num send_buffer_size\n", basename(argv[0]));
		return 1;
	}
	const char *ip = argv[1];
	int port = atoi(argv[2]);
	int buffer = atoi(argv[3]);

	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &addr.sin_addr);

	int sock = socket(PF_INET, SOCK_STREAM, 0);
	int len = sizeof(buffer);

	setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &buffer, sizeof(buffer));
	getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &buffer, (socklen_t*)&len);

	printf("the tcp send buffer size after setting is %d\n", buffer);

	if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != -1) {
		char buf[BUFFER_SIZE];
		memset(buf, 'a', BUFFER_SIZE);
		send(sock , buf, BUFFER_SIZE, 0);
	}
	close(sock);
	return 0;
}	
