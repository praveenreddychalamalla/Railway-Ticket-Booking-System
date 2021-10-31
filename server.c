/* @author Praveen Reddy 
 * @date : 2021-10-27
 * @desc Concurrent server Program - Fork
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/in.h>
#include "serverUtils.h"

int main(){

    int sd,nsd,size;
    struct sockaddr_in server, client;

    if((sd=socket(AF_UNIX,SOCK_STREAM,0))<0){  //0 - Default Protocol, returns -1 on failure
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family=AF_UNIX;
    server.sin_addr.s_addr=INADDR_ANY ; //IP Address
    server.sin_port=htons(9586) ; //host to network short 

    if(bind(sd, (void *)&server, sizeof(server))<0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    listen(sd, 10); //Backlog Queue size

    size=sizeof(client);

    while(1){

        nsd=accept(sd,(void *)&client, &size); //Waits for client

        if(!fork()){ 
            close(sd);
            mainMenu(nsd);
            exit(EXIT_SUCCESS);
        }
        else close(nsd);
    }

    return 0;
}