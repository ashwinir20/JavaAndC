#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>

char file_request[20];
char hostname[20];
int PORT;
int length;
struct sockaddr_in address;
struct sockaddr_in client;
int socket_fd;
char received_from[500];
char request[1024];

void form_request(){

    int n;
    int send_to_check;

    strcat(request,"GET ");
    strcat(request,"/");
    strcat(request,file_request);
    strcat(request," ");
    strcat(request,"HTTP/1.1");
    puts(request);

    //check for \0

    send_to_check = sendto(socket_fd,request,sizeof(request),0,(struct sockaddr *)&address,length);

    if ( send_to_check < 0 ){
      perror("sending request failed!");
    }

    int check;

    do{

    check = recvfrom(socket_fd,received_from,500,0,(struct sockaddr *)&client,&length);

    if ( check < 0){
      perror("i didn't get anything!");
      exit(0);
    }

    puts(received_from);

    int response_length;

    response_length = strlen(received_from);

    if ( received_from[response_length] == '\0' ){
      puts("last byte received!");
      break;
    }

    }while( check > 0);

}

void main( int argc, char *argv[]){

  struct hostent *server;
  length = sizeof(struct sockaddr_in);

  if( argc < 4){
    perror("enter the correct number of arguments!");
  }

  socket_fd  = socket(AF_INET,SOCK_DGRAM,0);
  if( socket_fd < 0 ){
      perror("socket creation failed!");
  }

  server = gethostbyname(argv[1]);
  PORT = atoi(argv[2]);
  strcpy(file_request,argv[3]);

  //create socket

  address.sin_family = AF_INET;
  address.sin_port = htons(PORT);
    bcopy((char *)server->h_addr,(char *)&address.sin_addr,server->h_length);

  bzero(request,sizeof(request));
  form_request();

  close(socket_fd);

}
