/**
 * Este aplicacion representa a una entidad crediticia la cual
 * concentra todos los creditos otorgados a los clientes de
 * diferentes entidades.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <winsock2.h>
#include <unistd.h>

#define PORT 50000 /* El puerto que será abierto */
#define BACKLOG 2 /* El número de conexiones permitidas */

void doprocessing (int sock)
{
    char buffer[256], myString[100], myString2[100], *aux, rfc[256], x[2] = {'#','#'}, *token, del[2] = "|", taker;
    memset(buffer, 0, 255);
	memset(rfc, 0, 255);
	int lenght, recvMsgSize, i;
	FILE *file;
    
    /* Receive message from client */
    if ((recvMsgSize = recv(sock, buffer, 256, 0)) < 0)
        perror("ERROR reading to socket");
		
	for(i=0; i<256; i++)
	{
			rfc[i] = buffer[i];
	}
	memset(buffer, 0, 255);
	
	printf("Here is the message: %s\n",rfc); //Just for testing if buffer is received
	
	
	file = fopen("C:\\Users\\usuario\\Documents\\GitHub\\WORKSHOP\\workshop-gdl\\workshop\\credit-bureau\\src\\c\\Loans.txt","r");
	if (file == NULL)
		perror ("Error opening file");
		
      else {
		//while(feof(file) == 0)
		while(!feof(file))
		{
			aux = fgets(myString, sizeof(myString), file);
			lenght = strlen(myString);
            if (strstr(myString,rfc))
			{
                 puts (aux);
				 send(sock, aux, lenght, 0);
				 }
				 memset(myString, 0, sizeof(myString));
		}
		//printf("Taker finish:  %s",taker);
		send(sock, x, sizeof(x), 0);
		
		while(!feof(file))
		{
			aux = fgets(myString2, sizeof(myString2), file);
            
			token = strtok(myString2, del);
			printf("\nToken :  %s\n",token);
				 memset(myString2, 0, sizeof(myString2));
		}
		fclose(file);
		}
		
    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
        /* Echo message back to client */
        if (send(sock, buffer, recvMsgSize, 0) != recvMsgSize)
            perror("ERROR writing to socket");

      /* See if there is more data to receive */
        if ((recvMsgSize = recv(sock, buffer, 256, 0)) < 0)
            perror("ERROR reading to socket");
    }

    closesocket(sock);    /* Close client socket */
}

BOOL initW32() 
{
		WSADATA wsaData;
		WORD version;
		int error;
		
		version = MAKEWORD( 2, 0 );
		
		error = WSAStartup( version, &wsaData );
		
		/* check for error */
		if ( error != 0 )
		{
		    /* error occured */
		    return FALSE;
		}
		
		/* check for correct version */
		if ( LOBYTE( wsaData.wVersion ) != 2 ||
		     HIBYTE( wsaData.wVersion ) != 0 )
		{
		    /* incorrect WinSock version */
		    WSACleanup();
		    return FALSE;
		}	
}

int main()
{

	 initW32(); /* Necesaria para compilar en Windows */ 
	 	
   int fd, fd2; /* los descriptores de archivos */
  // char buffer[256];
  // memset(buffer, 0, 255);
   

   /* para la información de la dirección del servidor */
   struct sockaddr_in server;

   /* para la información de la dirección del cliente */
   struct sockaddr_in client;

   unsigned int sin_size;

   pid_t pid;

   /* A continuación la llamada a socket() */
   if ((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {	/*AF_INET -> Address family format for IPv4
														  SOCK_STREAM -> A socket type that provides sequenced, 
														  reliable, two-way, connection-based byte streams. This 
														  socket type uses the Transmission Control Protocol (TCP).
														  0 -> Protocol used. If a value of 0 is specified, the caller 
														  does not wish to specify a protocol and the service provider 
														  will choose the protocol to use.*/
      printf("error en socket()\n");
      exit(-1);
   }

   server.sin_family = AF_INET; //AF_INET -> Address family format for IPv4

   server.sin_port = htons(PORT); /*Internet Protocol (IP) port
									The htons() function converts PORT from host to TCP/IP network byte order.
									*/
									
   server.sin_addr.s_addr = INADDR_ANY;		/*sinn_addr -> IP address in network byte order.
											  INADDR -> it allows the server to receive packets destined to any 
											  of the interfaces. Coloca nuestra dirección IP automáticamente */

   //bzero(&(server.sin_zero),8);
   memset(&(server.sin_zero), '0', 8);	/*Sets buffers to a specified character.*/
   /* escribimos ceros en el reto de la estructura */


   /* A continuación la llamada a bind().The bind function associates a local address with a socket.
	  If no error occurs, bind returns zero. Otherwise, it returns SOCKET_ERROR */
   if(bind(fd,(struct sockaddr*)&server, sizeof(struct sockaddr))==-1) {
      printf("error en bind() \n");
      exit(-1);
   }

   if(listen(fd,BACKLOG) == -1) {  /* llamada a listen() */
      printf("error en listen()\n");
      exit(-1);
   }

   while(1) {
      sin_size=sizeof(struct sockaddr_in);
      /* A continuación la llamada a accept() */
      if ((fd2 = accept(fd,(struct sockaddr *)&client, &sin_size))==-1) {
         printf("error en accept()\n");
         exit(-1);
      }
		printf("Se obtuvo una conexion desde %s\n", inet_ntoa(client.sin_addr) ); /*inet_ntoa function converts 
																				  an (Ipv4) Internet network 
																				  address into an ASCII string 
																				  in Internet standard dotted-decimal 
																				  format.
																				  que mostrará la IP del cliente */

      //send(fd2,"Welcome to C server, I got your message!.\n",1000,0);
      /* que enviará el mensaje de bienvenida al cliente */
      
      doprocessing(fd2);

   } /* end while */
}
