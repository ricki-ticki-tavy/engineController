/*
 * TelemetryPageHtml.h
 *
 *  Created on: 01.07.2017
 *      Author: jane
 */

#ifndef TELEMETRYPAGEHTML_H_
#define TELEMETRYPAGEHTML_H_

#include <pgmspace.h>

namespace tankController{

    const static char TELEMETRY_JS_PAGE[] PROGMEM = "var counter=0;\n"
            "function setFldVal(name, val){document.getElementById(name).innerHTML = val;}\n"
            "function telemetryLoaded(response) {\n"
            "var obj=JSON.parse(response);\n"
            "if (obj!=undefined){setFldVal(\"portExtender0Found\",obj.portExtender0Found);"
            "setFldVal(\"portExtender1Found\",obj.portExtender1Found);"
            "setFldVal(\"pwmFound\",obj.pwmFound);"
            "setFldVal(\"mainVoltage\",obj.mainVoltage);"
            "setFldVal(\"mainCurrentPeak\",obj.mainCurrentPeak);"
            "setFldVal(\"mainCurrentAvg\",obj.mainCurrentAvg);"
            "setFldVal(\"mbTempr\",obj.mbTempr);"
            "setFldVal(\"leftDriverTempr\",obj.leftDriverTempr);"
            "setFldVal(\"leftEngineTempr\",obj.leftEngineTempr);"
            "setFldVal(\"leftEngineVoltage\",obj.leftEngineVoltage);"
            "setFldVal(\"leftEngineCurrentPeak\",obj.leftEngineCurrentPeak);"
            "setFldVal(\"leftEngineCurrentAvg\",obj.leftEngineCurrentAvg);"
            "setFldVal(\"rightDriverTempr\",obj.rightDriverTempr);"
            "setFldVal(\"rightEngineTempr\",obj.rightEngineTempr);"
            "setFldVal(\"rightEngineVoltage\",obj.rightEngineVoltage);"
            "setFldVal(\"rightEngineCurrentPeak\",obj.rightEngineCurrentPeak);"
            "setFldVal(\"rightEngineCurrentAvg\",obj.rightEngineCurrentAvg);"
            "setFldVal(\"telemetryCounter\",obj.telemetryCounter);"
            "setFldVal(\"leftEncoderValue\",obj.leftEncoderValue);"
            "setFldVal(\"rightEncoderValue\",obj.rightEncoderValue);"
            "setFldVal(\"counter\",counter++);}"
            "setTelemetryTimer();}"
            "function sendAsyncPOSTRequest(addr,data,callback,additionalParamsForCallBack){"
            "var xhr=new XMLHttpRequest();"
            "var server=location.protocol+\"//\"+location.hostname+\"/\";"
            "xhr.open('POST',server+addr,false);"
            "xhr.setRequestHeader('Content-type','application/x-www-form-urlencoded;charset=UTF-8');"
            "xhr.onload=function(e){if(xhr.readyState===4){if(xhr.status===200){if (callback) {callback(xhr.responseText, additionalParamsForCallBack);}} else {console.error(xhr.statusText);}}};"
            "xhr.onerror=function(e){alert(xhr.statusText);};"
            "xhr.send(data);}"
            "function loadTelemetry(){sendAsyncPOSTRequest(\"getTelemetry\", \"\",telemetryLoaded);}"
            "function setTelemetryTimer(){setTimeout(loadTelemetry,100);}";

    const static char TELEMETRY_HTML_PAGE[] PROGMEM = "<!DOCTYPE html>\n"
            "<html lang=\"en\">\n"
            "<head>\n"
            "<script src='/js/telemetry.js'></script>\n"
            "<meta charset=\"UTF-8\">\n"
            "<title>Telemetry</title>\n"
            "</head>\n"
            "<body onLoad=\"setTelemetryTimer();\">\n"
            "<table>\n"
            "<tr><td style=\"width:200px\">Parameter</td><td style=\"width:200px\">Value</td></tr>\n"
            "<tr><td>portExtender0Found</td><td id=\"portExtender0Found\">-</td></tr>\n"
            "<tr><td>portExtender1Found</td><td id=\"portExtender1Found\">-</td></tr>\n"
            "<tr><td>pwmFound</td><td id=\"pwmFound\">-</td></tr>\n"
            "<tr><td>mainVoltage</td><td id=\"mainVoltage\">-</td></tr>\n"
            "<tr><td>mainCurrentPeak</td><td id=\"mainCurrentPeak\">-</td></tr>\n"
            "<tr><td>mainCurrentAvg</td><td id=\"mainCurrentAvg\">-</td></tr>\n"
            "<tr><td>mbTempr</td><td id=\"mbTempr\">-</td></tr>\n"
            "<tr><td>leftDriverTempr</td><td id=\"leftDriverTempr\">-</td></tr>\n"
            "<tr><td>leftEngineTempr</td><td id=\"leftEngineTempr\">-</td></tr>\n"
            "<tr><td>leftEngineVoltage</td><td id=\"leftEngineVoltage\">-</td></tr>\n"
            "<tr><td>leftEngineCurrentPeak</td><td id=\"leftEngineCurrentPeak\">-</td></tr>\n"
            "<tr><td>leftEngineCurrentAvg</td><td id=\"leftEngineCurrentAvg\">-</td></tr>\n"
            "<tr><td>rightDriverTempr</td><td id=\"rightDriverTempr\">-</td></tr>\n"
            "<tr><td>rightEngineTempr</td><td id=\"rightEngineTempr\">-</td></tr>\n"
            "<tr><td>rightEngineVoltage</td><td id=\"rightEngineVoltage\">-</td></tr>\n"
            "<tr><td>rightEngineCurrentPeak</td><td id=\"rightEngineCurrentPeak\">-</td></tr>\n"
            "<tr><td>rightEngineCurrentAvg</td><td id=\"rightEngineCurrentAvg\">-</td></tr>\n"
            "<tr><td>leftEncoderValue</td><td id=\"leftEncoderValue\">-</td></tr>\n"
            "<tr><td>rightEncoderValue</td><td id=\"rightEncoderValue\">-</td></tr>\n"
            "<tr><td>telemetryCounter</td><td id=\"telemetryCounter\">-</td></tr>\n"
            "<tr><td>counter</td><td id=\"counter\">-</td></tr>\n"
            "</table>\n"
            "</body>\n"
            "</html>\n\0";
}


#endif /* TELEMETRYPAGEHTML_H_ */
