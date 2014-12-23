#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <libgen.h>
#include <sys/uio.h>

int main(int argc , char *argv[]) {
	assert(argc > 2) ;

	const char *ip = argv[1];
	int port = atoi(argv[2]);

	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, ip, &address.sin_addr);
	socklen_t addrlength = sizeof(address);

	int sock = socket(PF_INET, SOCK_STREAM, 0);
	int connfd = connect(sock, (struct sockaddr*)&address, sizeof(address));

	if (connfd < 0) {
		printf("connect failed\n");
	}
	else{
		char str[40];
		/* receive message from sever!
		 * recvfrom(sock,str,10,0,(struct sockaddr*)&address, &addrlength);
		 */

		//close(STDIN_FILENO);
		//dup(sock);
		struct iovec iov[1];
		memset(&iov, '\0', sizeof(iov));
		iov[0].iov_base = str;
		iov[0].iov_len = 40;
		readv(sock, iov, 2);
		//printf("%d\n", scanf("%s", str));
		printf("success: %s , yeah!\n", str);
	}
	close(sock);
	return 0;
}