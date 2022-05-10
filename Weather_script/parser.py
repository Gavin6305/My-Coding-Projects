#!/bin/python3

#Use the xml.dom.minidom module to traverse the .xhtml documents one file at a time, 
#extract the relevant data, and insert the extracted data into a MySQL database.

import sys
import xml.dom.minidom
import mysql.connector

def getFileName (fname):
    result = str.upper(fname[0]) + ""
    for c in range(1, len(fname)):
        if fname[c] == ".":
            return result
        else:
            result += fname[c]
    return result

def getElementsByAttr (elements, attribute, value, value2 = ""):
    result = []
    for e in elements:
        if e.getAttribute(attribute) == value or e.getAttribute(attribute) == value2:
            result.append(e)
    return result

def getShortDesc (description):
    result = ""
    for i in description:
        if i == ',' or i == '.':
            return result
        result = result + i

def getTemp (description):
    result = ""
    inNum = False
    for i in description:
        if i in "0123456789":
            result += i
            inNum = True
        elif (i == '.' or i == ',') and inNum:
            return result
        elif (str.isalpha(i) or i == "%") and inNum:
            result = ""
            inNum = False
    
def getDay (desc):
    dotw = ["Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"]
    result = ""
    for i in range(len(desc)):
        if desc[i] == " ":
            if result in dotw:
                return result
            else:
                return "Today"
        elif i == len(desc) - 1:
            result += desc[i]
            if result in dotw:
                return result
            else:
                return "Today"
        else:
            result += desc[i]
    return result

def clear (cursor):
    cityName = getFileName(sys.argv[1])
    query = 'DELETE FROM ' + cityName
    cursor.execute(query)

def insert (cursor, date, dOrN, temp, sDesc, lDesc):
    cityName = getFileName(sys.argv[1])
    dnBool = 0
    if dOrN == "Day":
        dnBool = 1
    query = 'INSERT INTO ' + cityName + '(date,dayOrNight,temperature,shortDesc,longDesc) VALUES (%s,%s,%s,%s,%s)'
    cursor.execute(query, (date,dnBool,temp,sDesc,lDesc))

file = xml.dom.minidom.parse(sys.argv[1])
elements = file.getElementsByTagName("div")

forecasts = getElementsByAttr(elements, "class", "row row-odd row-forecast", "row row-even row-forecast")

days = []
dayOrNight = []
temps = []
shortDescs = []
longDescs = []

for forecast in forecasts:
    day = forecast.getElementsByTagName("b")
    divs = forecast.getElementsByTagName("div")
    desc = getElementsByAttr(divs, "class", "col-sm-10 forecast-text")

    for el in day[0].childNodes:
        if el.nodeType == el.TEXT_NODE:
            days.append(getDay(el.nodeValue))
            if "night" in el.nodeValue:
                dayOrNight.append("Night")
            elif "Night" in el.nodeValue:
                dayOrNight.append("Night")
            else:
                dayOrNight.append("Day")
            
    for el in desc[0].childNodes:
        if el.nodeType == el.TEXT_NODE:
            longDescs.append(el.nodeValue.strip())
            shortDescs.append(getShortDesc(el.nodeValue.strip()))
            temps.append(getTemp(el.nodeValue.strip()))

try:

    cnx = mysql.connector.connect(host='localhost', user='root', password='balls', database='forecastDB')
    cursor = cnx.cursor()

    clear(cursor)

    for i in range(len(forecasts)):
        insert(cursor, days[i], dayOrNight[i], temps[i], shortDescs[i], longDescs[i])
        cnx.commit()

    cursor.close()

except mysql.connector.Error as err:
    print(err)
    
finally:
    try:
        cnx
    except NameError:
        pass
    else:
        cnx.close()




