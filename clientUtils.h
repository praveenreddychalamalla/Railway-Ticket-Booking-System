/* @author Praveen Reddy 
 * @date : 2021-10-24
 * @desc Client Utils
 */
#include <fcntl.h>
#include <string.h>
#include "structures.h"
#include <time.h>
#define buffer_size 80

void mainMenu(int);
void signup(int, int);
void newUser(int);
void displayTrains(int);

void displayUserAccountIds(int sd){
	struct accountDataBase db;
	int flag;
	printf("\n-------User Accounts-------\n");
	printf("%10s\t %20s\t\t %s\n","acc_id","username","Name");
	read(sd,&flag,sizeof(flag));
	while(flag){
		read(sd,&db,sizeof(db));
		read(sd,&flag,sizeof(flag));
		printf("%5d\t %30s\t\t %s\n",db.acc_id,db.username,db.name);
	}
}

void deleteUser(int sd){

	displayUserAccountIds(sd);
	char buffer[buffer_size];
	int acc_id;

	printf("\nEnter User Id to delete User : ");
	scanf("%d",&acc_id);

	write(sd,&acc_id,sizeof(acc_id));

	read(sd,buffer,buffer_size);
	printf("%s",buffer);
}

void modifyUser(int sd){
	displayUserAccountIds(sd);

	char buffer[buffer_size];
	int acc_id,type;

	printf("\nEnter Account Id to Modify User : ");
	scanf("%d",&acc_id);
	printf("\nEnter type to modify account type: ");
	scanf("%d",&type);

	write(sd,&acc_id,sizeof(acc_id));
	write(sd,&type,sizeof(type));

	read(sd,buffer,buffer_size);
	printf("%s",buffer);
}

void searchUser(int sd){
	char buffer[buffer_size];
	int acc_id;

	printf("\nEnter Acc Id to Search User : ");
	scanf("%d",&acc_id);

	write(sd,&acc_id,sizeof(acc_id));

	read(sd,buffer,buffer_size);
	printf("%s",buffer);
}

void addTrain(int sd){

	char buffer[buffer_size];
	int train_id,flag;
	displayTrains(sd);
	printf("\nEnter train_id : ");
	scanf("%d",&train_id);

	write(sd,&train_id,sizeof(train_id));

	read(sd,&flag,sizeof(flag));

	if(flag){
		char src[30],dest[30],train_name[30];
		int seats;
		printf("\nEnter Train Name : ");
		scanf("%s",train_name);

		printf("\nEnter Source Station Name: ");
		scanf("%s",src);

		printf("\nEnter Destination Station Name: ");
		scanf("%s",dest);

		printf("\nEnter Seats : ");
		scanf("%d",&seats);

		write(sd,train_name,sizeof(train_name));
		write(sd,src,sizeof(src));
		write(sd,dest,sizeof(dest));
		write(sd,&seats,sizeof(seats));
	}
	read(sd,&buffer,buffer_size);
	printf("\n%s",buffer);
}

void deleteTrain(int sd){

	displayTrains(sd);

	char buffer[buffer_size];
	int train_id;
	printf("\nEnter train_id : ");
	scanf("%d",&train_id);

	write(sd,&train_id,sizeof(train_id));


	read(sd,&buffer,buffer_size);
	printf("\n%s",buffer);
}

void modifyTrain(int sd){

	displayTrains(sd);

	char buffer[buffer_size];
	int train_id,seats,flag;
	printf("\nEnter train_id : ");
	scanf("%d",&train_id);
	printf("\nEnter No. of seats to increase in train: ");
	scanf("%d",&seats);

	write(sd,&train_id,sizeof(train_id));
	write(sd,&seats,sizeof(seats));

	read(sd,&buffer,buffer_size);
	printf("\n%s",buffer);
}

void searchTrain(int sd){

	char buffer[buffer_size];
	int train_id,flag;
	printf("\nEnter train_id : ");
	scanf("%d",&train_id);

	write(sd,&train_id,sizeof(train_id));


	read(sd,&buffer,buffer_size);
	printf("\n%s",buffer);
}

void displayTrains(int sd){
	struct trainDataBase db;
	int flag;
	printf("\n---------Trains List----------\n");
	printf("%5s\t %20s\t %5s\t %20s\t %20s\t\n","train_id","train_name","Seats_Available","Source","Destination");
	read(sd,&flag,sizeof(flag));
	while(flag){
		read(sd,&db,sizeof(db));
		read(sd,&flag,sizeof(flag));
		printf("%5d\t\t %20s\t\t %5d\t\t %20s\t %20s\t\n",db.train_id,db.train_name,db.seats,db.src,db.dest);
	}
}

