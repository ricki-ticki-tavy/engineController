/*
 * Logger.h
 *
 *  Created on: 26.06.2017
 *      Author: jane
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <Arduino.h>

#define LOGGER_SIZE 8192

namespace tankController {

class Logger{
public:
	char logData[LOGGER_SIZE + 1];
	int logLen = 0;

	Logger();
	void add(char* msg);

	/**
	 * debug
	 */
	void add(String msg);

	void error(String msg);
	void succed(String msg);
	void finished(String msg);
	void info(String msg);

	void succedOrError(bool isSuccess, String msg);
	void infoOrError(bool isInfo, String msg);
	void finishedOrError(bool isFinished, String msg);

	static void handleLog();
private:
	String spanStart = "<span style=\"color: ";
	String spanEnd = "</span>";

};

extern Logger LOGGER;

}


#endif /* LOGGER_H_ */
