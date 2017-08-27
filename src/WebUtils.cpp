/*
 * WebUtils.cpp
 *
 *  Created on: 02.07.2017
 *      Author: jane
 */

#include "WebUtils.h"
#include <ESP8266WebServer.h>
#include "Logger.h"

namespace tankController{

WebUtils WEB_UTILS;


bool WebUtils::isStringContainsValidNumber(char* source, bool isFloat, bool isCanBeNegative){
	// Признак, что знак уже найден и не может более присутствовать в строке
	bool signAlreadyFound = !isCanBeNegative;

	// Признак, что десятичный разделитель уже найден и не может более присутствовать в строке
	bool decimalSeparatorAlreadyFound = !isFloat;

	int sourceIndex = 0;

	bool result = true;

	// пока не достигнут конец строки или пока не превышена допустимая длина
	while ((source[sourceIndex]) && (sourceIndex < 21)) {
		if ((source[sourceIndex] == '-') || (source[sourceIndex] == '+')) {
			// Найден знак
			if (signAlreadyFound) {
				result = false;
				break;
			} else {
				signAlreadyFound = true;
			}
		} else if (source[sourceIndex] == '.') {
			// найден десятичный разделитель
			if (decimalSeparatorAlreadyFound) {
				result = false;
				break;
			} else {
				decimalSeparatorAlreadyFound = true;
			}
		} else if ((source[sourceIndex] < 0x30) || (source[sourceIndex] > 0x39)){
			// найден символ, отличный от цифры
			result = false;
			break;
		}
		sourceIndex++;
	}

	// проверка, что конец строки найден при отсутствии прочих ошибок
	if ((result) && (source[sourceIndex])){
		result = false;
	}

	return result;
}
//---------------------------------------------------------------------------------------------

/**
 * Считать из параметров строковое значение
 */
bool WebUtils::readStringArg(ESP8266WebServer* httpServer, char* name, char* buffer, int maxLen){
	String val = httpServer->arg(name);
	if (strcmp("", &val[0]) == 0){
		LOGGER.error(PARAM_TEXT + String(name) + "\" not set");
		return false;
	} else {
		if ((val.length() + 1) > maxLen) {
			LOGGER.error(PARAM_TEXT + String(name) + "\" too long");
			return false;
		} else {
			memcpy(buffer, &val[0], val.length());
			buffer[val.length()] = 0x00;
			return true;
		}
	}
}
//---------------------------------------------------------------------------------------------

/**
 * Считать из параметров числовое значение с плавающей точкой
 */
bool WebUtils::readDoubleArg(ESP8266WebServer* httpServer, char* name, double* buffer){
	char* strBuffer = (char*)malloc(21);
	bool rslt = false;
	if (readStringArg(httpServer, name, strBuffer, 21)) {
		if (isStringContainsValidNumber(strBuffer, true, true)){
			buffer[0] = atof(strBuffer);
			rslt = true;
		} else {
			LOGGER.error(PARAM_TEXT + String(name) + IS_NOT_TEXT + "a float.");
		}
	}
	delete strBuffer;
    return rslt;
}
//---------------------------------------------------------------------------------------------

/**
 * Считать из параметров целое числовое значение
 */
bool WebUtils::readIntArg(ESP8266WebServer* httpServer, char* name, int* buffer){
	char* strBuffer = (char*)malloc(13);
	bool rslt = false;
	if (readStringArg(httpServer, name, strBuffer, 13)) {
		if (isStringContainsValidNumber(strBuffer, false, true)){
			buffer[0] = atoi(strBuffer);
			rslt = true;
		} else {
			LOGGER.error(PARAM_TEXT + String(name) + IS_NOT_TEXT + "an Integer.");
		}
	}
	delete strBuffer;
    return rslt;
}
//---------------------------------------------------------------------------------------------

/**
 * Считать из параметров целое байтовое значение
 */
bool WebUtils::readCharArg(ESP8266WebServer* httpServer, char* name, char* buffer){
	int data;
	bool result = false;

	if (readIntArg(httpServer, name, &data)){
		if (data > 255) {
			LOGGER.error(PARAM_TEXT + String(name) + "\" is grater than 255");
		} else if (data < 0) {
			LOGGER.error(PARAM_TEXT + String(name) + "\" is less than 0");
		} else {
			buffer[0] = (char)data;
			result = true;
		}
	}

    return result;
}
//---------------------------------------------------------------------------------------------

/**
 * Считать из параметров логическое значение
 */
bool WebUtils::readBooleanArg(ESP8266WebServer* httpServer, char* name, bool* buffer){
	char* strBuffer = (char*)malloc(6);
	bool rslt = false;
	if (readStringArg(httpServer, name, strBuffer, 6)) {
		if (strcmp("true", strBuffer) == 0){
			buffer[0] = true;
			rslt = true;
		} else if (strcmp("false", strBuffer) == 0){
			buffer[0] = false;
			rslt = true;
		} else {
			LOGGER.error(PARAM_TEXT + String(name) + IS_NOT_TEXT + "a Boolean.");
		}
	}
	delete strBuffer;
    return rslt;
}
//---------------------------------------------------------------------------------------------


int WebUtils::itob(int source, char *buffer, int index){
	itoa(source, &buffer[index], 10);
	return index + strlen(&buffer[index]);
}


}


