/*
 * ChartJSLibraryHtmlPage.h
 *
 *  Created on: 09.07.2017
 *      Author: jane
 */

#ifndef CHARTJSLIBRARYHTMLPAGE_H_
#define CHARTJSLIBRARYHTMLPAGE_H_

#include <pgmspace.h>

namespace tankController {

    const static char AJAX_JS_LYBRARY[] PROGMEM = "function sendAsyncPOSTRequest(addr, data, callback, additionalParamsForCallBack) {\n"
            "var xhr = new XMLHttpRequest();\n"
            "var server = location.protocol + \"//\" + location.hostname + \"/\";\n"
            "//var server = \"http://192.168.4.1/\";\n"
            "xhr.open('POST', server + addr, false);\n"
            "xhr.setRequestHeader('Content-type','application/x-www-form-urlencoded;charset=UTF-8');\n"
            "xhr.onload = function (e) {if (xhr.readyState === 4) {if (xhr.status === 200) {if (callback) {callback(xhr.responseText, additionalParamsForCallBack);}} else {console.error(xhr.statusText);}}};\n"
            "xhr.onerror = function (e) {alert(xhr.statusText);};\n"
            "xhr.send(data);\n"
            "}\n";


    const static char CHART_JS_LYBRARY[] PROGMEM = "SvgGraph = function(){\n"
            "var container;\n"
            "var legend;\n"
            "var values;\n"
            "var polyLines;\n"
            "var maxSize;\n"
            "var svg;\n"
            "var hgt;\n"
            "var names = new Array();\n"
            "var setChartVisible = function () {\n"
            "polyLines[this.value].style.display = this.checked ? '' : 'none';}\n"
            "var addChart__ = function(name, color){\n"
            "var svgNS = svg.namespaceURI;\n"
            "var graph = document.createElementNS(svgNS, 'polyline');\n"
            "graph.setAttribute('style', 'fill:none;stroke:' + color + ';stroke-width:1;');\n"
            "graph.setAttribute('points', '');\n"
            "svg.appendChild(graph);\n"
            "polyLines.push(graph);\n"
            "names.push([name, color]);\n"
            "var legendLed = document.createElement('div');\n"
            "legendLed.setAttribute('style', 'width:auto; height:22px; margin: 9px 12px');\n"
            "legend.appendChild(legendLed);\n"
            "var cbLegendLed = document.createElement('input');\n"
            "cbLegendLed.setAttribute('type', 'checkbox');\n"
            "cbLegendLed.setAttribute('style', 'float:left;margin-top: 5px;');\n"
            "cbLegendLed.value = polyLines.length - 1;\n"
            "cbLegendLed.checked = true;\n"
            "cbLegendLed.onclick = setChartVisible;\n"
            "var legendCbContainer = document.createElement('div');\n"
            "legendCbContainer.setAttribute('style', 'width:auto; height:22px; float:left; background-color: ' + color);\n"
            "legendCbContainer.appendChild(cbLegendLed);\n"
            "legendLed.appendChild(legendCbContainer);\n"
            "var legendLedText = document.createElement('div');\n"
            "legendLedText.innerText = name;\n"
            "legendLedText.setAttribute('style', 'float:left; padding-left: 5px');\n"
            "legendLed.appendChild(legendLedText);\n"
            "return polyLines.length - 1;};\n"
            "var buildChart = function(index){\n"
            "var curData = '';\n"
            "for (i = 0; i < values.length; i++){curData += ' ' + i + ',' + (hgt - values[i][index]);}\n"
            "polyLines[index].setAttribute('points', curData);};\n"
            "var buildCharts=function(){for (index=0;index<polyLines.length;index++){buildChart(index);}};\n"
            "var getCharts__=function(){return polyLines;}\n"
            "var init__=function(width, height){\n"
            "hgt=height;\n"
            "container=document.createElement('div');\n"
            "container.setAttribute('style', 'width:' + width + 'px; height:' + height + 'px; background-color: lightgray');\n"
            "var svgContainer=document.createElement('div');\n"
            "svgContainer.setAttribute('style','width:'+(width-180)+'px;height:'+height+'px;float:left');\n"
            "container.appendChild(svgContainer);\n"
            "legend = document.createElement('div');\n"
            "legend.setAttribute('style', 'width: 180px; height:' + height + 'px; float: left; background-color: moccasin');\n"
            "container.appendChild(legend);\n"
            "maxSize = width - 180;\n"
            "svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg');\n"
            "values = new Array();\n"
            "polyLines = new Array();\n"
            "svg.setAttribute('width',width);\n"
            "svg.setAttribute('height',height);\n"
            "svgContainer.appendChild(svg);\n"
            "return container;};\n"
            "function addPoints__(valuesToAdd){\n"
            "var countToAdd=valuesToAdd.length;\n"
            "while ((countToAdd+values.length)>maxSize){values.shift();}\n"
            "for(i=0;i<valuesToAdd.length;i++){\n"
            "values.push(valuesToAdd[i]);}\n"
            "buildCharts();}\n"
            "return {\n"
            "init: init__,\n"
            "addPoints: addPoints__ ,\n"
            "addChart: addChart__,\n"
            "getCharts: getCharts__}};\n";


}


#endif /* CHARTJSLIBRARYHTMLPAGE_H_ */
