#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#define MAX_SIZE 2048
#define MY_ERROR(s) printf(s); system("PAUSE"); exit(1);



int main(int argc , char *argv[]) {
	int serverSocket = 0, clientSocket = 0;
	struct sockaddr_in serverAddress, clientAddress; // sockaddr_in：IP4 格式使用 ,  sockaddr_in6：IP6 格式使用 , sockaddr：通用格式
	unsigned clientAddressLen;
	int bytesRead;
	char buf[MAX_SIZE];
	int serverPort;
	int firstFlag = 1;
	char msgSave[MAX_SIZE][MAX_SIZE];
	int msgNum = 0;

	/*
	if (argc == 2) {
		serverPort = atoi(argv[1]);
	}
	*/
	serverPort = 20110;
	// Create socket
	serverSocket = socket(PF_INET , SOCK_STREAM , 0); // (address , type , protocal(0表示不強制) )

	// Set the server information
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(serverPort); //converts a u_short from host to TCP/IP network byte order

	// Bind the socket
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		MY_ERROR("Bind Error\n");
	}

	// Prepare to listen to the incoming clients
	if (listen(serverSocket, 3) < 0) {
		MY_ERROR("Listen Error\n");
	}

	while (1) {
		printf("Waiting...\n");

		// Accept a client
		clientAddressLen = sizeof(clientAddress);
		clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);

		// Receive the data from the client, and send it back
		
		
        char ServerWelcomeMsg[MAX_SIZE] = "---MENU---\n1. Read All Messages.\n2. Write a new message.\nPlease Type 1 or 2 to select an option:";
        
		send(clientSocket, ServerWelcomeMsg, sizeof(ServerWelcomeMsg), 0);
		
		//printf("Firstflag = %d",firstFlag);

		bytesRead = recv(clientSocket, buf, sizeof(buf), 0);
		buf[bytesRead] = '\0';
		//printf("Client IP is : %s \n", inet_ntoa(clientAddress.sin_addr));
		printf("Receive %d byte(s): %s\n", bytesRead, buf);


		if (buf[0] == '1')
		{
			char ServerReMsg[MAX_SIZE] = "";
			//printf("%d",msgNum);
			if (msgNum == 0)
				strcat(ServerReMsg, "There aren't any new messages");
			else
				strcat(ServerReMsg, "All Messages:\n");
			for(int i=0; i<=msgNum; i++)
			{
				strcat(ServerReMsg, msgSave[i]);
				strcat(ServerReMsg, "\n");
			}
			//printf("%s",ServerReMsg);
			send(clientSocket, ServerReMsg, sizeof(ServerReMsg), 0);
		}
		else if (buf[0] == '2')
		{
			bytesRead = recv(clientSocket, buf, sizeof(buf), 0);
			strcpy(msgSave[msgNum],buf);
			printf("Recived %s\n",msgSave[msgNum]);
			msgNum+=1;

		}	
		else
			printf("!!!Not 1 or 2\n");
		close(clientSocket);
	}

  return 0;
}
