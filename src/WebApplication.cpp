/*
 * WebApplication.cpp
 *
 *  Created on: 27.06.2017
 *      Author: jane
 */

#include "WebApplication.h"
#include "Logger.h"
#include "TelemetryPageHtml.h"
#include "InMemoryConstUtils.h"
#include "SettingsManagerPageHtml.h"
#include "WebMainPageHtml.h"
#include "WebUtils.h"
#include "JSLibraryHtmlPage.h"
#include "EngineStatisticsHtmlPage.h"

namespace tankController {

    ESP8266WebServer *httpServer;

    DeviceInfo *deviceInfo;
    PwmPCA9685Driver *web_pwmPCA9685Driver;
    EnginePowerController *web_enginePowerController;

    WebApplication::WebApplication(ESP8266WebServer *server,
                                   DeviceInfo *deviceInfo_,
                                   EnginePowerController *enginePowerController,
                                   PwmPCA9685Driver *pwmPCA9685Driver) {
        httpServer = server;
        deviceInfo = deviceInfo_;
        web_pwmPCA9685Driver = pwmPCA9685Driver;
        web_enginePowerController = enginePowerController;

        httpServer->on("/", handleRootPage);
        httpServer->on("/log", handleLog);
        httpServer->on("/js/common.js", handleJsCommonLibrary);
        httpServer->on("/getTelemetry", handleTelemetry);
        httpServer->on("/telemetry", handleTelemetryPage);
        httpServer->on("/js/telemetry.js", handleTelemetryJsPage);

        httpServer->on("/settings", handleSettingsPage);
        httpServer->on("/data/settings", handleSettings);
        httpServer->on("/js/settings1.js", handleJsSettingsLibrary1);
        httpServer->on("/js/settings2.js", handleJsSettingsLibrary2);
        httpServer->on("/js/settings3.js", handleJsSettingsLibrary3);

        httpServer->on("/js/svgCharts.js", handleJsChartLibraryPage);
        httpServer->on("/engineStatistic", handleEngineStatistic);
        httpServer->on("/data/engineStatistic", handleEngineStatisticData);
    }
//------------------------------------------------------------------

    void WebApplication::handleLog() {
        httpServer->send(200, "text/html", LOGGER.logData);
    }
//------------------------------------------------------------------

    void WebApplication::handleTelemetry() {
        String data = String("{") +
                      "\"portExtender0Found\":" + String(deviceInfo->portExtender0Found ? "true," : "false,") +
                      "\"portExtender1Found\":" + String(deviceInfo->portExtender1Found ? "true," : "false,") +
                      "\"pwmFound\":" + String(deviceInfo->pwmFound ? "true," : "false,") +
                      "\"mainVoltage\":" + String(deviceInfo->mainVoltage) + "," +
                      "\"mainCurrentPeak\":" + String(deviceInfo->mainCurrentPeak) + "," +
                      "\"mainCurrentAvg\":" + String(deviceInfo->mainCurrentAvg) + "," +
                      "\"mbTempr\":" + String(deviceInfo->mbTempr) + "," +
                      "\"leftDriverTempr\":" + String(deviceInfo->leftDriverTempr) + "," +
                      "\"leftEngineTempr\":" + String(deviceInfo->leftEngineTempr) + "," +
                      "\"leftEngineVoltage\":" + String(deviceInfo->leftEngineVoltage) + "," +
                      "\"leftEngineCurrentPeak\":" + String(deviceInfo->leftEngineCurrentPeak) + "," +
                      "\"leftEngineCurrentAvg\":" + String(deviceInfo->leftEngineCurrentAvg) + "," +
                      "\"rightDriverTempr\":" + String(deviceInfo->rightDriverTempr) + "," +
                      "\"rightEngineTempr\":" + String(deviceInfo->rightEngineTempr) + "," +
                      "\"rightEngineVoltage\":" + String(deviceInfo->rightEngineVoltage) + "," +
                      "\"rightEngineCurrentPeak\":" + String(deviceInfo->rightEngineCurrentPeak) + "," +
                      "\"rightEngineCurrentAvg\":" + String(deviceInfo->rightEngineCurrentAvg) + "," +
                      "\"leftEncoderValue\":" + String(deviceInfo->engineControlData[0].currentEncoderFrequency) + "," +
                      "\"rightEncoderValue\":" + String(deviceInfo->engineControlData[1].currentEncoderFrequency) +
                      "," +
                      "\"telemetryCounter\":" + String(deviceInfo->telemetryCounter) + "}";

        httpServer->send(200, "text/json", &data[0]);
    }

//------------------------------------------------------------------
    void WebApplication::answerPage(const char page[], String mimeType) {
        char *data;
        data = (char *) malloc(sizeof(page) + 1);
        IN_MEMORY_CONST_UTILS.readConstMessage(page, data);
        httpServer->send(200, mimeType, data);
        delete data;
    }
//------------------------------------------------------------------

