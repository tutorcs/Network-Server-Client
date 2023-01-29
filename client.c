https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

int sendStuff(int sd, struct sockaddr_in server_address);
char *rtrim(char *s);
int main(int argc, char *argv[])
{
  int sd;
  struct sockaddr_in server_address;
  int portNumber;
  char serverIP[29];

  if (argc < 3){
    printf ("usage is client <ipaddr> <portnumber>\n");
    exit(1);
  }

  // create the socket

  sd = socket(AF_INET, SOCK_DGRAM, 0);

  portNumber = strtol(argv[2], NULL, 10); // convert the port 
  strcpy(serverIP, argv[1]); // use the IP address

  server_address.sin_family = AF_INET; //internet addressing
  server_address.sin_port = htons(portNumber); //concert 16 bit port number
  server_address.sin_addr.s_addr = inet_addr(serverIP); // convert ip of server

  for (;;){ // loop forever
    sendStuff( sd, server_address); // call my send function
  }
  return 0 ; 

}

int sendStuff(int sd, struct sockaddr_in server_address){
  int rc = 0;
  char bufferOut[16];
  char bufferIn[1000];
  int sequenceNumber = 0;
  int randomNumber = 19;
  size_t length;
  char *bPtr = bufferIn; // don't ask. sometimes i love indirection...
  
  // ask for a number and send it
  printf ("Enter the sequence number you'd like to send\n");
  rc = scanf("%d", &sequenceNumber);
  // weird thing is rc == 0 if character entered. told it i wanted numbers
  if (rc <=0){
    printf ("no funny business, only enter numbers\n");
    getline(&bPtr, &length, stdin); // eat the line
    return 0;//don't send anything
  }


  
  memset(bufferOut, 0, 16); // clear the buffer

  // now put the data into the buffer
  sprintf (bufferOut, "%11d%4d", sequenceNumber,
	   randomNumber);
  // hardcoding (don't do this) that the packet is 15 bytes in length
  rc = sendto(sd, bufferOut, 15, 0,
		  (struct sockaddr *) &server_address, sizeof(server_address));
  
  if (rc < 0)
    perror ("sendto");
  return 0; 

}
