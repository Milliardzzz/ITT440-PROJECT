#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>	// Header contain time and date function

int main(int argc, char *argv[])
{
	int socket_desc, new_socket, c;
	struct sockaddr_in server, client;
	char client_reply[2000];
	int optval;
	socklen_t optlen = sizeof(optval);

	// Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc == -1)
	{
		printf("[+]Could not create socket");
	}


	// Check the status for the keepalive option
	if(getsockopt(socket_desc, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0)
	{
		perror("getsockopt()");
		close(socket_desc);
		exit(EXIT_FAILURE);
	}

	//Set the option active
	optval = 1;
	optlen = sizeof(optval);

	if(setsockopt(socket_desc, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0)
	{
		perror("setsockopt()");
		close(socket_desc);
		exit(EXIT_FAILURE);
	}


	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(9800);	//Using port 9800

	//Bind
	if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("[+]Bind failed");
		return 1;
	}

	puts("[+]Bind done");

	//Listen
	listen(socket_desc, 3);

	//Accept and incoming connection
	puts("[+]Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	while(1){
		new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

		puts("[+]Connection accepted");

		recv(new_socket, client_reply, 2000,0);
		printf("\nClient: %s", client_reply);

		//Daytime protocol
		time_t currentTime;
		time(&currentTime);

		printf("Client requested for time at %s", ctime(&currentTime));
		send(new_socket, ctime(&currentTime), 30, 0);

		return 0;
	}

	if(new_socket < 0)
	{
		perror("[+]Accept failed");
		return 1;
	}

	return 0;
}
