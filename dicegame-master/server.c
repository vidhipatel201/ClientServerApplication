#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <time.h>

static int client_count = 0;

void serviceClient(int client, int client_number)
{
  int clientScore[2], serverScore[2];
  clientScore[0]=0;
  clientScore[1]=0;
  serverScore[0]=0;
  serverScore[1]=0;
  char message[255];

  while(1)
  {
    
    if(read(client, &clientScore, sizeof(clientScore))<0)
    {
      fprintf(stderr, "read() error\n");
      exit(3);
    }
    if(clientScore[0] == 0)
    {
      printf("Client %d has left\n",client_number);
      close(client);
      exit(5);      
    }
    printf("Client:\n");
    printf("Points earned: %d\n",clientScore[0]);
    printf("Total: %d\n",clientScore[1]);
    

    if(clientScore[1] >= 100)
    {
      write(client,"Game over: You won the game.",50);
      printf("Client won\n");
      printf("Client %d is gone\n",client_number);  
      close(client);
      exit(5);
    }
   
    serverScore[0] = (rand() % 6)+1;
    serverScore[1]+=serverScore[0];
    printf("Server:\n");
    printf("Points earned: %d\n", serverScore[0]);
    printf("Total: %d\n", serverScore[1]);
    write(client, &serverScore, sizeof(serverScore));
      
    if(serverScore[1] >= 100)
    {
      write(client,"Game over: You lost the game.",50);      

      printf("Client %d is gone\n",client_number);
        
      close(client);
      exit(5);
    }

    write(client,"Game on: you can now play your dice",50);
    
  }
    exit(1); 
}

int main(int argc, char *argv[]){  

  srand(time(NULL));
  pid_t p;
  int n;
  int sd, client, portNumber;
  socklen_t len;
  char *currentTime;
  time_t currentUnixTime;

  struct sockaddr_in servAdd;     // server socket address
  
 if(argc != 2){
    printf("Call model: %s <Port Number>\n", argv[0]);
    exit(0);
  }

  sd = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;
  servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
  sscanf(argv[1], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);
  
  bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
  listen(sd, 5);

  while(1){
    client = accept(sd, NULL, NULL);
    client_count++;
    printf("Client %d joined\n",client_count);
    if(!fork())
    {
      serviceClient(client, client_count);
    }    
  }
    close(client);  
}
