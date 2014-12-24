#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>

int main(int argc, char* argv[]) {
	assert(argc > 3);
	const char *ip = argv[1];
	int port = atoi(argv[2]);
	const char *file_name = argv[3];

	int filefd = open(file_name, O_RDONLY);
	assert(filefd > 0) ;
	struct stat stat_buf;
	fstat(filefd, &stat_buf);

	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, ip, &address.sin_addr);

	int sock = socket(PF_INET, SOCK_STREAM, 0);
	int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
	assert(ret != -1);

	ret = listen(sock , 5);
	assert(ret != -1);

	struct sockaddr_in client ;
	socklen_t client_addrlen = sizeof(client);
	int connfd = accept(sock , (struct sockaddr*)&client, &client_addrlen);
	if (connfd < 0) {
		printf("connect failed\n");
	}
	else {
		sendfile(filefd,connfd, NULL, &stat_buf.st_size, NULL , 0);
		close(connfd);
	}
	close(sock);
	return 0;

}
