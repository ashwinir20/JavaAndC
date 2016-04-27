#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>

#define MSS 200
#define SSTHRESH 1000
bool intial_start = true;
bool retransmit_flag = false;

int PORT;
int k =0;
int socket_d;
int advertised_window;
int read_len =0;
int cwnd;
int exponentVal;
int len_first_pck;

char filename[20];
socklen_t length;
struct sockaddr_in client;
char buffer[191];
char retransmit_buffer[191];


bool first_packet = true;
FILE *fp;

int data_sent =0;
int data_read =0;
char content[200];
char recv_buffer[200];
char header[50];

int duplicate_ack_count;

struct timeval tv;

//at a time 4 packets can remain unacknowledged
int expected_ack_array[4] = {0};
int ack_recv_array[4];

//packet header
int seq_no;
int ack_no =0;
char data[1400];
int data_length;
char ACK_FLAG[1];


void retransmit(int byte){

  FILE *f;

  byte = byte - 1;

  int byte_to_read_from;

  byte_to_read_from = byte - 191;

  f = fopen(filename,"r");

  fseek(f,byte_to_read_from,SEEK_SET);

  fread(retransmit_buffer,sizeof(char),191,f);

  printf("\nDATA BEING SENT AGAIN!:%s",retransmit_buffer);

  data_read = strlen(retransmit_buffer);
  seq_no = byte_to_read_from +1;

  printf("\n********************");
  printf("\nSEQUENCE NUMBER:%d\n",seq_no);
  printf("\n---------------");
  printf("\nLENGTH OF DATA:%d",data_sent);
  sprintf(header,"%d:%d:%d:",seq_no,ack_no,data_read);
  strcat(content,header);
  printf("\n---------------");
  printf("FLAG::%s",ACK_FLAG);
  strcpy(ACK_FLAG,"D"); //indicates data is being sent
  strcat(content,ACK_FLAG);
  strcat(content,":");
  strcat(content,retransmit_buffer);
  printf("\n********************");

  sendto(socket_d,content,200,0,(struct sockaddr *)&client,length);
  bzero(content,200);



}

void listen_for_ack(){

  int recv_check;
  //getting back the header
  int sequence_no;

  char temp_seqno[4];
  int acknow_no;
  char temp_ackno[4];
  char FLAG[1];
  char recv_data[1400];
  int recv_data_length;
  char temp_datalen[4];
  tv.tv_sec = 2;
  tv.tv_usec = 0;

  setsockopt(socket_d,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));

  printf("\nListening for ACK's");


  while((recv_check = recvfrom(socket_d,recv_buffer,200,0,(struct sockaddr *)&client,&length)) > 0){

  strcpy(temp_seqno,strtok(recv_buffer,":"));
  strcpy(temp_ackno,strtok(NULL,":"));

  acknow_no = atoi(temp_ackno);
  ack_recv_array[k] = acknow_no;
  printf("\nACK RECEIVED ARRAY:%d",ack_recv_array[k]);
  k++;
  printf("\n--------------");
  printf("\nACKNOWLEDGMENT");
  printf("\n%d",acknow_no);
  //printf("\n%d\n%d:K:",acknow_no,k);
  strcpy(temp_datalen,strtok(NULL,":"));
  strcpy(FLAG,strtok(NULL,":"));
  printf("\nFLAG");
  printf("%s",FLAG);
  memset(recv_buffer,0,200);

}//end of while

  k=0;
  //once you get an ack traverse the expected ACK NUMBER ARRAY to see if it is present
  int j;
  int m;
  int p;

  //check if you have received the expected ACK's
  // if NO then retransmit and call listen_for_ack again
/**  for(j=0; j<4; j++){

    p =  expected_ack_array[j];
    if( ack_recv_array[j] ==  p){
      printf("j:%d",j);
      printf("Do i come here!");
      expected_ack_array[j] = '\0';
      ack_recv_array[j] = '\0';
    }
  }

  for(m=0;m<4;m++){
    if(expected_ack_array[m] != '\0'){
      p = expected_ack_array[m];
      printf("RETRANSMITTING!");
      retransmit_flag = true;
      retransmit(p);
    }
  }

  printf("RETRANSMITTING ENDS!");

  if(retransmit_flag){
    listen_for_ack();
  } else{**/
      //go back to slow_start if you have received all the ACK's
      slow_start();
  

}


