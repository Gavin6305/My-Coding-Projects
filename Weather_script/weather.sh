#!/bin/bash

milford="https://forecast.weather.gov/MapClick.php?CityName=Milford&state=NJ"
phillipsburg="https://forecast.weather.gov/MapClick.php?CityName=Phillipsburg&state=NJ"
elizabeth="https://forecast.weather.gov/MapClick.php?CityName=Elizabeth&state=NJ"
camden="https://forecast.weather.gov/MapClick.php?CityName=Camden&state=NJ"
vineland="https://forecast.weather.gov/MapClick.php?CityName=Vineland&state=NJ"

urls=( $milford $phillipsburg $elizabeth $camden $vineland )
cities=( "milford" "phillipsburg" "elizabeth" "camden" "vineland" )

tagsoupLink="https://repo1.maven.org/maven2/org/ccil/cowan/tagsoup/tagsoup/1.2.1/tagsoup-1.2.1.jar"

#Start MySQL server
sudo service mysql start

wget -q -O tagsoup-1.2.1.jar $tagsoupLink

for i in {0..4}
do
    cityName=${cities[i]}
    cityUrl=${urls[i]}

    #Download the weather Web page for the designated number of cities
    wget -q -O ${cityName}.html $cityUrl 

    #Call TagSoup to generate a .xhtml file that corresponds to each downloaded .html file.
    java -jar tagsoup-1.2.1.jar --files ${cityName}.html

    #Execute a Python script named parser.py
    ./parser.py ${cityName}.xhtml

    #weather.sh should not leave any .html, .xhtml, or temporary files on the disk drive when finished
    rm ${cityName}.html
    rm ${cityName}.xhtml
done

#Remove unnecessary filees
rm tagsoup-1.2.1.jar

#Stop MySQL server
sudo service mysql stop