    void WebApplication::handleTelemetryPage() {
        char *data;
        data = (char *) malloc(sizeof(TELEMETRY_HTML_PAGE) + 1);
        IN_MEMORY_CONST_UTILS.readConstMessage(TELEMETRY_HTML_PAGE, data);
        httpServer->send(200, "text/html", data);
        delete data;
    }
//------------------------------------------------------------------

    void WebApplication::handleTelemetryJsPage() {
        char *data;
        data = (char *) malloc(sizeof(TELEMETRY_JS_PAGE) + 1);
        IN_MEMORY_CONST_UTILS.readConstMessage(TELEMETRY_JS_PAGE, data);
        httpServer->send(200, "text/html", data);
        delete data;
    }
//------------------------------------------------------------------

    void WebApplication::handleSettings() {
        if (!httpServer->args()) {
            GlobalSettings *settings = deviceInfo->settingsManager->globalSettings;
            String data = String("{") +
                          "\"startWifi\":" + String(settings->startWifi ? "true," : "false,") +
                          "\"wifiPassword\":\"" + String(settings->wifiPassword) + "\"," +
                          "\"wifiSSID\":\"" + String(settings->wifiSSID) + "\"," +
                          "\"hostName\":\"" + String(settings->hostName) + "\"," +
                          "\"coefP\":" + String(settings->coefP) + "," +
                          "\"coefD\":" + String(settings->coefD) + "," +
                          "\"coefI\":" + String(settings->coefI) + "," +
                          "\"coefE\":" + String(settings->coefE) + "," +
                          "\"maxAbsISum\":" + String(settings->maxAbsISum) + "," +
                          "\"pidPeriodMs\":" + String(settings->pidPeriodMs) + "," +
                          "\"diffMin\":" + String(settings->diffMin) + "," +
                          "\"leftForwardPWMChannel\":" + String((int) settings->leftForwardPWMChannel) + "," +
                          "\"leftBackwardPWMChannel\":" + String((int) settings->leftBackwardPWMChannel) + "," +
                          "\"rightForwardPWMChannel\":" + String((int) settings->rightForwardPWMChannel) + "," +
                          "\"rightBackwardPWMChannel\":" + String((int) settings->rightBackwardPWMChannel) + "," +
                          "\"pwmFrequency\":" + String(settings->pwmFrequency) + "," +
                          "\"leftEncoderGpioPin\":" + String((int) settings->leftEncoderGpioPin) + "," +
                          "\"rightEncoderGpioPin\":" + String((int) settings->rightEncoderGpioPin) + "," +
                          "\"maxPower\":" + String(settings->maxPower) + "," +
                          "\"minPower\":" + String(settings->minPower) + "," +
                          "\"maxRotateSpeed\":" + String(settings->maxRotateSpeed) + "," +
                          "\"encoderCountsPerRotate\":" + String(settings->encoderCountsPerRotate) + "," +
                          "\"maxContinuousDriverCurrentMa\":" + String(settings->maxContinuousDriverCurrentMa) + "," +
                          "\"maxPeakDriverCurrentMa\":" + String(settings->maxPeakDriverCurrentMa) + "," +
                          "\"maxDriverTemperature\":" + String(settings->maxDriverTemperature) + "," +
                          "\"maxEngineTemperature\":" + String(settings->maxEngineTemperature) + "," +
                          "\"maxMBCurrentMa\":" + String(settings->maxMBCurrentMa) + "," +
                          "\"minEngineVoltageMv\":" + String(settings->minEngineVoltageMv) + "," +
                          "\"minTotalVoltageMv\":" + String(settings->minTotalVoltageMv) + "," +
                          "\"telemetryIntervalMs\":" + String(settings->telemetryIntervalMs) + "," +
                          "\"SMAPeakValues\":" + String(settings->SMAPeakValues) + "," +
                          "\"SMAAvgValues\":" + String(settings->SMAAvgValues) + "," +
                          "\"enginesEnabled\":" + String(settings->enginesEnabled ? "true," : "false,") +
                          "\"enginesLogBuffer\":" + String(settings->enginesLogBuffer) + "," +
                          "\"mainCurrentAnalogChannel\":" + String((int) settings->mainCurrentAnalogChannel) + "," +
                          "\"mainVoltageAnalogChannel\":" + String((int) settings->mainVoltageAnalogChannel) + "," +
                          "\"mbTemprAnalogChannel\":" + String((int) settings->mbTemprAnalogChannel) + "," +
                          "\"leftEngineCurrentAnalogChannel\":" +
                          String((int) settings->leftEngineCurrentAnalogChannel) + "," +
                          "\"leftEngineVoltageAnalogChannel\":" +
                          String((int) settings->leftEngineVoltageAnalogChannel) + "," +
                          "\"leftDriverTemprAnalogChannel\":" + String((int) settings->leftDriverTemprAnalogChannel) +
                          "," +
                          "\"leftEngineTemprAnalogChannel\":" + String((int) settings->leftEngineTemprAnalogChannel) +
                          "," +
                          "\"rightEngineCurrentAnalogChannel\":" +
                          String((int) settings->rightEngineCurrentAnalogChannel) + "," +
                          "\"rightEngineVoltageAnalogChannel\":" +
                          String((int) settings->rightEngineVoltageAnalogChannel) + "," +
                          "\"rightDriverTemprAnalogChannel\":" + String((int) settings->rightDriverTemprAnalogChannel) +
                          "," +
                          "\"rightEngineTemprAnalogChannel\":" + String((int) settings->rightEngineTemprAnalogChannel) +
                          "}";

            httpServer->send(200, "text/json", &data[0]);
        } else {
            if (strcmp("save", &httpServer->arg("action")[0]) == 0) {
                webSaveSettings();
            }
        }
    }
//------------------------------------------------------------------