void bookTicket(int sd){

	displayTrains(sd);

	char buffer[buffer_size];
	int train_id,seats;

	printf("\nEnter train_id : ");
	scanf("%d",&train_id);
	printf("\nEnter No.of seats : ");
	scanf("%d",&seats);

	write(sd,&train_id, sizeof(train_id));
	write(sd,&seats, sizeof(seats));

	read(sd,&buffer,buffer_size);
	printf("\n%s",buffer);
}

void viewPreviousBookings(int sd){

	struct bookingDataBase db;
	int flag;
	printf("\n-------Previous Bookings-------\n");
	printf("%6s\t %6s\t %22s\n","TrainId","Seats_Booked","Last Modified");
	read(sd,&flag,sizeof(flag));
	while(flag){
		read(sd,&db,sizeof(db));
		read(sd,&flag,sizeof(flag));
		char* t1=ctime(&db.lastModified);
		printf("%5d\t %5d\t\t %s",db.train_id,db.seats_booked,t1);
	}
}

void updateBooking(int sd){
	int train_id,seats;
	char buffer[buffer_size];

	printf("\nEnter train_id to update booked seats : ");
	scanf("%d",&train_id);
	printf("\nEnter no.of seats : ");
	scanf("%d",&seats);

	write(sd,&train_id, sizeof(train_id));
	write(sd,&seats, sizeof(seats));

	read(sd,buffer,buffer_size);
	printf("\n%s",buffer);
}

void cancelBooking(int sd){
	int train_id;
	char buffer[buffer_size];

	printf("\nEnter train_id to cancel : ");
	scanf("%d",&train_id);

	write(sd,&train_id, sizeof(train_id));

	read(sd,buffer,buffer_size);
	printf("\n%s",buffer);
}

void bookTicketAgent(int sd){

	displayTrains(sd);

	char buffer[buffer_size];
	int train_id,seats;
	char aadhar[12],name[30];

	printf("\nEnter passenger name : ");
	scanf(" %[^\n]",name);
	printf("\nEnter passenger_aadhar number : ");
	scanf("%s",aadhar);
	printf("\nEnter train_id : ");
	scanf("%d",&train_id);
	printf("\nEnter No.of seats : ");
	scanf("%d",&seats);

	write(sd,&name, sizeof(name));
	write(sd,&aadhar, sizeof(aadhar));
	write(sd,&train_id, sizeof(train_id));
	write(sd,&seats, sizeof(seats));

	read(sd,&buffer,buffer_size);
	printf("\n%s",buffer);
}

void viewPreviousBookingsAgent(int sd){

	struct bookingDataBase db;
	int flag;
	printf("\n-------Previous Bookings-------\n");
	printf(" %12s\t %6s\t %6s\t %22s\n","Passenger's Aadhar_no","TrainId", "Seats_Booked","Last Modified");
	read(sd,&flag,sizeof(flag));
	while(flag){
		read(sd,&db,sizeof(db));
		read(sd,&flag,sizeof(flag));
		char* t1=ctime(&db.lastModified);
		printf("%12s\t\t %5d\t\t %5d\t\t %s",db.passenger_aadhar,db.train_id,db.seats_booked,t1);
	}
}

void updateBookingAgent(int sd){
	int train_id,seats;
	char buffer[buffer_size];
	char aadhar[12];

	printf("\nEnter passenger's aadhar_no to update booking : ");
	scanf("%s",aadhar);
	printf("\nEnter train_id to update booked seats : ");
	scanf("%d",&train_id);
	printf("\nEnter no.of seats : ");
	scanf("%d",&seats);

	write(sd,&aadhar, sizeof(aadhar));
	write(sd,&train_id, sizeof(train_id));
	write(sd,&seats, sizeof(seats));

	read(sd,buffer,buffer_size);
	printf("\n%s",buffer);
}

void cancelBookingAgent(int sd){

	int train_id;
	char buffer[buffer_size];
	char aadhar[12];

	printf("\nEnter passenger's aadhar_no to update booking : ");
	scanf("%s",aadhar);
	printf("\nEnter train_id to update booked seats : ");
	scanf("%d",&train_id);

	write(sd,&aadhar, sizeof(aadhar));
	write(sd,&train_id, sizeof(train_id));

	read(sd,buffer,buffer_size);
	printf("\n%s",buffer);
}

void displayUserMenu(int sd){ 
	int choice;

	printf("\n1.BOOK TICKET\n2.VIEW PREVIOUS BOOKINGS\n3.UPDATE BOOKING\n4.CANCEL BOOKING\n5.EXIT\n");
	printf("Enter your choice: ");
	scanf("%d",&choice);

	write(sd, &choice, sizeof(choice));
	switch(choice){
		case 1: bookTicket(sd); 
				break;
		case 2: viewPreviousBookings(sd);
				break;
		case 3: updateBooking(sd);
				break;
		case 4: cancelBooking(sd);
				break;
		case 5: exit(0);
				break;
	}
	displayUserMenu(sd);
}
void displayAgentMenu(int sd){
	int choice;

	printf("\n1.BOOK TICKET\n2.VIEW PREVIOUS BOOKINGS\n3.UPDATE BOOKING\n4.CANCEL BOOKING\n5.EXIT\n");
	printf("Enter your choice: ");
	scanf("%d",&choice);

	write(sd, &choice, sizeof(choice));
	switch(choice){
		case 1: bookTicketAgent(sd); 
				break;
		case 2: viewPreviousBookingsAgent(sd);
				break;
		case 3: updateBookingAgent(sd);
				break;
		case 4: cancelBookingAgent(sd);
				break;
		case 5: exit(0);
				break;
	}
	displayAgentMenu(sd);
}

