https://tutorcs.com
WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int create_socket(int *sd, int portNumber);

int main(int argc, char *argv[])
{
  int sd; /* socket descriptor */
  int rc; /* return code from recvfrom */
  struct sockaddr_in from_address; // the 'from' address on recvfrom
  char buffer[100]; // buffer i will receive data into

  if (argc < 2){
    printf("usage is: server <portnumber>\n");
    exit (1);
  }
 
  int seqNumber, randomNumber; // the 'data' i am expecting

  rc = create_socket(&sd, atoi(argv[1])); // wanted to modularize
  if (rc == -1) // something bad happened, exit
    printf("error from create socket\n");

  for (;;){ // loop forever, getting a seq number and a random number

    int rc;
    int flags = 0;
    socklen_t fromLength = sizeof(from_address); // unix unstable if not set

    memset (buffer, 0, 100); // always always clear the buffer
  
    // get the data from the client
    rc = recvfrom(sd, &buffer, 100, flags, (struct sockaddr *)&from_address,
		  &fromLength); // sd is socket, data put in buffer, up to
                                // 100 bytes. the 'from_address' contains
                               // the return address

    if (rc <= 0){ // check return code just in case
      perror ("recvfrom");
      exit (1);
    }


    // parse the data. remember the format of the data in the prompt
    // notice how this parallels the sprintf in the client
    sscanf(buffer, "%11d%4d", &seqNumber, &randomNumber);

    // print out the datan
    printf ("thanks! you send seq # %d, and random number %d\n",
	    seqNumber, randomNumber);
  }// end of the loop forever

  return 0;
}


int create_socket(int *sd, int portNumber){
  struct sockaddr_in server_address;

  int rc;
  
  *sd = socket (AF_INET, SOCK_DGRAM, 0); // sd passed by reference
  
  server_address.sin_family = AF_INET; // internet addressing
  server_address.sin_port = htons(portNumber); // htons on 16bit port
  server_address.sin_addr.s_addr = INADDR_ANY; // server, any adapter works

  // do the bind and ALWAYS check the rc
  rc = bind (*sd, (struct sockaddr *)&server_address, sizeof(server_address));
  if (rc < 0){
    perror("bind"); // probably means someone using my port number!
    exit (1);
  }
  return 1;
}

