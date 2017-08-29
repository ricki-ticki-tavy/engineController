/*
 * WebMainPageHtml.h
 *
 *  Created on: 03.07.2017
 *      Author: jane
 */

#ifndef WEBMAINPAGEHTML_H_
#define WEBMAINPAGEHTML_H_

#include <pgmspace.h>

namespace tankController{

const static char ROOT_HTML_PAGE[] PROGMEM = "<!DOCTYPE html>\n"
            "<html lang=\"en\">\n"
            "<head>\n"
            "<script language=\"JavaScript\">\n"
            "function setSelected(eName) {document.getElementById(eName).className = \"selected\";}\n"
            "function setUnselected(eName) {document.getElementById(eName).className = \"ref\";}\n"
            "function setUnselectedAll(){\n"
            "setUnselected(\"log\");\n"
            "setUnselected(\"telemetry\");\n"
            "setUnselected(\"engineStatistic\");\n"
        	"setUnselected(\"settings\");\n"
            "setUnselected(\"update\");}\n"
            "function openSrc(eName){setUnselectedAll();setSelected(eName);\n"
            "var frame = document.getElementById(\"pageFrame\");\n"
            "frame.src = location.protocol + \"//\" + location.hostname + \"/\" + eName;}\n"
            "</script>\n"
            "<meta charset=\"UTF-8\">\n"
            "<title>Tank engines controller console</title>\n"
            "<style>\n"
            ".ref {\n"
            "cursor: pointer;\n"
            "border: solid black 1px;\n"
            "padding-left: 10px;\n"
            "background-color: antiquewhite;\n"
            "}\n"
            ".selected {\n"
            "cursor: pointer;\n"
            "border: solid black 1px;\n"
            "padding-left: 10px;\n"
            "background-color: gold;\n"
            "}\n"
            "</style>\n"
            "</head>\n"
            "<body>\n"
            "<table style=\"width: 900px; height: 800px; \" cellspacing=\"2px\" cellpadding=\"2px\">\n"
            "<tr style=\"height: 25px\"><td colspan=\"2\" style=\"text-align: center; background-color: lightgreen\">Tank engines controller console</td></tr>\n"
            "<tr>\n"
            "<td style=\"width: 200px; vertical-align: top\">\n"
            "<table style=\"width: 200px \" cellspacing=\"2px\" cellpadding=\"2px\">\n"
            "<tr><td onclick=\"openSrc('log');\" id=\"log\" class=\"ref\" style=\"width: 200px\">Log</td></tr>\n"
            "<tr><td onclick=\"openSrc('telemetry');\" id=\"telemetry\" class=\"ref\" >Telemetry</td></tr>\n"
			"<tr><td onclick=\"openSrc('engineStatistic');\" id=\"engineStatistic\" class=\"ref\" >Engines statistic</td></tr>\n"
            "<tr><td onclick=\"openSrc('settings');\" id=\"settings\" class=\"ref\" >Settings</td></tr>\n"
            "<tr><td onclick=\"openSrc('update');\" id=\"update\" class=\"ref\" >Upgrade firmware</td></tr>\n"
            "</table>\n"
            "</td>\n"
            "<td style=\"background-color: antiquewhite\">\n"
            "<iframe id=\"pageFrame\" frameborder=\"0px\" style=\"width: 100%; height: 100%; overflow: scroll\"></iframe>\n"
            "</td></tr></table></body></html>\n";


}


#endif /* WEBMAINPAGEHTML_H_ */
