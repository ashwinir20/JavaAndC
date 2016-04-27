  #include <stdlib.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdbool.h>
  #include <string.h>
  #include <netdb.h>
  #include <stdio.h>
  #include <unistd.h>

  struct sockaddr_in server;
  struct sockaddr_in client;

  int advertised_window;

  double probability;

  int PORT;
  int socket_d;
  char filename[20];
  int read_data_len =0;
  int length;
  //int expected_seq_num;
  char header[50];
  char received_packet[1451];
  char packet_to_send[1451];

  bool random_bool(double probability_percent){

    return rand() < probability_percent*((double)RAND_MAX + 1.0);

  }

  void send_request(){


    int n;

    n = sendto(socket_d,filename,20,0,(const struct sockaddr *)&server,length);

    if( n < 0){
      perror("sending filename failed!");
    }else
    printf("sent a filename request!");

  }

  void send_ack(int ackN){

    printf("SENDING AN ACK!");

    int check;
    int ack;
    int sequence_no;
    char FLAG[1];
    sequence_no =0;

    strcpy(FLAG,"A");
    ack = ackN;

    sprintf(header,"%d:%d:%d:",sequence_no,ack,0);
    strcat(packet_to_send,header);
    strcat(packet_to_send,FLAG);
    strcat(packet_to_send,":");
    printf("\n%s",packet_to_send);

    check = sendto(socket_d,packet_to_send,200,0,(const struct sockaddr *)&server,length);
    memset(packet_to_send,0,100);

    if( check < 0){
      puts("sending ACK failed!!");

    } else{
      puts("ACK SENT!");
    }

  }

  void write_to_file(char data_writ[191]){

    FILE *fp;

    fp = fopen("output.txt","a");

    fwrite(data_writ,1,sizeof(data_writ),fp);

  }


  void process_packet(){

    printf("I came to process packet!");
    //process to check for seq no and calculate expected seq no using data length
    //if the subsequent seq no does not match drop the packet

    int seq_no;
    int ack_no;
    char seq_temp[2];
    char ack_temp[2];
    char length_temp[2];
    char data[191];
    int data_length;
    char ACK_FLAG[1];

    strcpy(seq_temp,strtok(received_packet,":"));

    seq_no = atoi(seq_temp);
    printf("\n************************");
    printf("\nSEQUENCE NUMBER RECEIVED:%d",seq_no);
    strcpy(ack_temp,strtok(NULL,":"));
    ack_no = atoi(ack_temp);
    strcpy(length_temp,strtok(NULL,":"));
    data_length = atoi(length_temp);
    printf("\n--------------");
    printf("\nLENGTH OF DATA:%d",data_length);
    strcpy(ACK_FLAG,strtok(NULL,":"));
    printf("\n***********************");
    printf("\nFLAG:%s",ACK_FLAG);
    strcpy(data,strtok(NULL,":"));
  //  printf("\n%s",data);

    read_data_len = read_data_len + data_length;
    printf("\nEXPECTED SEQUENCE NO:%d",read_data_len+1);
    //if it is the first packet
    if(seq_no == 0){
      printf("\nThis was the first packet!");
      //send ack anyway
      //read_data_len = data_length;
      send_ack(data_length +1);

    }

    printf("READ DATA LENGTH:%d",read_data_len);

    if( seq_no == (read_data_len +1)){

          printf("\nDo I come here?");
          send_ack(read_data_len+1);
    }




}

  void receive_data(){

      int check;

      do{

      check = recvfrom(socket_d,received_packet,200,0,(struct sockaddr *)&client,&length);

      if( random_bool(probability)){
        printf("\nPacket dropped!!!");
        continue;
      } else{
      printf("No packets dropped!");
      process_packet();
      }

    }while( check > 0);

    if(check <0){
      printf("\nNo more data to read!");
      exit(1);
    }

  }

  int main(int argc,char *argv[]){

    struct hostent *host;

    host = gethostbyname(argv[1]);
    PORT = atoi(argv[2]);
    advertised_window = atoi(argv[4]);
    strcpy(filename,argv[3]);
    //probability = atof(argv[5]);
    sscanf(argv[5],"%lf",&probability);

    if( argc < 6){
      printf("\nPlease input the correct number of arguments!");

    }

    socket_d = socket(AF_INET,SOCK_DGRAM,0);

    if( socket_d < 0 ){
      perror("error creating socket!");
      exit(1);
    }

    server.sin_family = AF_INET;

    bcopy((char *)host->h_addr,(char *)&server.sin_addr,host->h_length);

    server.sin_port = htons(PORT);

    length = sizeof(struct sockaddr_in);

    send_request();
    receive_data();

    close(socket_d);
    return 0;


}
