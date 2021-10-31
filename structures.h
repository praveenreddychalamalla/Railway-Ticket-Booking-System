/* @author Praveen Reddy 
 * @date : 2021-10-24
 * @desc Strutures for user,train and booking
 */
#include <time.h>
struct accountDataBase{
	char username[30];
	char password[20];
	int acc_id;
	int acc_type;
	char name[30];
	char aadhar[12];
	int age;
	int active;
};
struct trainDataBase{
	int train_id;
	char train_name[30];
	char src[30];
	char dest[30];
	int ticket_no;
	int seats;
	int active;
};

struct bookingDataBase{
	int acc_id;
	char passenger_name[30];
	char passenger_aadhar[12];
	int train_id;
	int seats_booked;
	int active;
	time_t lastModified;
};

struct accId{
    int acc_id;
};
