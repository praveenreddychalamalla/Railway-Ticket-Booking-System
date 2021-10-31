/* @author Praveen Reddy 
 * @date : 2021-10-24
 * @desc Server Utils
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include "structures.h"
#define buffer_size 80

void mainMenu(int);
void signup(int, int);
void newUser(int);
void displayTrains(int);

struct flock lock; 
struct flock accountlock; 

void displayUserAccountIds(int sd){

	int fd = open("accounts.dat",O_RDWR , 0666);
	int flag=1;

	struct accountDataBase db;

	while(read(fd, (char *)&db, sizeof(db))){
		if(db.active==1 && (db.acc_type==1 || db.acc_type==2)){
			write(sd,&flag,sizeof(flag));
			write(sd,&db,sizeof(db));
		}
	}
	flag=0;
	write(sd,&flag,sizeof(flag));
}

void deleteUser(int sd){

	displayUserAccountIds(sd);
	int acc_id,flag=0;

	read(sd,&acc_id,sizeof(acc_id));

	int fd = open("accounts.dat",O_RDWR , 0666);
	struct accountDataBase db;

	while(read(fd, (char *)&db, sizeof(db))){

		if(acc_id==db.acc_id && db.active==1){
			lseek(fd,-sizeof(db),SEEK_CUR);
			lock.l_type=F_WRLCK; //Exclusive lock. 
			lock.l_whence=SEEK_CUR;
			lock.l_start=0; 
			lock.l_len=sizeof(db); //Record Lock
			lock.l_pid=getpid();

			fcntl(fd,F_SETLKW,&lock); 
			db.active=0;
			
			write(fd, (char *)&db, sizeof(db));
			lock.l_type=F_UNLCK; //Release Lock
			flag=1;
			break;
		}
	}

	if(flag){
		char *msg="\nUser Deleted Successfully";
		write(sd,msg,strlen(msg)+1);
	}
	else{
		char *msg="\nNo user matched with given acc_id";
		write(sd,msg,strlen(msg)+1);
	}

	close(fd);
}

void modifyUser(int sd){ //Modifies account type of a user

	displayUserAccountIds(sd);
	int acc_id,flag=0,acc_type;

	read(sd,&acc_id,sizeof(acc_id));
	read(sd,&acc_type,sizeof(acc_type));

	if(acc_type!=1 && acc_type!=2 && acc_type!=3){

		char *msg="Enter Valid Account Type To Modify\n";
		write(sd,msg,strlen(msg)+1);
		return;
	}

	int fd = open("accounts.dat",O_RDWR , 0666);
	struct accountDataBase db;

	while(read(fd, (char *)&db, sizeof(db))){
		if(acc_id==db.acc_id && db.active==1 && db.acc_type!=3){

			lseek(fd,-sizeof(db),SEEK_CUR);
			lock.l_type=F_WRLCK; //Exclusive lock. 
			lock.l_whence=SEEK_CUR;
			lock.l_start=0; 
			lock.l_len=sizeof(db); //Record Lock
			lock.l_pid=getpid();
			fcntl(fd,F_SETLKW,&lock); 

			db.acc_type=acc_type;
			write(fd, (char *)&db, sizeof(db));
			flag=1;
			break;
		}
	}
	if(flag){
		char *msg="User Modified Successfully\n";
		write(sd,msg,strlen(msg)+1);
	}
	else{
		char *msg="No user matched with given acc_id\n";
		write(sd,msg,strlen(msg)+1);
	}
	lock.l_type=F_UNLCK; //Release Lock
	close(fd);
}

void searchUser(int sd){

	int acc_id,flag=0;

	read(sd,&acc_id,sizeof(acc_id));

	int fd = open("accounts.dat",O_RDONLY , 0666);
	struct accountDataBase db;
	while(read(fd, (char *)&db, sizeof(db))){
		if(acc_id==db.acc_id && db.active==1){
			flag=1;
			break;
		}
	}
	close(fd);
	if(flag){
		char *msg="User Found with given acc_id\n";
		write(sd,msg,strlen(msg)+1);
	}
	else{
		char *msg="No user matched with given acc_id\n";
		write(sd,msg,strlen(msg)+1);
	}
}

int checkTrainExists(int train_id){

	int fd = open("trains.dat", O_CREAT | O_RDONLY , 0666);

	struct trainDataBase db;
	while(read(fd, (char *)&db, sizeof(db))){

		if(db.train_id==train_id && db.active==1){
			close(fd);
			return 1;
		}
	}
	close(fd);
	return 0;	
	
}

void addTrain(int sd){

	displayTrains(sd);
	int fd = open("trains.dat", O_APPEND | O_RDWR , 0666);

	lock.l_type=F_WRLCK; //Exclusive lock. 
    lock.l_whence=SEEK_SET;
    lock.l_start=0; 
    lock.l_len=0; //Mandatory Lock
    lock.l_pid=getpid();

    fcntl(fd,F_SETLKW,&lock); 

	int train_id,flag=1;
	read(sd,&train_id,sizeof(train_id));

	if(checkTrainExists(train_id)){
		flag=0;
	}
	write(sd,&flag,sizeof(flag));

	if(flag){
		struct trainDataBase db;
		db.train_id=train_id;
		db.ticket_no=1;
		db.active=1;
		read(sd,db.train_name,sizeof(db.train_name));
		read(sd,db.src,sizeof(db.src));
		read(sd,db.dest,sizeof(db.dest));
		read(sd,&db.seats,sizeof(db.seats));

		write(fd,&db,sizeof(db));

		char* msg="Train added Successfully\n";
		write(sd,msg,strlen(msg)+1);
	}
	else{
		char* msg="Train with given train_id already exists\n";
		write(sd,msg,strlen(msg)+1);
	}
	lock.l_type=F_UNLCK; //Release Lock
	close(fd);
}

void deleteTrain(int sd){

	displayTrains(sd);

	int train_id,flag=0;

	read(sd,&train_id,sizeof(train_id));

	int fd = open("trains.dat",O_RDWR , 0666);
	struct trainDataBase db;

	while(read(fd, (char *)&db, sizeof(db))){
		if(train_id==db.train_id && db.active==1){

			lseek(fd,-sizeof(db),SEEK_CUR);
			lock.l_type=F_WRLCK; //Exclusive lock. 
			lock.l_whence=SEEK_CUR;
			lock.l_start=0; 
			lock.l_len=sizeof(db); //Record Lock
			lock.l_pid=getpid();

			fcntl(fd,F_SETLKW,&lock); 

			db.active=0;
			write(fd, (char *)&db, sizeof(db));
			lock.l_type=F_UNLCK; //Release Lock
			flag=1;
			break;
		}
	}
	if(flag){
		char *msg="Train Deleted Successfully\n";
		write(sd,msg,strlen(msg)+1);
	}
	else{
		char *msg="No train matched with given train_id\n";
		write(sd,msg,strlen(msg)+1);
	}
	
	close(fd);
}

void modifyTrain(int sd){

	displayTrains(sd);

	int train_id,seats,flag=0,cnt=0;

	read(sd,&train_id,sizeof(train_id));
	read(sd,&seats,sizeof(seats));

	int fd = open("trains.dat",O_RDWR , 0666);
	struct trainDataBase db;

	while(read(fd, (char *)&db, sizeof(db))){
		if(train_id==db.train_id && db.active==1){

			lseek(fd,-sizeof(db),SEEK_CUR);
			lock.l_type=F_WRLCK; //Exclusive lock. 
			lock.l_whence=SEEK_CUR;
			lock.l_start=0; 
			lock.l_len=sizeof(db); //Record Lock
			lock.l_pid=getpid();

			fcntl(fd,F_SETLKW,&lock); 
			
			db.seats+=seats; //Increasing the capacity of train
			write(fd, (char *)&db, sizeof(db));
			flag=1;
			break;
		}
	}
	if(flag){
		char* msg="Train Modified Successfully\n";
		write(sd,msg,strlen(msg)+1);
	}
	else{
		char* msg="No train matched with given train_id\n";
		write(sd,msg,strlen(msg)+1);
	}
	lock.l_type=F_UNLCK; //Release Lock
	close(fd);
}

void searchTrain(int sd){
	
	int train_id,flag=0;

	read(sd,&train_id,sizeof(train_id));

	int fd = open("trains.dat",O_RDONLY , 0666);
	struct trainDataBase db;

	while(read(fd, (char *)&db, sizeof(db))){
		if(train_id==db.train_id && db.active==1){
			flag=1;
			break;
		}
	}
	close(fd);
	if(flag){
		char *msg="Train Found with given train_id\n";
		write(sd,msg,strlen(msg)+1);
	}
	else{
		char *msg="No train matched with given train_id\n";
		write(sd,msg,strlen(msg)+1);
	}
}

void makeLogEntry(int train_id,int seats,struct accountDataBase user){

	struct flock lock;
	int fd = open("bookings.dat",O_CREAT|O_RDWR , 0666);

	struct bookingDataBase db;
	db.acc_id=user.acc_id;
	strcpy(db.passenger_name,user.name);
	strcpy(db.passenger_aadhar,user.aadhar);
	db.train_id=train_id;
	db.seats_booked=seats;
	db.active=1;
	time(&db.lastModified);

	lseek(fd,0,SEEK_END);
	write(fd,&db,sizeof(db));
	close(fd);
}

int validateAndBook(int train_id,int seats,struct accountDataBase user){

	int fd = open("trains.dat",O_RDWR , 0666);
	struct trainDataBase db;
	if(seats<0)return -1;
	while(read(fd, (char *)&db, sizeof(db))){

		if(train_id==db.train_id && db.active==1){ 

			lseek(fd,-sizeof(db),SEEK_CUR); //Adjust the file pointer to over ride existing data
			lock.l_type=F_WRLCK; //Exclusive lock. 
		    lock.l_whence=SEEK_CUR;
		    lock.l_start=0;
		    lock.l_len=sizeof(db); //Lock 1 record
		    lock.l_pid=getpid();

			if(seats<=db.seats){

				fcntl(fd,F_SETLKW,&lock); //Record Lock
				db.seats-=seats;
				write(fd, (char *)&db, sizeof(db));
				lock.l_type=F_UNLCK; //Release lock
				close(fd);
				makeLogEntry(train_id,seats,user);
				return 1;
			}
			else return -1;
		}
	}
	close(fd);
	return 0;
}
void displayTrains(int sd){

	int fd = open("trains.dat",O_RDWR , 0666);
	int flag=1;
	struct trainDataBase db;

	lock.l_type=F_RDLCK; //Shared lock. 
    lock.l_whence=SEEK_SET;
    lock.l_start=0; 
    lock.l_len=0; //Mandatory Lock
    lock.l_pid=getpid();

    fcntl(fd,F_SETLKW,&lock); 

	while(read(fd, (char *)&db, sizeof(db))){
		if(db.active==1){
			write(sd,&flag,sizeof(flag));
			write(sd,&db,sizeof(db));
		}
	}
	flag=0;
	write(sd,&flag,sizeof(flag));
	lock.l_type=F_UNLCK; //Release Lock
	close(fd);
}

void bookTicket(int sd,struct accountDataBase user){

	displayTrains(sd);
	int train_id,seats,status;

	read(sd,&train_id, sizeof(train_id));
	read(sd,&seats, sizeof(seats));

	status=validateAndBook(train_id,seats,user);
	if(status==1){
			char* msg="Booking Successfull\n";
			write(sd,msg,strlen(msg)+1);
		}
	else if(status==-1){
		char* msg="Enter seats within available range\n";
		write(sd,msg,strlen(msg)+1);
	}
	else{
		char* msg="No train matched with given train_id\n";
		write(sd,msg,strlen(msg)+1);
	}
}

void viewPreviousBookings(int sd, struct accountDataBase user){

	int fd=open("bookings.dat",O_RDONLY);
	struct bookingDataBase db;
	int flag=1;

	while(read(fd, (char *)&db, sizeof(db))){
		
		if(user.acc_id==db.acc_id && db.active==1){ 
			write(sd,&flag,sizeof(flag));
			write(sd,&db,sizeof(db));
		}
	}
	flag=0;
	write(sd,&flag,sizeof(flag));
}

void updateLogEntry(int train_id,int seats,struct accountDataBase user){

	struct flock lock;
	int fd = open("bookings.dat",O_CREAT|O_RDWR , 0666);
	struct bookingDataBase db;
	while(read(fd, (char *)&db, sizeof(db))){
		if(db.train_id==train_id && !strcmp(db.passenger_aadhar,user.aadhar) && db.acc_id==user.acc_id && db.active==1){
			db.seats_booked+=seats;
			time(&db.lastModified);
			lseek(fd,-sizeof(db),SEEK_CUR);
			write(fd, (char *)&db, sizeof(db));
			close(fd);
			return;
		}
	}
	close(fd);
}

int checkBookingExists(int train_id,struct accountDataBase user){

	int fd = open("bookings.dat",O_CREAT|O_RDONLY , 0666);
	struct bookingDataBase db;
	while(read(fd, (char *)&db, sizeof(db))){
		if(db.train_id==train_id && !strcmp(db.passenger_aadhar,user.aadhar) && db.acc_id==user.acc_id && db.active==1){
			return 1;
		}
	}
	close(fd);
	return 0;
}

int validateAndUpdate(int train_id,int seats,struct accountDataBase user){

	if(!checkBookingExists(train_id,user)) return 0;
	if(seats<=0)return -1;
	//For booking more seats
	int fd = open("trains.dat",O_RDWR , 0666);
		struct trainDataBase db;

		while(read(fd, (char *)&db, sizeof(db))){

			if(train_id==db.train_id && db.active==1){ 
				lseek(fd,-sizeof(db),SEEK_CUR); //Adjust the file pointer 
				lock.l_type=F_WRLCK; //Exclusive lock. 
			    lock.l_whence=SEEK_CUR;
			    lock.l_start=0;
			    lock.l_len=sizeof(db); //Lock 1 record
			    lock.l_pid=getpid();

				if(seats<=db.seats){
					fcntl(fd,F_SETLKW,&lock); //Record Lock
					db.seats-=seats;
					write(fd, (char *)&db, sizeof(db));
					updateLogEntry(train_id,seats,user);
					lock.l_type=F_UNLCK; //Release Lock
					close(fd);
					return 1;
				}
				else {
					close(fd);
					return -1;
				}
			}
		}
	return 0;
}

void updateBooking(int sd, struct accountDataBase user){
	int train_id,seats,status;

	read(sd,&train_id, sizeof(train_id));
	read(sd,&seats, sizeof(seats));

	status=validateAndUpdate(train_id,seats,user);
	if(status==1){
			char* msg="Updated Successfully\n";
			write(sd,msg,strlen(msg)+1);
		}
	else if(status==-1){
		char* msg="Enter valid no.of seats\n";
		write(sd,msg,strlen(msg)+1);
	}
	else{
		char* msg="No booking matched with given train_id\n";
		write(sd,msg,strlen(msg)+1);
	}
}

void updateTrainSeats(int train_id, int seats){

	int fd = open("trains.dat",O_RDWR , 0666);
	struct trainDataBase db;

	while(read(fd, (char *)&db, sizeof(db))){
		if(train_id==db.train_id && db.active==1){
			lseek(fd,-sizeof(db),SEEK_CUR); //Adjust the file pointer 
			lock.l_type=F_WRLCK; //Exclusive lock. 
		    lock.l_whence=SEEK_CUR;
		    lock.l_start=0;
		    lock.l_len=sizeof(db); //Lock 1 record
		    lock.l_pid=getpid();
		    fcntl(fd,F_SETLKW,&lock); //Record Lock

			db.seats+=seats;
			write(fd, (char *)&db, sizeof(db));

			lock.l_type=F_UNLCK; //Release Lock
			close(fd);
			return ;
		}
	}
}

int validateAndCancel(int train_id, struct accountDataBase user){
	int fd = open("bookings.dat",O_RDWR);
	struct bookingDataBase db;
	while(read(fd, (char *)&db, sizeof(db))){
		if(db.train_id==train_id && !strcmp(db.passenger_aadhar,user.aadhar)  && db.acc_id==user.acc_id && db.active==1){
			db.active=0;
			lseek(fd,-sizeof(db),SEEK_CUR);
			write(fd, (char *)&db, sizeof(db));
			updateTrainSeats(train_id,db.seats_booked);
			close(fd);
			return 1;
		}
	}
	close(fd);
	return 0;
}

void cancelBooking(int sd, struct accountDataBase user){
	int train_id;

	read(sd,&train_id, sizeof(train_id));

	int status=validateAndCancel(train_id,user);
	if(status==1){
			char* msg="Cancelled Successfully\n";
			write(sd,msg,strlen(msg)+1);
	}
	else{
		char* msg="No booking matched\n";
		write(sd,msg,strlen(msg)+1);
	}
}

void bookTicketAgent(int sd,struct accountDataBase agent){

	int fd = open("accounts.dat",O_RDWR , 0666);
	struct accountDataBase db;

	while(read(fd, (char *)&db, sizeof(db))){

		if(agent.acc_id==db.acc_id && db.active==1 && db.acc_type==2){
			break;
		}
	}

	lseek(fd,-sizeof(db),SEEK_CUR);
	accountlock.l_type=F_WRLCK; //Exclusive lock. 
    accountlock.l_whence=SEEK_CUR;
    accountlock.l_start=0;
    accountlock.l_len=sizeof(db); //Lock 1 record
    accountlock.l_pid=getpid();

    fcntl(fd,F_SETLKW,&accountlock); //Lock to prevent booking or modifying from other sessions

	displayTrains(sd);

	int train_id,seats,status;

	read(sd,&agent.name, sizeof(agent.name));
	read(sd,&agent.aadhar, sizeof(agent.aadhar));
	read(sd,&train_id, sizeof(train_id));
	read(sd,&seats, sizeof(seats));

	status=validateAndBook(train_id,seats,agent);
	if(status==1){
			char* msg="Booking Successfull\n";
			write(sd,msg,strlen(msg)+1);
		}
	else if(status==-1){
		char* msg="Enter seats within available range\n";
		write(sd,msg,strlen(msg)+1);
	}
	else{
		char* msg="No train matched with given train_id\n";
		write(sd,msg,strlen(msg)+1);
	}
	accountlock.l_type=F_UNLCK; //Release Lock
	close(fd);
}

void viewPreviousBookingsAgent(int sd, struct accountDataBase agent){

	int fd1 = open("accounts.dat",O_RDWR , 0666);
	struct accountDataBase db1;

	while(read(fd1, (char *)&db1, sizeof(db1))){

		if(agent.acc_id==db1.acc_id && db1.active==1 && db1.acc_type==2){
			break;
		}
	}

	lseek(fd1,-sizeof(db1),SEEK_CUR);
	accountlock.l_type=F_RDLCK; //Shared lock. 
    accountlock.l_whence=SEEK_CUR;
    accountlock.l_start=0;
    accountlock.l_len=sizeof(db1); //Lock 1 record
    accountlock.l_pid=getpid();

    fcntl(fd1,F_SETLKW,&accountlock); //Allow only view previous bookings from  other sessions if any

	int fd2=open("bookings.dat",O_RDONLY);

	struct bookingDataBase db2;
	int flag=1;

	while(read(fd2, (char *)&db2, sizeof(db2))){
		
		if(agent.acc_id==db2.acc_id && db2.active==1){ 
			write(sd,&flag,sizeof(flag));
			write(sd,&db2,sizeof(db2));
		}
	}
	flag=0;
	write(sd,&flag,sizeof(flag));

	accountlock.l_type=F_UNLCK; //Release Lock
	close(fd1);
	close(fd2);
}

void updateBookingAgent(int sd, struct accountDataBase agent){

	int fd = open("accounts.dat",O_RDWR , 0666);
	struct accountDataBase db;

	while(read(fd, (char *)&db, sizeof(db))){

		if(agent.acc_id==db.acc_id && db.active==1 && db.acc_type==2){
			break;
		}
	}

	lseek(fd,-sizeof(db),SEEK_CUR);
	accountlock.l_type=F_WRLCK; //Exclusive lock. 
    accountlock.l_whence=SEEK_CUR;
    accountlock.l_start=0;
    accountlock.l_len=sizeof(db); //Lock 1 record
    accountlock.l_pid=getpid();

    fcntl(fd,F_SETLKW,&accountlock); //Lock to prevent booking or modifying from other sessions

	int train_id,seats,status;

	read(sd,&agent.aadhar, sizeof(agent.aadhar));
	read(sd,&train_id, sizeof(train_id));
	read(sd,&seats, sizeof(seats));

	status=validateAndUpdate(train_id,seats,agent);
	if(status==1){
			char* msg="Updated Successfully\n";
			write(sd,msg,strlen(msg)+1);
		}
	else if(status==-1){
		char* msg="Enter valid no.of seats\n";
		write(sd,msg,strlen(msg)+1);
	}
	else{
		char* msg="No booking matched with given train_id and passenger_aadhar number\n";
		write(sd,msg,strlen(msg)+1);
	}
	accountlock.l_type=F_UNLCK; //Release Lock
	close(fd);
}

void cancelBookingAgent(int sd, struct accountDataBase agent){

	int fd = open("accounts.dat",O_RDWR , 0666);
	struct accountDataBase db;

	while(read(fd, (char *)&db, sizeof(db))){

		if(agent.acc_id==db.acc_id && db.active==1 && db.acc_type==2){
			break;
		}
	}

	lseek(fd,-sizeof(db),SEEK_CUR);
	accountlock.l_type=F_WRLCK; //Exclusive lock. 
    accountlock.l_whence=SEEK_CUR;
    accountlock.l_start=0;
    accountlock.l_len=sizeof(db); //Lock 1 record
    accountlock.l_pid=getpid();

    fcntl(fd,F_SETLKW,&accountlock); //Lock to prevent booking or modifying from other sessions

	int train_id,status;

	read(sd,&agent.aadhar, sizeof(agent.aadhar));
	read(sd,&train_id, sizeof(train_id));

	status=validateAndCancel(train_id,agent);
	if(status==1){
			char* msg="Cancelled Successfully\n";
			write(sd,msg,strlen(msg)+1);
		}
	else if(status==-1){
		char* msg="Enter valid no.of seats\n";
		write(sd,msg,strlen(msg)+1);
	}
	else{
		char* msg="No booking matched with given train_id and passenger_aadhar number\n";
		write(sd,msg,strlen(msg)+1);
	}
	accountlock.l_type=F_UNLCK; //Release Lock
	close(fd);
}

void displayUserMenu(int sd, struct accountDataBase user){
	int choice;
	read(sd, &choice, sizeof(choice));
	switch(choice){

		case 1: bookTicket(sd,user); 
				break;
		case 2: viewPreviousBookings(sd,user);
				break;
		case 3: updateBooking(sd,user);
				break;
		case 4: cancelBooking(sd,user);
				break;
		case 5: accountlock.l_type=F_UNLCK; //Release lock
				exit(0);
				break;
	}
	displayUserMenu(sd,user);
}

void displayAgentMenu(int sd, struct accountDataBase agent){
	int choice;
	read(sd, &choice, sizeof(choice));
	switch(choice){

		case 1: bookTicketAgent(sd,agent); 
				break;
		case 2: viewPreviousBookingsAgent(sd,agent);
				break;
		case 3: updateBookingAgent(sd,agent);
				break;
		case 4: cancelBookingAgent(sd,agent);
				break;
		case 5: exit(0);
				break;
	}
	displayAgentMenu(sd,agent);
}

void displayAdminMenu(int sd, struct accountDataBase admin){
	int choice;
	read(sd, &choice, sizeof(choice));
	switch(choice){
		case 1: newUser(sd); 
				break;
		case 2: deleteUser(sd);
				break;
		case 3: modifyUser(sd);
				break;
		case 4: searchUser(sd);
				break;
		case 5: addTrain(sd);
				break;
		case 6: deleteTrain(sd);
				break;
		case 7: modifyTrain(sd);
				break;
		case 8: searchTrain(sd);
				break;
		case 9: accountlock.l_type=F_UNLCK; //Release lock
				exit(0);
				break;
	}
	displayAdminMenu(sd,admin);
}

void userLogin(int sd){
	char username[30],password[20];
	int status=0;

	read(sd,username,sizeof(username));
	read(sd,password,sizeof(password));

	int fd = open("accounts.dat",O_RDWR , 0666);
	struct accountDataBase db;

	while(read(fd, (char *)&db, sizeof(db))){

		if(!strcmp(db.username, username) && !strcmp(db.password, password) && db.active==1 && db.acc_type==1){
			status=1;
			break;
		}
	}

	write(sd,&status,sizeof(status));

	if(status){
		lseek(fd,-sizeof(db),SEEK_CUR);
		accountlock.l_type=F_WRLCK; //Exclusive lock. 
	    accountlock.l_whence=SEEK_CUR;
	    accountlock.l_start=0;
	    accountlock.l_len=sizeof(db); //Lock 1 record
	    accountlock.l_pid=getpid();

	    int retval=fcntl(fd,F_SETLK,&accountlock); //Lock to prevent multiple logins of same user

	    write(sd,&retval,sizeof(retval));

	    if(retval==-1){
	    	char *msg="You are already logged in!\n";
			write(sd,msg,strlen(msg)+1);
			mainMenu(sd);
	    }
		else{
			char *msg="\n--------Welcome User---------";
			write(sd,msg,strlen(msg)+1);
			displayUserMenu(sd,db);
		}
	}
	else{
		char *msg="\nInvalid Credentials";
		write(sd,msg,strlen(msg)+1);
		mainMenu(sd);
	}
}  

void agentLogin(int sd){

	char username[30],password[20];
	int status=0;

	read(sd,username,sizeof(username));
	read(sd,password,sizeof(password));

	int fd = open("accounts.dat",O_RDWR , 0666);
	struct accountDataBase db;

	while(read(fd, (char *)&db, sizeof(db))){

		if(!strcmp(db.username, username) && !strcmp(db.password, password) && db.active==1 && db.acc_type==2){
			status=1;
			break;
		}
	}
	write(sd,&status,sizeof(status));

	if(status){
		char *msg="\n--------Welcome Agent---------";
		write(sd,msg,strlen(msg)+1);
		displayAgentMenu(sd,db);
	}
	else{
		char *msg="\nInvalid Credentials";
		write(sd,msg,strlen(msg)+1);
		mainMenu(sd);
	}
}  

void adminLogin(int sd){

	char username[30],password[20];
	int status=0;

	read(sd,username,sizeof(username));
	read(sd,password,sizeof(password));

	int fd = open("accounts.dat",O_RDWR , 0666);
	struct accountDataBase db;

	while(read(fd, (char *)&db, sizeof(db))){

		if(!strcmp(db.username, username) && !strcmp(db.password, password) && db.active==1 && db.acc_type==3){
			status=1;
			break;
		}
	}
	write(sd,&status,sizeof(status));

	if(status){

		lseek(fd,-sizeof(db),SEEK_CUR);
		accountlock.l_type=F_WRLCK; //Exclusive lock. 
	    accountlock.l_whence=SEEK_CUR;
	    accountlock.l_start=0;
	    accountlock.l_len=sizeof(db); //Lock 1 record
	    accountlock.l_pid=getpid();

	    fcntl(fd,F_SETLK,&accountlock); //Lock to prevent multiple logins of same user
		

		int retval=fcntl(fd,F_SETLK,&accountlock); //Lock to prevent multiple logins of same user

		write(sd,&retval,sizeof(retval));
		
	    if(retval==-1){
	    	char *msg="You are already logged in!\n";
			write(sd,msg,strlen(msg)+1);
			mainMenu(sd);
	    }
		else{
			char *msg="\n--------Welcome Admin---------";
			write(sd,msg,strlen(msg)+1);
			displayAdminMenu(sd,db);
		}
	}
	else{
		char *msg="\nInvalid Credentials";
		write(sd,msg,strlen(msg)+1);
		mainMenu(sd);
	}
}

int checkAccountExists(char uname[],char path[]){

	int fd = open(path, O_CREAT | O_RDWR , 0666);

	struct accountDataBase db;
	while(read(fd, (char *)&db, sizeof(db))){

		if(!strcmp(db.username, uname) && db.active==1){
			close(fd);
			return 1;
		}
	}
	close(fd);
	return 0;	
	
}

int addAccount(struct accountDataBase db){

    struct accId id;

	int fd1 = open("accounts.dat", O_APPEND | O_RDWR , 0666);
	int fd2 = open("account_id.dat",O_RDWR);

	lock.l_type=F_WRLCK; //Exclusive lock. 
    lock.l_whence=SEEK_SET;
    lock.l_start=0; 
    lock.l_len=0; //Mandatory Lock
    lock.l_pid=getpid();

    fcntl(fd2,F_SETLKW,&lock); //Waits if lock is unavailable

    read(fd2,&id,sizeof(id));
    db.acc_id=id.acc_id;

    write(fd1,&db,sizeof(db)); //adding account to account Database with unique acc_id

    id.acc_id++; //Increment id.
    
    lseek(fd2,0,SEEK_SET); //As you read from file, fp will be at the end, hence writing directly without setting file pointer will not update the existing ticket no
    write(fd2,&id,sizeof(id)); //Write the changes into database
    
    lock.l_type=F_UNLCK; //Release lock

    close(fd1);
    close(fd2);
	
	return 1;
}

void signup(int sd,int type){

	struct accountDataBase db;
	int status=0;
	read(sd,db.username,sizeof(db.username));
	read(sd,db.password,sizeof(db.password));
	read(sd,db.name,sizeof(db.name));
	read(sd,&db.age,sizeof(db.age));
	db.active=1;
	db.acc_type=type;

	if(!checkAccountExists(db.username,"accounts.dat")){
		addAccount(db);
		status=1;
	}

	write(sd,&status,sizeof(status));

	if(status){
		char *msg="\n--------SIGNUP SUCCESSFULL---------";
		write(sd,msg,strlen(msg)+1);
	}
	else{
		char *msg="\nusername already exits. Try with a different username";
		write(sd,msg,strlen(msg)+1);
	}
	mainMenu(sd);
}

void newUser(int sd){
	int choice;
	read(sd, &choice, sizeof(choice));
	switch(choice){
		case 1: signup(sd,1);
				break;
		case 2: signup(sd,2);
				break;
		case 3: signup(sd,3);
				break;
		case 4: exit(0);
				break;
	}
}

void mainMenu(int sd){
	int choice;
	read(sd, &choice, sizeof(choice));
	switch(choice){
		case 1: userLogin(sd);
				break;
		case 2: agentLogin(sd);
				break;
		case 3: adminLogin(sd);
				break;
		case 4: newUser(sd);
				break;
		case 5: exit(0);
				break;
	}
}