#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/types.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>

char request_header[1024];
struct sockaddr_in address;
struct sockaddr_in client;
socklen_t from_length;
int fromlength;

void process_request(int new_fd){

    puts("in PROCESS REQUEST\n");
    puts(request_header);

    char method[50];
    char file_name[50];
    char protocol[50];
    char buffer[500];

    strcpy(method,strtok (request_header," \t"));
    strcpy(file_name,strtok (NULL," \t\n"));
    strcpy(protocol, strtok (NULL," \t\n"));


    int k;
    if( file_name[0] == '/') {

      for ( k=0 ; k < strlen(file_name) ; k++){
        file_name[k] = file_name[k + 1];
     }
   }


   FILE *fp;
   fp = fopen(file_name,"r");
   char c;
   int n =0;

   if( fp == NULL )
   {
     perror("can't open!");
     sendto(new_fd,"HTTP/1.1 404 Not Found\n",25,0,(struct sockaddr *)&client,from_length);
     exit(1);
   }
   else{

    int check;

    int content_length;
    char content[500];

    puts("do u even open the file");

    if( !feof(fp) ){

    fgets(buffer,sizeof(buffer),fp);

    strcat(content,"HTTP/1.1 200 OK\n");
    strcat(content,buffer);
    content_length = strlen(content);
    puts(content);

    check = sendto(new_fd,content,content_length,0,(struct sockaddr *)&client,from_length);
    if( check < 0){
      perror("No data sent");
      exit(0);
    }

    }
   }

}

int main(int argc,char *argv[]){

  if( argc < 2){
    perror("enter the port number!");
  }

  int PORT = atoi(argv[1]);
  printf("PORT:%d",PORT);

  int receive_d;
  int length;
  int sock_fd;

  sock_fd=socket(AF_INET,SOCK_DGRAM,0);
  if( sock_fd  == -1 ){
      perror("socket creation failed!");
      exit(1);
    }
    else{
      puts("socket created successfully");
    }
    printf("socket_value:%d",sock_fd);

  length = sizeof(address);

  bzero(&address,length);

  address.sin_family=AF_INET;
  address.sin_addr.s_addr=INADDR_ANY;
  address.sin_port=htons(PORT);

  int b;
  b = bind(sock_fd,(struct sockaddr *)&address,length);

  printf("binding result:%d\n", b);
  if (b == -1){
    printf("binding failed!");
    exit(0);
  }
  else{
    puts("bind done");
  }

  from_length = sizeof(struct sockaddr_in);

  while(1){

    //getting the GET request here

    receive_d = recvfrom(sock_fd,request_header,1024,0,(struct sockaddr *)&client,&from_length);

    if ( receive_d > 0){
      puts("i got soemthing!");
    }
    if( receive_d  < 0 ){
      perror("receive failed!");
    }
      //process the request
      process_request(sock_fd);

  }

}
