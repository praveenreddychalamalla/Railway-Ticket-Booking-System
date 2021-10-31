/* @author Praveen Reddy 
 * @date : 2021-10-24
 * @desc Client Program
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <netinet/in.h>
#include "clientUtils.h"

int main(){

    int sd,nsd,size;
    struct sockaddr_in serv;

    if((sd=socket(AF_UNIX,SOCK_STREAM,0))<0){  //0 - Default Protocol, returns -1 on failure
        perror("socket");
        exit(EXIT_FAILURE);
    }
    serv.sin_family=AF_UNIX;
    serv.sin_addr.s_addr=INADDR_ANY ; //IP Address
    serv.sin_port=htons(9586) ; //host to network short 

    if(connect(sd, (void *)&serv, sizeof(serv))<0){
        perror("connect");
        exit(EXIT_FAILURE);
    }

    mainMenu(sd);
    close(sd);

    return 0;
}