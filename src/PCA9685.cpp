/*
 * PCA9685.cpp
 *
 *  Created on: Dec 9, 2012
 *      Author: terahz
 */

#include "PCA9685.h"

PCA9685::PCA9685(int bus, int address) {
	_i2cbus = bus;
	_i2caddr = address;
	reset();
}

PCA9685::~PCA9685() {
	//do something smart
}

void PCA9685::reset() {
	write_byte(MODE1, 0x00);
}

void PCA9685::setPWMFreq(int freq) {
	int new_freq = CLOCK_FREQ / 4096;
	new_freq /= freq;
	new_freq -= 1;
	char oldmode = read_byte(MODE1);
	char newmode = (oldmode & 0x7F) | 0x10; // sleep
	write_byte(MODE1, newmode); // go to sleep
	write_byte(PRE_SCALE, new_freq); // set the prescaler
	write_byte(MODE1, oldmode);
	write_byte(MODE1, oldmode | 0x80);
}

void PCA9685::setPWM(int device, int value) {
	write_byte(LED0_ON_L + LED_MULTIPLYER * device, 0);
	write_byte(LED0_ON_H + LED_MULTIPLYER * device, 0 >> 8);
	write_byte(LED0_OFF_L + LED_MULTIPLYER * device, value);
	write_byte(LED0_OFF_H + LED_MULTIPLYER * device, value >> 8);
}

void PCA9685::setPWM(int device, int on_value, int off_value) {
	write_byte(LED0_ON_L + LED_MULTIPLYER * device, on_value);
	write_byte(LED0_ON_H + LED_MULTIPLYER * device, on_value >> 8);
	write_byte(LED0_OFF_L + LED_MULTIPLYER * device, off_value);
	write_byte(LED0_OFF_H + LED_MULTIPLYER * device, off_value >> 8);
}

char PCA9685::read_byte(char address) {
	//do i2c magic
	return 0;
}

void PCA9685::write_byte(char address, char data) {
	//do i2c magic
}