    void WebApplication::handleSettingsPage() {
        char *data;
        data = (char *) malloc(sizeof(SETTINGS_HTML_PAGE));
        IN_MEMORY_CONST_UTILS.readConstMessage(SETTINGS_HTML_PAGE, data);
        httpServer->send(200, "text/html", data);
        delete data;
    }
//------------------------------------------------------------------

    void WebApplication::handleJsSettingsLibrary1() {
        char *data;
        data = (char *) malloc(sizeof(SETTINGS_JS_LIB1_HTML_PAGE));
        IN_MEMORY_CONST_UTILS.readConstMessage(SETTINGS_JS_LIB1_HTML_PAGE, data);
        httpServer->send(200, "text/plain", data);
        delete data;
    }
//------------------------------------------------------------------

    void WebApplication::handleJsSettingsLibrary2() {
        char *data;
        data = (char *) malloc(sizeof(SETTINGS_JS_LIB2_HTML_PAGE));
        IN_MEMORY_CONST_UTILS.readConstMessage(SETTINGS_JS_LIB2_HTML_PAGE, data);
        httpServer->send(200, "text/plain", data);
        delete data;
    }
//------------------------------------------------------------------

    void WebApplication::handleJsSettingsLibrary3() {
        char *data;
        data = (char *) malloc(sizeof(SETTINGS_JS_LIB3_HTML_PAGE));
        IN_MEMORY_CONST_UTILS.readConstMessage(SETTINGS_JS_LIB3_HTML_PAGE, data);
        httpServer->send(200, "text/plain", data);
        delete data;
    }
//------------------------------------------------------------------


//    void WebApplication::handleJsChartLibraryPage() {
//        String* st = IN_MEMORY_CONST_UTILS.readConstMessage(CHART_JS_LYBRARY);
//        LOGGER.add(*st);
    //httpServer->send(200, "text/plain", *st);
    //     delete st;
// http://192.168.4.1/js/svgCharts.js
//    }
//------------------------------------------------------------------


    void WebApplication::handleJsChartLibraryPage() {
        char *data;
        data = (char *) malloc(sizeof(CHART_JS_LYBRARY) + 1);
        IN_MEMORY_CONST_UTILS.readConstMessage(CHART_JS_LYBRARY, data);
//        LOGGER.add(data);
        httpServer->send(200, "text/plain", data);
        delete data;
    }
//------------------------------------------------------------------

//    void WebApplication::handleJsChartLibraryPage() {
//        char *data;
//        data = (char *) malloc(sizeof(CHART_JS_LYBRARY) + 1);
//        IN_MEMORY_CONST_UTILS.readConstMessage(CHART_JS_LYBRARY, data);
//        httpServer->send(200, "text/plain", data);
//        delete data;
//    }
//------------------------------------------------------------------

