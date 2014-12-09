#include <sys/types.h>
#include <sys/socket.h>
#include <libgen.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
	if (argc <= 2) {
		printf ("usage: %s ip_address, port number.\n", basename(argv[0]));
		return 1;
	}
	
	int port = atoi(argv[2]);
	const char* ip = argv[1];

	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);

	int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	assert(sockfd >= 0);

	
	// if (connect(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0) {
	// 	printf("connect failed\n");
	// 	return -1;
	// } 
	//else {
		const char* buffer = "hello Elaine";
		sendto(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&address, sizeof(address));
	//}
	close(sockfd);
	return 0;

}