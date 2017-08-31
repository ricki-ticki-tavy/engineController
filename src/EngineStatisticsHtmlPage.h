/*
 * EngineStatisticsHtmlPage.h
 *
 *  Created on: 10.07.2017
 *      Author: jane
 */

#ifndef ENGINESTATISTICSHTMLPAGE_H_
#define ENGINESTATISTICSHTMLPAGE_H_

#include <pgmspace.h>

namespace tankController{

const static char ENGINE_STATISTICS_HTML_PAGE[] PROGMEM = "<!DOCTYPE html>\n"
		"<html lang='en'>\n"
		"<head>\n"
		"<meta charset='UTF-8'>\n"
		"<title>testSvg</title>\n"
		"<script src='/js/svgCharts.js'></script>\n"
		"<script src='/js/common.js'></script>\n"
		"<script language='JavaScript'>\n"
		"var maxSize = 682;\n"
		"var charts = new Array();\n"
		"var lastIndexes = new Array(0, 0);\n"
		"var chartIndex = 0;\n"
		"function chartDataLoaded(response){\n"
		"var data = JSON.parse(response);\n"
		"lastIndexes[chartIndex] = data.lastIndex;\n"
		"var dataArray = data.data;\n"
		"if (dataArray.length > 0){ charts[chartIndex].addPoints(dataArray); }\n"
		"if (chartIndex == 0) { chartIndex = 1; setTimeout(loadChartData, 1); }\n"
		"else { chartIndex = 0; setTimeout(loadChartData, 1000);}}\n"
		"function loadChartData(){ sendAsyncPOSTRequest(\"data/engineStatistic\", \"lastIndex=\" + lastIndexes[chartIndex] + \"&engineIndex=\" + chartIndex, chartDataLoaded); }\n"
		"function initChart(index){\n"
		"charts[index].addChart('req enc spd', 'green');\n"
		"charts[index].addChart('fact enc spd', 'blue');\n"
		"charts[index].addChart('error', 'red');\n"
		"charts[index].addChart('P corr', 'lime');\n"
		"charts[index].addChart('D corr', 'orange');\n"
		"charts[index].addChart('I corr', 'brown');\n"
		"charts[index].addChart('power', '#ff69b4');\n"
		"charts[index].addChart('voltage', '#9400d3');\n"
		"charts[index].addChart('current', '#00bfff'); "
		"charts[index].addChart('drv tempr', '#FFFF00'); "
		"charts[index].addChart('eng tempr', '#00FFF9');}\n"
		"function loaded(){\n"
		"charts.push(new  SvgGraph());\n"
		"charts.push(new  SvgGraph());\n"
		"var leftEngineTitle = document.createElement('div');\n"
		"leftEngineTitle.setAttribute('style', 'width:auto; height:18px');\n"
		"leftEngineTitle.innerText = 'Left engine statistics';\n"
		"document.body.appendChild(leftEngineTitle);\n"
		"document.body.appendChild(charts[0].init(maxSize, 345));\n"
		"var rightEngineTitle = document.createElement('div');\n"
		"rightEngineTitle.setAttribute('style', 'width:auto; height:18px; padding-top: 20px');\n"
		"rightEngineTitle.innerText = 'Right engine statistics';\n"
		"document.body.appendChild(rightEngineTitle);\n"
		"document.body.appendChild(charts[1].init(maxSize, 345));\n"
		"initChart(0);\n"
		"initChart(1);\n"
		"setTimeout(loadChartData, 100);}\n"
		"</script>\n"
		"</head><body onload='loaded();'></body></html";


}

#endif /* ENGINESTATISTICSHTMLPAGE_H_ */
