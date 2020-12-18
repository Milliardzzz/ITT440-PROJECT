#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>	// Header contain time & date function


int main(int argc, char *argv[]){
	int socket_desc;
	struct sockaddr_in server;
	char message[2000], response[30];

	// Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc == -1)
	{
		printf("Could not create socket");
	}

	server.sin_addr.s_addr = inet_addr("192.168.0.163"); // Server IP address
	server.sin_family = AF_INET;
	server.sin_port = htons(9800);	// Using port 9800

	//Connect to remote server
	if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("[+]Connect error");
		return 1;
	}

	puts("[+]Connected");

	while(1)
	{
		//Send data message
		printf("Enter message: ");
		fgets(message, 2000, stdin);
		send(socket_desc, message,2000,0);

		//Time from server
		recv(socket_desc, response, 29, 0);
		printf("Time from server: %s", response);

		return 0;
	}

	return 0;
}

