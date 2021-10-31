# Railway Ticket Booking System

@author Praveen Reddy (MT2021033) 

@date : 2021-10-30


This project uses tcp connection to communicate between client and server.

client.c        : This is client side connection file which represents the client operations.
server.c        : This is server side connection file which fetches data from client.c, perform subsequent operations on it 
                  and communicate the response to client.

clientUtils.h   : This file contains all client side functions. This is included in client.c as header file.
serverUtils.h   : This file contains all server side functions. This is included in server.c as header file.
structures.h    : This file contains all strutures for user(all types), train and booking

accounts.dat    : This file contains account related information of users(Normal User,Agent, Admin).
account_id.dat  : This file contains the last alloted account_id to the user. This helps in generating unique account ids.
acc_id.c        : This creates the account_id.dat file and initialize the account_id to 1.

bookings.dat    : This file contains all the booking logs.

trains.dat      : This file maintains information related to all trains.

Port No. used   : 9586 (you can change the port of server and client)

run the server:
    >./server.out

run the client:
    >./client.out

First sign up and create one admin account. 
The secret pin for admin account is "secret".

Now login into admin account and add trains.

Create customers/agents and login to book ticket.

Multiple login sessions are allowed only for agent accounts. Proper locking mechanism was implemented.


