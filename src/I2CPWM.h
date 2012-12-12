/*
 * I2CPWM.h
 *
 *  Created on: Dec 9, 2012
 *      Author: terahz
 */

#ifndef I2CPWM_H_
#define I2CPWM_H_

#define BD_NO_CHDIR				01
#define BD_NO_CLOSE_FILES		02
#define BD_NO_REOPEN_STD_FDS	04
#define BD_NO_UMASK0			10
#define BD_MAX_CLOSE			8192

class I2CPWM {
public:
	I2CPWM();
	int daemonize(int);

};

#endif /* I2CPWM_H_ */