void displayAdminMenu(int sd){

	int choice;

	printf("\n1.ADD USER\n2.DELETE USER\n3.MODIFY USER\n4.SEARCH USER\n5.ADD TRAIN\n6.DELETE TRAIN\n7.MODIFY TRAIN\n8.SEARCH TRAIN\n9.EXIT\n");
	printf("Enter your choice: ");
	scanf("%d",&choice);

	write(sd, &choice, sizeof(choice));
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
		case 9: exit(0);
				break;
	}
	displayAdminMenu(sd);
}

void userLogin(int sd){

	printf("\nWelcome to User Login");
	char username[30],password[20],buffer[buffer_size];
	int status=0;

	printf("\nEnter username : ");
	scanf(" %[^\n]",username);

	printf("\nEnter password : ");
	scanf(" %[^\n]",password);

	write(sd,username,sizeof(username));
	write(sd,password,sizeof(password));

	read(sd,&status,sizeof(status));

	if(status){
		read(sd,&status,sizeof(status));
		if(status==-1){
			read(sd,buffer,buffer_size);
			printf("\n%s",buffer);
			mainMenu(sd);
		}
		else{
			read(sd,buffer,buffer_size);
			printf("\n%s",buffer);
			displayUserMenu(sd);
		}
		
	}
	else{
		read(sd,buffer,buffer_size);
		printf("\n%s",buffer);
		mainMenu(sd);
	}
}  
void agentLogin(int sd){

	printf("\nWelcome to Agent Login");
	char username[30],password[20],buffer[buffer_size];
	int status=0;

	printf("\nEnter username : ");
	scanf(" %[^\n]",username);

	printf("\nEnter password : ");
	scanf(" %[^\n]",password);

	write(sd,username,sizeof(username));
	write(sd,password,sizeof(password));

	read(sd,&status,sizeof(status));

	read(sd,buffer,buffer_size);
	printf("\n%s",buffer);

	if(status){
		displayAgentMenu(sd);
	}
	else{
		mainMenu(sd);
	}
}  

void adminLogin(int sd){

	printf("\nWelcome to Admin Login");
	char username[30],password[20],buffer[buffer_size];
	int status=0;

	printf("\nEnter admin username : ");
	scanf(" %[^\n]",username);

	printf("\nEnter admin password : ");
	scanf(" %[^\n]",password);

	write(sd,username,sizeof(username));
	write(sd,password,sizeof(password));

	read(sd,&status,sizeof(status));

	if(status){

		read(sd,&status,sizeof(status));

		if(status==-1){
			read(sd,buffer,buffer_size);
			printf("\n%s",buffer);
			mainMenu(sd);
		}
		else{
			read(sd,buffer,buffer_size);
			printf("\n%s",buffer);
			displayAdminMenu(sd);
		}
		
	}
	else{
		read(sd,buffer,buffer_size);
		printf("\n%s",buffer);
		mainMenu(sd);
	}
}

void signup(int sd,int type){

	char username[30],password[20],name[30], buffer[buffer_size];
	int status=0,age;

	printf("\nEnter username : ");
	scanf(" %[^\n]",username);

	printf("\nEnter password : ");
	scanf(" %[^\n]",password);

	printf("\nEnter Name : ");
	scanf(" %[^\n]",name);

	printf("\nEnter Age : ");
	scanf("%d",&age);


	write(sd,username,sizeof(username));
	write(sd,password,sizeof(password));
	write(sd,name,sizeof(name));
	write(sd,&age,sizeof(age));

	read(sd,&status,sizeof(status));

	read(sd,buffer,buffer_size);
	printf("\n%s",buffer);
	
	mainMenu(sd);
}

void newUser(int sd){
	int choice;

	printf("\n1.USER SIGNUP\n2.AGENT SIGNUP\n3.ADMIN SIGNUP\n4.EXIT\n");
	printf("Enter your choice: ");
	scanf("%d",&choice);

	write(sd, &choice, sizeof(choice));
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

	printf("\n1.USER LOGIN\n2.AGENT LOGIN\n3.ADMIN LOGIN\n4.SIGNUP\n5.EXIT\n");
	printf("Enter your choice: ");
	scanf("%d",&choice);

	write(sd, &choice, sizeof(choice));
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