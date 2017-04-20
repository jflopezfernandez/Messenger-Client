/** TCP Client in C
 *  
 *  1. Create a TCP socket using socket()
 *  2. Establish a connection to the server using connect()
 *  3. Communicate using send and recv()
 *  4. Close the connection with close()
 */


#include "includes\Main.h"


int main(int argc, char *argv[])
{
	// Check for correct number of arguments
	if (argc < 3 || argc > 4)
		DieWithUserMessage("Parameter(s)", "<Server Address> <Echo Word> [<Server Port>]");
	
	char *servIP = argv[1]; 		// First arg: server IP address
	char *echoString = argv[2]; 	// Second arg: string to echo
	
	// Optional third arg to pass in server port number
	in_port_t servPort = (argc == 4) ? atoi(argv[3]) : 7;
	
	// Initialize socket
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == -1)
		DieWithSystemMessage("socket() failed");
	
	// Construct the server address struct
	struct sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	
	// Convert address
	int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
	if (rtnVal == 0)
		DieWithUserMessage("inet_pton() failed", "invalid address string");
	else if (rtnVal == -1)
		DieWithSystemMessage("inet_pton() failed");
	
	// Server port
	servAddr.sin_port = htons(servPort);
	
	// Establish connection to echo server
	if (connect (sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		DieWithSystemMessage("connect() failed");
	
	// Determine input length
	size_t echoStringLen = strlen(echoString);
	
	// Send the string to the server
	size_t numBytes = send(sock, echoString, echoStringLen, 0);
	if (numBytes == -1)
		DieWithSystemMessage("send() failed");
	else if (numBytes != echoStringLen)
		DieWithUserMessage("send(),", "sent unexpected number of bytes");
	
	// Receive the same string back from the server
	unsigned int totalBytesRcvd = 0;
	fputs("Received: ", stdout);
	char buffer[BUFSIZE];
	
	while (totalBytesRcvd < echoStringLen)
	{
		// Receive up to the buffer size (minus 1 to leave space for null terminator)
		numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
		if (numBytes == -1)
			DieWithSystemMessage("recv() failed");
		else if (numBytes == 0)
			DieWithUserMessage("recv()", "connection closed prematurely");
		
		totalBytesRcvd += numBytes; 	// Keep track of total bytes
		buffer[numBytes] = '\0'; 		// Terminate the buffer string
		fputs(buffer, stdout); 			// Print the echo buffer
	}
	
	fputc('\n', stdout);
	
	close(sock);	
	return EXIT_SUCCESS;
}