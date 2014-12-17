#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	if (argc <= 2) {
		printf("usage: %s ip_address, port number.\n", basename(argv[0]));
		return 1;
	}
	const char* ip = argv[1];
	int port = atoi(argv[2]);

	struct sockaddr_in server_address;
	bzero(&server_address, sizeof(server_address));

	server_address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &server_address.sin_addr);
	server_address.sin_port = htons(port);

	/* TCP */
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(sockfd >= 0);
	
	int ret = bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address));
	assert(ret != -1);

	ret = listen(sockfd, 5);
	assert(ret != -1);

	struct sockaddr_in client;
	socklen_t client_addrlen = sizeof(client);
	int fd ;
	if ((fd = accept(sockfd, (struct sockaddr*)&client, &client_addrlen)) < 0) {
		printf("connect failed\n");
		return -1;
	}
	else {
		struct msghdr msg;
		struct iovec iov;
		char data[14];
		bzero(&msg, sizeof(msg));
		bzero(&iov, sizeof(iov));
		iov.iov_base = (void*)data;
		iov.iov_len = sizeof(data);
		msg.msg_name = NULL;
		msg.msg_namelen = sizeof(server_address);
		msg.msg_iov = &iov;
		msg.msg_iovlen = 1;
		
		if( recvmsg(fd, &msg, 0) < 0 ) {
			printf("recv failed: %s\n", strerror(errno));
		}	
		printf("msg: %s", data);
	}
	close(sockfd);
	return 0;

}