    void WebApplication::handleRootPage() {
        char *data;
        data = (char *) malloc(sizeof(ROOT_HTML_PAGE) + 1);
        IN_MEMORY_CONST_UTILS.readConstMessage(ROOT_HTML_PAGE, data);
        httpServer->send(200, "text/html", data);
        delete data;
    }
//------------------------------------------------------------------

    void WebApplication::handleJsCommonLibrary() {
        char *data;
        data = (char *) malloc(sizeof(AJAX_JS_LYBRARY) + 1);
        IN_MEMORY_CONST_UTILS.readConstMessage(AJAX_JS_LYBRARY, data);
        httpServer->send(200, "text/plain", data);
        delete data;
    }
//------------------------------------------------------------------

    void WebApplication::webSaveSettings() {
        // сохранение настроек
        LOGGER.info("Saving settings via console...");

        int errorOccured = 0;

        GlobalSettings *globalSettings = deviceInfo->settingsManager->globalSettings;

        char *strBuffer = (char *) malloc(26);

        if (!WEB_UTILS.readStringArg(httpServer, "wifiPassword", strBuffer, sizeof(globalSettings->wifiPassword))) {
            errorOccured++;
        } else {
            if (strcmp(&globalSettings->wifiPassword[0], strBuffer) != 0) {
                // изменился пароль
                memcpy(&globalSettings->wifiPassword[0], strBuffer, strlen(strBuffer));
                globalSettings->wifiStarted = false;
            }
        }

        if (!WEB_UTILS.readStringArg(httpServer, "wifiSSID", strBuffer, sizeof(globalSettings->wifiSSID))) {
            errorOccured++;
        } else {
            if (strcmp(&globalSettings->wifiSSID[0], strBuffer) != 0) {
                // изменилось имя сети
                memcpy(&globalSettings->wifiSSID[0], strBuffer, strlen(strBuffer));
                globalSettings->wifiStarted = false;
            }
        }

        delete strBuffer;

        if (!WEB_UTILS.readStringArg(httpServer, "hostName", &globalSettings->hostName[0],
                                     sizeof(globalSettings->hostName))) {
            errorOccured++;
        }

        // PID настройки
        if (!WEB_UTILS.readDoubleArg(httpServer, "coefP", &globalSettings->coefP)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readDoubleArg(httpServer, "coefD", &globalSettings->coefD)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readDoubleArg(httpServer, "coefI", &globalSettings->coefI)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readDoubleArg(httpServer, "coefE", &globalSettings->coefE)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readDoubleArg(httpServer, "maxAbsISum", &globalSettings->maxAbsISum)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readIntArg(httpServer, "diffMin", &globalSettings->diffMin)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readIntArg(httpServer, "pidPeriodMs", &globalSettings->pidPeriodMs)) {
            errorOccured++;
        }

        // Настройки PWM каналов
        if (!WEB_UTILS.readCharArg(httpServer, "leftForwardPWMChannel", &globalSettings->leftForwardPWMChannel)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readCharArg(httpServer, "leftBackwardPWMChannel", &globalSettings->leftBackwardPWMChannel)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readCharArg(httpServer, "rightForwardPWMChannel", &globalSettings->rightForwardPWMChannel)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readCharArg(httpServer, "rightBackwardPWMChannel", &globalSettings->rightBackwardPWMChannel)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readCharArg(httpServer, "leftEncoderGpioPin", &globalSettings->leftEncoderGpioPin)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readCharArg(httpServer, "rightEncoderGpioPin", &globalSettings->rightEncoderGpioPin)) {
            errorOccured++;
        }
        int tempInt;
        if (!WEB_UTILS.readIntArg(httpServer, "pwmFrequency", &tempInt)) {
            errorOccured++;
        } else {
            if (globalSettings->pwmFrequency != tempInt) {
                // частота изменилась. Проапдейтим железо
                globalSettings->pwmFrequency = tempInt;
                web_pwmPCA9685Driver->setPwmFrequency(globalSettings->pwmFrequency);
                web_pwmPCA9685Driver->setChannelValue(11, 2048);

            }
        }

        // Настройки каналов двигателей
        if (!WEB_UTILS.readIntArg(httpServer, "maxPower", &globalSettings->maxPower)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readIntArg(httpServer, "minPower", &globalSettings->minPower)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readIntArg(httpServer, "maxRotateSpeed", &globalSettings->maxRotateSpeed)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readIntArg(httpServer, "encoderCountsPerRotate", &globalSettings->encoderCountsPerRotate)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readBooleanArg(httpServer, "enginesEnabled", &globalSettings->enginesEnabled)) {
            errorOccured++;
        }

        /**
         * Предельные значения токов, температур и напряжений, при которых нужно сообщать об ошибке
         */
        if (!WEB_UTILS.readIntArg(httpServer, "maxContinuousDriverCurrentMa",
                                  &globalSettings->maxContinuousDriverCurrentMa)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readIntArg(httpServer, "maxPeakDriverCurrentMa", &globalSettings->maxPeakDriverCurrentMa)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readIntArg(httpServer, "maxDriverTemperature", &globalSettings->maxDriverTemperature)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readIntArg(httpServer, "maxEngineTemperature", &globalSettings->maxEngineTemperature)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readIntArg(httpServer, "maxMBCurrentMa", &globalSettings->maxMBCurrentMa)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readIntArg(httpServer, "minEngineVoltageMv", &globalSettings->minEngineVoltageMv)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readIntArg(httpServer, "minTotalVoltageMv", &globalSettings->minTotalVoltageMv)) {
            errorOccured++;
        }



        // Настройки телеметрии
        if (!WEB_UTILS.readIntArg(httpServer, "enginesLogBuffer", &globalSettings->enginesLogBuffer)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readIntArg(httpServer, "telemetryIntervalMs", &globalSettings->telemetryIntervalMs)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readIntArg(httpServer, "SMAPeakValues", &globalSettings->SMAPeakValues)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readIntArg(httpServer, "SMAAvgValues", &globalSettings->SMAAvgValues)) {
            errorOccured++;
        }

        if (!WEB_UTILS.readCharArg(httpServer, "mainCurrentAnalogChannel", &globalSettings->mainCurrentAnalogChannel)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readCharArg(httpServer, "mainVoltageAnalogChannel", &globalSettings->mainVoltageAnalogChannel)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readCharArg(httpServer, "mbTemprAnalogChannel", &globalSettings->mbTemprAnalogChannel)) {
            errorOccured++;
        }

        if (!WEB_UTILS.readCharArg(httpServer, "leftEngineCurrentAnalogChannel",
                                   &globalSettings->leftEngineCurrentAnalogChannel)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readCharArg(httpServer, "leftEngineVoltageAnalogChannel",
                                   &globalSettings->leftEngineVoltageAnalogChannel)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readCharArg(httpServer, "leftDriverTemprAnalogChannel",
                                   &globalSettings->leftDriverTemprAnalogChannel)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readCharArg(httpServer, "leftEngineTemprAnalogChannel",
                                   &globalSettings->leftEngineTemprAnalogChannel)) {
            errorOccured++;
        }

        if (!WEB_UTILS.readCharArg(httpServer, "rightEngineCurrentAnalogChannel",
                                   &globalSettings->rightEngineCurrentAnalogChannel)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readCharArg(httpServer, "rightEngineVoltageAnalogChannel",
                                   &globalSettings->rightEngineVoltageAnalogChannel)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readCharArg(httpServer, "rightDriverTemprAnalogChannel",
                                   &globalSettings->rightDriverTemprAnalogChannel)) {
            errorOccured++;
        }
        if (!WEB_UTILS.readCharArg(httpServer, "rightEngineTemprAnalogChannel",
                                   &globalSettings->rightEngineTemprAnalogChannel)) {
            errorOccured++;
        }

        if (globalSettings->enginesLogBuffer > 200){
            globalSettings->enginesLogBuffer = 200;
        } else if (globalSettings->enginesLogBuffer <= 10){
            globalSettings->enginesLogBuffer = 10;
        }

        if (!errorOccured) {
            // нет ошибок. Можно все сохранить
            deviceInfo->settingsManager->saveSettings();

            // Если были изменения в каналлах управления двигателями, то перезагрузим их
            web_enginePowerController->loadPwmChannels();
        }

        if (errorOccured) {
            httpServer->send(200, "text/json",
                             "{\"answerMsg\":\"Settings accepted, but not saved. Occurred " + String(errorOccured) +
                             " errors.\", \"answerCode\":0}");
        } else {
            httpServer->send(200, "text/json", "{\"answerMsg\":\"Settings accepted and saved\", \"answerCode\":0}");
        }
        LOGGER.finished("settings from console saved");
    }
//---------------------------------------------------------


