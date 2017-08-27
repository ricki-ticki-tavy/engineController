/*
 * WebUtils.h
 *
 *  Created on: 02.07.2017
 *      Author: jane
 */

#ifndef WEBUTILS_H_
#define WEBUTILS_H_

#include <ESP8266WebServer.h>

namespace tankController{

class WebUtils{
public:
	/**
	 * Считать из параметров строковое значение
	 */
	bool readStringArg(ESP8266WebServer* httpServer, char* name, char* buffer, int maxLen);

	/**
	 * Считать из параметров числовое значение с плавающей точкой
	 */
	bool readDoubleArg(ESP8266WebServer* httpServer, char* name, double* buffer);

	/**
	 * Считать из параметров целое числовое значение
	 */
	bool readIntArg(ESP8266WebServer* httpServer, char* name, int* buffer);

	/**
	 * Считать из параметров целое байтовое значение
	 */
	bool readCharArg(ESP8266WebServer* httpServer, char* name, char* buffer);

	/**
	 * Считать из параметров логическое значение
	 */
	bool readBooleanArg(ESP8266WebServer* httpServer, char* name, bool* buffer);

	/**
	 * Проверка, что строка содержит допустимое число
	 */
	bool isStringContainsValidNumber(char* source, bool isFloat, bool isCanBeNegative);

	int itob(int source, char *buffer, int index);
private:
	char* PARAM_TEXT = "parametr \"";
	char* IS_NOT_TEXT = "\" is not ";
};


extern WebUtils WEB_UTILS;
}


#endif /* WEBUTILS_H_ */
