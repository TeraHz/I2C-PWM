/*
 * I2CPWM.cpp
 *
 *  Created on: Dec 9, 2012
 *      Author: terahz
 */
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */
#include <syslog.h>		/* Syslog functionallity */
#include "I2CPWM.h"
#include "PCA9685.h"

I2CPWM::I2CPWM() {
	//do something?
}

int I2CPWM::daemonize(int flags) {
	int maxfd, fd;

	switch (fork()) {					/* Become background process */
	case -1:
		return -1;
	case 0:
		break;							/* Child falls through... */
	default:
		_exit(EXIT_SUCCESS);			/* while parent terminates */
	}

	if (setsid() == -1)					/* Become leader of new session */
		return -1;

	switch (fork()) {					/* Ensure we are not session leader */
	case -1:
		return -1;
	case 0:
		break;
	default:
		_exit(EXIT_SUCCESS);
	}

	if (!(flags & BD_NO_UMASK0))
		umask(0);						/* Clear file mode creation mask */

	if (!(flags & BD_NO_CHDIR))
		chdir("/");						/* Change to root directory */

	if (!(flags & BD_NO_CLOSE_FILES)) { /* Close all open files */
		maxfd = sysconf(_SC_OPEN_MAX);
		if (maxfd == -1)				/* Limit is indeterminate... */
			maxfd = BD_MAX_CLOSE;		/* so take a guess */

		for (fd = 0; fd < maxfd; fd++)
			close(fd);
	}

	if (!(flags & BD_NO_REOPEN_STD_FDS)) {
		close(STDIN_FILENO); 			/* Reopen standard fd's to /dev/null */

		fd = open("/dev/null", O_RDWR);

		if (fd != STDIN_FILENO)			/* 'fd' should be 0 */
			return -1;
		if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
			return -1;
		if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
			return -1;
	}

	PCA9685 driver(3,0x40);
	sleep(10);
	for (int ii = 1; ii < 4095; ii=ii<<1){
			driver.setPWM(1,ii);
			sleep(2);
	}
	for (int ii = 4095; ii > 0; ii=ii>>1){
		driver.setPWM(1,ii);
			sleep(2);
	}
	sleep(5);
	syslog( LOG_INFO, "Stopping I2CPWM Daemon" );
	return 0;

}
int mult_by_pow_2(int number, int power)
{
    return number<<power;
}
int div_by_pow_2(int number, int power)
{
    return number>>power;
}
int main(int argc, char* argv[]) {
	syslog( LOG_INFO, "Starting I2CPWM Daemon" );
	I2CPWM pwm;
	pwm.daemonize(0);

	return (0);
}