    void WebApplication::handleEngineStatisticData() {
        int lastIndex;
        int engineIndex;
        const int maxPoints = 50;

        if (!WEB_UTILS.readIntArg(httpServer, "lastIndex", &lastIndex)) {
            httpServer->send(200, "text/json", "{\"answerMsg\":\"absent lastIndex\", \"answerCode\":1}");
        }

        if (!WEB_UTILS.readIntArg(httpServer, "engineIndex", &engineIndex)) {
            httpServer->send(200, "text/json", "{\"answerMsg\":\"absent engineIndex\", \"answerCode\":1}");
        }
        char *buffer = (char *) malloc(3000); // максимальный пакет

        EngineStatisticData *engineStatisticData = deviceInfo->engineStatistic[engineIndex];
        int savedHead = engineStatisticData->engineStatisticsHeadIndex;

        // Худший расклад 40 байт на 1 нод лога одного движка. При 7к пакете это 170 элементов
        String temp = "{\"lastIndex\":" + String(savedHead) +
                      ", \"data\":[";
        int len = strlen(&temp[0]);
        memcpy(&buffer[0], &temp[0], len);
        int bufIndex = len;

        int elementsCount = 0;
        // подсчитаем кол-во точек данных с последнего запроса. если их более максимального, то будем отсчитывать 170 последних
        if (lastIndex > savedHead) {
            // голова закруглилась к началу массива. Расчет кол-ва элементов будет немного иным
            elementsCount = deviceInfo->settingsManager->globalSettings->enginesLogBuffer - lastIndex; // -1
            lastIndex = 0;
        }

        elementsCount += savedHead - lastIndex;

        if (elementsCount > maxPoints) {
            elementsCount = maxPoints;
        }

        // теперь выставим начальный индекс для сериализации данных
        int readIndex;
        if (elementsCount > savedHead) {
            // был переход через максимум массива на начало
            readIndex = deviceInfo->settingsManager->globalSettings->enginesLogBuffer - elementsCount + savedHead;
        } else {
            readIndex = savedHead - elementsCount;
        }

        String isFirst = "";
        // сериализуем
        while (elementsCount > 0) {
            temp = isFirst + "[" + String(engineStatisticData->statistics[readIndex].requiredEncoderValue) + "," +
                   String(engineStatisticData->statistics[readIndex].currEncoderValue) + "," +
                   String(engineStatisticData->statistics[readIndex].error) + "," +
                   String(engineStatisticData->statistics[readIndex].pCorr) + "," +
                   String(engineStatisticData->statistics[readIndex].dCorr) + "," +
                   String(engineStatisticData->statistics[readIndex].iCorr) + "," +
                   String(engineStatisticData->statistics[readIndex].power) + "," +
                   String(engineStatisticData->statistics[readIndex].voltage) + "," +
                   String(engineStatisticData->statistics[readIndex].current) + "," +
                   String(engineStatisticData->statistics[readIndex].drvTempr) + "," +
                   String(engineStatisticData->statistics[readIndex].engTempr) + "]";
            isFirst = ",";
            memcpy(&buffer[bufIndex], &temp[0], temp.length());
            bufIndex += temp.length();

            if (++readIndex >= deviceInfo->settingsManager->globalSettings->enginesLogBuffer) {
                readIndex = 0;
            }

            elementsCount--;
        }

        temp = "]}";
        memcpy(&buffer[bufIndex], &temp[0], temp.length());
        bufIndex += temp.length();
        buffer[bufIndex] = 0;

        httpServer->send(200, "text/json", buffer);
        delete buffer;
    }
//---------------------------------------------------------

    void WebApplication::handleEngineStatistic() {
        char *data;
        data = (char *) malloc(sizeof(ENGINE_STATISTICS_HTML_PAGE));
        IN_MEMORY_CONST_UTILS.readConstMessage(ENGINE_STATISTICS_HTML_PAGE, data);
        httpServer->send(200, "text/html", data);
        delete data;
    }
//---------------------------------------------------------


}
