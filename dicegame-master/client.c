#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> 
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>


int main(int argc, char *argv[])
{
  srand(time(NULL));
  signal(SIGINT, SIG_IGN);
  char message[100];
  int server, portNumber;
  socklen_t len;
  struct sockaddr_in servAdd;     // server socket address
  
 if(argc != 3){
    printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
    exit(0);
  }

  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
     fprintf(stderr, "Cannot create socket\n");
     exit(1);
  }

  servAdd.sin_family = AF_INET;
  sscanf(argv[2], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);

  if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){
  fprintf(stderr, " inet_pton() has failed\n");
  exit(2);
  }

 if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
  fprintf(stderr, "connect() has failed, exiting\n");
  exit(3);
 }


  int clientScore[2], serverScore[2];
  clientScore[0]=0;
  clientScore[1]=0;
  serverScore[0]=0;
  serverScore[1]=0;
  char ch;

  while(1)
  {

    printf("Press Enter to play or Enter $ to exit\n");
    
    while(1)
    {
      ch=fgetc(stdin);
      
      if(ch=='\n')
      {
        clientScore[0] = (rand() % 6)+1;
        clientScore[1]+=clientScore[0];
        break;
      }
      else if(ch=='$')
      {
        clientScore[0]=0;
        clientScore[1]=0;
        write(server, &clientScore, sizeof(clientScore));
        printf("I quit the game\n");
        exit(7);        
      }
      else
      {
        clientScore[0]=0;
        printf("Press enter to play\n");
      }
    }    

    printf("Client:\n");
    printf("Points earned: %d\n", clientScore[0]);
    printf("Total: %d\n", clientScore[1]);
    printf("\n");
    write(server, &clientScore, sizeof(clientScore));

    if(clientScore[1] >= 100)
    {
      if (read(server, &message, 50)<0){
        fprintf(stderr, "read() error\n");
        exit(3); 
      }
      
      printf("Message: %s\n",message);
      close(server);
      exit(5);
    }

    if (read(server, &serverScore, sizeof(serverScore))<0){
      fprintf(stderr, "read() error\n");
      exit(3); 
    }
    printf("Server:\n");    
    printf("Points earned: %d\n",serverScore[0]);
    printf("Total: %d\n",serverScore[1]);

    if(serverScore[1] >= 100)
    {
      if (read(server, &message, 50)<0){
        fprintf(stderr, "read() error\n");
        exit(3); 
      }
      printf("Message: %s\n",message);
      close(server);
      exit(5);
    }
 
 if (read(server, &message, 50)<0){
      fprintf(stderr, "read() error\n");
      exit(3); 
    }

    printf("Message: %s\n",message);
  }

  exit(0);
}
