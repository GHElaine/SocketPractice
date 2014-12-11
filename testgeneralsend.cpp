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
	
	if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
		printf("errno: %d\n; return!!!\n",errno);
		return -1;
	}
	else {
		const char buffer[] = "hello Elaine!";
		struct msghdr msg;
		struct iovec iov;
		bzero(&iov, sizeof(iov));
		bzero(&msg, sizeof(msg));
		printf("%s\n", buffer);
		msg.msg_name = NULL;
		msg.msg_namelen = sizeof(server_address);
		iov.iov_base = (void*)buffer;
		iov.iov_len = strlen(buffer);
		msg.msg_iov = &iov;
		msg.msg_iovlen = 1;

		printf("len : %lu  msg : %s\n",strlen(buffer),msg.msg_iov->iov_base);
		
		if(sendmsg(sockfd, &msg, 0) < 0) {
			printf("failed\n");
		}				
	}
	close(sockfd);
	return 0;

}