/*
 * Logger.cpp
 *
 *  Created on: 26.06.2017
 *      Author: jane
 */

#include "Logger.h"
#include <stdio.h>
#include <Arduino.h>

namespace tankController{

Logger LOGGER;

#define NEW_LINE_PART_LEN 4

Logger::Logger(){
}
//------------------------------------------------------------------------------

void Logger::add(String msg){
	add((char*)&msg[0]);
}
//------------------------------------------------------------------------------

void Logger::error(String msg){
	add(spanStart + "red\">ERROR: " + msg + spanEnd);
}
//------------------------------------------------------------------------------

void Logger::succed(String msg){
	add(spanStart + "lightGreen\">OK: " + msg + spanEnd);
}
//------------------------------------------------------------------------------

void Logger::finished(String msg){
	add(spanStart + "lime\">Finished: " + msg + spanEnd);
}
//------------------------------------------------------------------------------

void Logger::info(String msg){
	add(spanStart + "blue\">Info: " + msg + spanEnd);
}
//------------------------------------------------------------------------------


void Logger::succedOrError(bool isSuccess, String msg){
	if (isSuccess){
		succed(msg);
	} else {
		error(msg);
	}
}
//------------------------------------------------------------------------------

void Logger::infoOrError(bool isInfo, String msg){
	if (isInfo){
		info(msg);
	} else {
		error(msg);
	}
}
//------------------------------------------------------------------------------

void Logger::finishedOrError(bool isFinished, String msg){
	if (isFinished){
		finished(msg);
	} else {
		error(msg);
	}
}
//------------------------------------------------------------------------------

void Logger::add(char* msg){
  int32_t msgLen = strlen(msg);
  int32_t copyLen;
  if ((msgLen + NEW_LINE_PART_LEN + logLen) > LOGGER_SIZE){
	int8_t startCopyPos = msgLen + NEW_LINE_PART_LEN + logLen - LOGGER_SIZE;
	memmove(logData, logData  + startCopyPos, LOGGER_SIZE - startCopyPos);
	logLen = LOGGER_SIZE - msgLen - NEW_LINE_PART_LEN;
  }
  int32_t posToInsert = logLen;
  logLen = logLen + msgLen + NEW_LINE_PART_LEN;
  memmove(logData + posToInsert, msg, msgLen);
  logData[logLen - 4] = '<';
  logData[logLen - 3] = 'b';
  logData[logLen - 2] = 'r';
  logData[logLen - 1] = '>';

}

}


