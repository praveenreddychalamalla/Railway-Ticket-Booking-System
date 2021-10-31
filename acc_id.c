/* @author Praveen Reddy 
 * @date : 2021-10-27
 * @desc "account_id.dat is used to maintain the last used acc_id 
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "structures.h"

int main(){

    struct accId db;

    int fd=open("account_id.dat",O_RDWR|O_CREAT, S_IRWXU|S_IRGRP|S_IROTH); //0744

    if(fd==-1){
        perror("open");
        exit(EXIT_FAILURE);
    }
    db.acc_id=1;

    write(fd,&db,sizeof(db));
    close(fd);

    
    return 0;
}
