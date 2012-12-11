/*
 * I2CPWM.cpp
 *
 *  Created on: Dec 9, 2012
 *      Author: terahz
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "I2CPWM.h"
#include "PCA9685.h"

I2CPWM::I2CPWM() {
	//do something?
}

void I2CPWM::daemonize() {
	pid_t pid = 0;
	pid_t sid = 0;
	FILE *fp = NULL;
	int i = 0;
	pid = fork(); // fork a new child process

	if (pid < 0) {
		printf("fork failed!\n");
		exit(1);
	}

	if (pid > 0) // its the parent process
			{
		printf("pid of child process %d \n", pid);
		exit(0); //terminate the parent process successfully
	}

	umask(0); //unmasking the file mode

	sid = setsid(); //set new session
	if (sid < 0) {
		exit(1);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	PCA9685 pwm(3,0x40);
	fp = fopen("test.txt", "w+");
	while (i < 10) {
		sleep(1);
		fprintf(fp, "%d\n", i);
		i++;
	}
	fclose(fp);

}

int main(int argc, char* argv[]) {
	I2CPWM pwm;
	pwm.daemonize();

	return (0);
}
