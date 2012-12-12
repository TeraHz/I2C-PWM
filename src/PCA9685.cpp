/*
 * PCA9685.cpp
 *
 *  Created on: Dec 9, 2012
 *      Author: terahz
 */

#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdio.h>      /* Standard I/O functions */
#include <fcntl.h>
#include <syslog.h>		/* Syslog functionallity */
#include <inttypes.h>

#include "PCA9685.h"

PCA9685::PCA9685(int bus, int address) {
	_i2cbus = bus;
	_i2caddr = address;
	snprintf(busfile, sizeof(busfile), "/dev/i2c-%d", bus);
	reset();
	setPWMFreq(1000);
}

PCA9685::~PCA9685() {
	//do something smart
}

void PCA9685::reset() {
	write_byte(MODE1, 0x00);
}

void PCA9685::setPWMFreq(int freq) {
	syslog( LOG_INFO, "Setting frequency to %d", freq);

	int new_freq = CLOCK_FREQ / 4096;
	new_freq /= freq;
	new_freq -= 1;
	char runmode = read_byte(MODE1);
	char sleepmode = (runmode & 0x7F) | 0x10;
	write_byte(MODE1, sleepmode);
	write_byte(PRE_SCALE, new_freq);
	write_byte(MODE1, runmode);
	write_byte(MODE1, runmode | 0x80);
}

void PCA9685::setPWM(uint8_t led, int value) {
	syslog( LOG_INFO, "Setting pwm of led %d to %d", led,value);

	setPWM(led,0, value);
}

void PCA9685::setPWM(uint8_t led, int on_value, int off_value) {

	write_byte(LED0_ON_L + LED_MULTIPLYER * (led-1), on_value & 0xFF);
	write_byte(LED0_ON_H + LED_MULTIPLYER * (led-1), on_value >> 8);
	write_byte(LED0_OFF_L + LED_MULTIPLYER * (led-1), off_value & 0xFF);
	write_byte(LED0_OFF_H + LED_MULTIPLYER * (led-1), off_value >> 8);
	syslog( LOG_INFO, "Writing 0x%x 0x%x 0x%x 0x%x", on_value & 0xff, on_value >>8, off_value &0xff, off_value >> 8);

}

uint8_t PCA9685::read_byte(uint8_t address) {

	int fd;
	if ((fd = open(busfile, O_RDWR)) < 0) {
		syslog(LOG_ERR, "Couldn't open I2C Bus %d", _i2cbus);
		return (-1);
	}
	if (ioctl(fd, I2C_SLAVE, _i2caddr) < 0) {
		syslog(LOG_ERR, "I2C slave %d failed", _i2caddr);
		return (-2);
	}
	int br = read(fd, dataBuffer, sizeof(dataBuffer));

	if (br == -1) {
		syslog(LOG_ERR, "Could not read data from I2C Device. read_byte()");
	}
	close(fd);

	return 0;

}

void PCA9685::write_byte(uint8_t address, uint8_t data) {
	int fd;
	uint8_t buff[2];
	buff[0] = address;
	buff[1] = data;
	if ((fd = open(busfile, O_RDWR)) < 0) {
		syslog(LOG_ERR, "Couldn't open I2C Bus %d", _i2cbus);
	}
	if (ioctl(fd, I2C_SLAVE, _i2caddr) < 0) {
		syslog(LOG_ERR, "I2C slave %d failed", _i2caddr);
	}
	uint8_t br = read(fd, dataBuffer, sizeof(dataBuffer));

	if (br == -1) {
		syslog(LOG_ERR, "Could not read data from I2C Device. read_byte()");
	}
	if (write(fd, buff, sizeof(buff)) != 2) {
		//error
	}
	close(fd);

}