void send_data(int packet_no){

  printf("\ncreating a packet and sending it...");

  if( first_packet ){
  seq_no =0;
  ack_no =0;
  strcpy(ACK_FLAG,"D"); //indicates data is being sent
  data_sent = strlen(buffer);

  //forming the packet
  printf("\n********************");
  printf("\nSEQUENCE NO:%d",seq_no);
  printf("\n---------------");
  printf("\nLENGTH OF DATA:%d",data_sent);
  sprintf(header,"%d:%d:%d:",seq_no,ack_no,data_sent);
  strcat(content,header);
  printf("\n---------------");
  printf("FLAG::%s",ACK_FLAG);
  strcat(content,ACK_FLAG);
  strcat(content,":");
  strcat(content,buffer);
  printf("\n********************");
  sendto(socket_d,content,200,0,(struct sockaddr *)&client,length);

  //update the expected ACK

  read_len = data_sent;

  expected_ack_array[packet_no] = data_sent +1;
  first_packet = false;
  bzero(content,200);

  }else{

  seq_no = read_len +1;

  data_sent = strlen(buffer);

  read_len = read_len + data_sent;

  printf("\n********************");
  printf("\nSEQUENCE NUMBER:%d\n",seq_no);
  printf("\n---------------");
  printf("\nLENGTH OF DATA:%d",data_sent);
  sprintf(header,"%d:%d:%d:",seq_no,ack_no,data_sent);
  strcat(content,header);
  printf("\n---------------");
  printf("FLAG::%s",ACK_FLAG);
  strcpy(ACK_FLAG,"D"); //indicates data is being sent
  strcat(content,ACK_FLAG);
  strcat(content,":");
  strcat(content,buffer);
  printf("\n********************");

  sendto(socket_d,content,200,0,(struct sockaddr *)&client,length);
  bzero(content,200);

  expected_ack_array[packet_no] = read_len+1;

  }
  //sent packets - window size 4
  int j;
  for(j=0; j<4; j++){
    printf("Expected ACK:%d",expected_ack_array[j]);
  }

}

int minimum(int a,int b){

    return (a < b)? a : b;

}

void read_file(int n){


  if( !feof(fp) ){

  int i;

  for(i =0; i < n; i++ ){

  bzero(buffer,191);

  fread(buffer,sizeof(char),191,fp);

  send_data(i);

  }
  listen_for_ack();
}

}

int congestion_avoidance(){

  int minWindow;
  int number_of_packets;

  printf("Congestion Avoidance!");

  cwnd = cwnd + MSS*(MSS/cwnd);

  minWindow = minimum(cwnd,advertised_window);
  printf("Window size calculated:%d",minWindow);

  number_of_packets = minWindow/MSS;

  read_file(number_of_packets);

}

int slow_start(){

  printf("\nStarting slow start!");
  int minWindow;
  int number_of_packets;

  //calculating number of packets
  //getting the minimum of advertised window and congestion window

  if( intial_start ){

    printf("\nInitial Start!");
    cwnd = 200;
    minWindow = minimum(cwnd,advertised_window);
    printf("\nWindow size calculated:%d",minWindow);
    number_of_packets = minWindow/MSS;
    exponentVal =2;
    intial_start = false;
    read_file(number_of_packets);

  }
  //when u get an ACK you do exponentially increase cwnd
  else{

    printf("\nIncreasing CWND exponentially!");
    //checking cwnd
    if(cwnd >= SSTHRESH ){
      congestion_avoidance();
    }

    cwnd = exponentVal*cwnd;
    exponentVal = exponentVal*2;
    minWindow = minimum(cwnd,advertised_window);
    number_of_packets = minWindow/MSS;
    printf("Window size calculated:%d",minWindow);
    read_file(number_of_packets);
  }

}


void process_filename_request(){

  int n;

  n = recvfrom(socket_d,filename,20,0,(struct sockaddr *)&client,&length);

  if( n < 0 ){
    perror("Didn't get the filename!");
    exit(0);
  }else{
    printf("Reading the file:%s",filename);
  }


  int i;
  fp = fopen(filename,"r");

  if( fp == NULL ){
    perror("File does not exist!");
  }

  slow_start();

}


int main(int argc, char *argv[]){

  PORT = atoi(argv[1]);
  advertised_window = atoi(argv[2]);

  struct sockaddr_in server;

  int server_length;
  int bind_result;

  server_length = sizeof(server);

    //set up the socket
  socket_d = socket(AF_INET,SOCK_DGRAM,0);

  if( socket < 0 ){
    perror("error creating a socket");
  }

  bzero(&server,server_length);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind_result = bind(socket_d,(struct sockaddr *)&server,server_length);

    if( bind_result < 0 ){
      perror("Could not bind!");
      exit(0);
    } else{
      printf("\nServer started and listening at PORT:%d:",PORT);
    }

    length = sizeof(struct sockaddr_in);

    process_filename_request();

    return 0;

}
