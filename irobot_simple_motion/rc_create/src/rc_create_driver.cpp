#include "ros/ros.h"
#include "rc_create/Control.h"
#include "std_srvs/Empty.h"
using namespace std;

/*new code*/

#include <iostream>
#include <unistd.h> // UNIX standard function definitions
#include <fcntl.h> // File control definitions
#include <errno.h>
#include <termios.h>
#include <time.h>   // time calls

/* Define the serial port path */
#define CREATE_SERIAL_PORT "/dev/ttyUSB0"
/* Define baudrate */
#define CREATE_SERIAL_BRATE B115200

static int fd;


int create_init() {
////////////////
// Initialize //
////////////////

fd = open(CREATE_SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY );


// can't open port
if (fd == -1) {
  cout << "Error opening port\n";
  return -1;
}
// open successful
else {
  cout << "Serial port opened with status: " << fd << endl;
  fcntl(fd, F_SETFL, 0);
}
// configure port
struct termios portSettings;
tcgetattr(fd, &portSettings);


if(cfsetispeed(&portSettings, CREATE_SERIAL_BRATE) != 0)
  cout << "Failed setting baud rate for input";
if(cfsetospeed(&portSettings, CREATE_SERIAL_BRATE) != 0)
  cout << "Failed setting baud rate for output";


//set parity bits
portSettings.c_cflag &= ~PARENB;
portSettings.c_cflag &= ~CSTOPB;
portSettings.c_cflag &= ~CSIZE;
portSettings.c_cflag |= CS8;
cfmakeraw(&portSettings);


if(tcsetattr(fd, TCSANOW, &portSettings) != 0) {
  cout << "Failed pushing port settings.\n";
  return fd;
}

// Running iRobot create

char initsafe[] = {128, 131};
char initfull[] = {128, 132};  

int wd;
wd = write(fd, initfull, 8);
wd = write(fd, initfull, 8);
wd = write(fd, initfull, 8);
}


void forward() {
	
char forward[] = {137, 0, 100, 128, 0 };
int wd;
wd =  write(fd, forward, 8);
cout << "FORWARD Written";

}

void backward() {
	
char forward[] = {137, 255, 100, 128, 0 };
int wd;
wd =  write(fd, forward, 8);
cout << "BACKWARD Written";

}


void stop() {
	
char forward[] = {137, 0, 0, 0, 0 };
int wd;
wd =  write(fd, forward, 8);
cout << "STOP Written";

}

void right() {
	
char forward[] = {137, 0, 100, 255, 244 };
int wd;
wd =  write(fd, forward, 8);
cout << "RIGHT Written";

}

void left() {
	
char forward[] = {137, 255, 100, 255, 244 };
int wd;
wd =  write(fd, forward, 8);
cout << "LEFT Written";

}

void song() {
	
char forward[] = {140,0,2,83,64,86,64};
int wd;
wd =  write(fd, forward, 8);
cout << "SONG Written";
sleep(0.5);
char forward2[] = {141,0};
int wd2;
wd2 =  write(fd, forward2, 8);

}


/* new code ends*/

std::string mode = "D";

bool exitCallback(std_srvs::Empty::Request &req,
                  std_srvs::Empty::Response &res)
{
        ros::shutdown();

        return true;
}

/******************************
 *
 * SETUP MODE CALLBACK HERE.
 *
 ******************************/
bool modeCallback(rc_create::Control::Request  &req,
                  rc_create::Control::Response &res)
{

        mode = req.mode;


	if (mode == "I")
	{
		create_init();
		forward();
	}
	if (mode == "K")
	{
		create_init();
		backward();
	}
	if (mode == "J")
	{
		create_init();
		left();
	}
	if (mode == "L")
	{
		create_init();
		right();
	}
	if (mode == "D")
	{
		create_init();
		stop();
	}
	if (mode == "S")
	{
		create_init();
		song();
	}
        


        res.success = bool(true);

//	ImageConverter ic;
        return true;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "rc_create_driver");
  ros::NodeHandle node;
  
  ros::ServiceServer exitServer = node.advertiseService("exit", exitCallback);

        /******************************
         *
         * SETUP THE MODE SERVER HERE.
         *
         ******************************/
  ros::ServiceServer modeServer = node.advertiseService("mode", modeCallback);


  //ImageConverter ic;
  ros::spin();
  return 0;
}
