#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	assert( argc == 2) ;
	char *host = argv[1];
	struct hostent* hostinfo;
	bzero(&hostinfo, sizeof(hostinfo));
	hostinfo = gethostbyname(argv[1]);
	assert( hostinfo );

	struct servent* servinfo = getservbyname("daytime", "tcp");
	assert(servinfo);
	printf("daytime port is %d\n", ntohs(servinfo->s_port));
	printf("%d \n", hostinfo->h_addrtype);

	struct sockaddr_in address;
	bzero(&address,sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = servinfo->s_port;
	printf ("%s \n", *(hostinfo->h_addr_list));
	address.sin_addr = *(struct in_addr*)(*(hostinfo->h_addr_list));

	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	int ret = connect(sockfd, (struct sockaddr*)&address, sizeof(address));
	assert(ret != -1);

	char buffer[128];
	ret = read(sockfd, buffer, sizeof(buffer));
	assert( ret > 0);
	buffer[ret] = '\0';
	printf("the daytime is %s ", buffer);
	close(sockfd);
	return 0;
} 