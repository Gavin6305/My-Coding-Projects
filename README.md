# Weather Script
This is a script that displays a table of the extended weather forecast for 5 cities in New Jersey (Camden, Elizabeth, Milford, Phillipsburg and Vineland). It consists of multiple files.

**weather.sh:**
This is a shell script that downloads the html for the 5 webpages that displays the weather information of the 5 cities via wget, converts them into XHTML files using a Java library called tagsoup, and then executes a python script called parser.py on each of those files. It does not leave any unnecessary files in the current directory after it runs; this includes the html, XHTML and tagsoup files created during the download process.

**parser.py:**
This is a python script that takes an XHTML file as an argument, scrapes the relevant weather information from the file and puts it into an SQL database. It uses the MySQL Connector module (mysql.connector)  to insert the scraped data into the database so that it can be displayed in the PHP files.

**PHP files (display.php, elizabeth.php, milford.php, phillipsburg.php, and vineland.php):**
These are the files that actually displays the weather information in a table. It displays the day of the week, whether it is night or day, the temperature, a short description of the weather, and a detailed description of the weather for each day. It connects to the SQL database using mysqli, uses SQL queries to get the information from the database, and displays it as HTML. Also, display.php displays the weather information for Camden, NJ; I named it display.php because it's easy to decide which one to display first instead of choosing a random city. 

**styles.css:**
This is the CSS styling for the HTML table that is generated by the PHP files.

**sql_dumps:**
These are dumps of the SQL database and the table data.

**sources.txt:**
These are the links to the forecast pages of the 5 cities in NJ, which are on https://forecast.weather.gov.

**Note:**
In order to display the pages on localhost, an Apache2 web server must already be running. In order to have the webpages display the latest forecast, run weather.sh script on a Linux terminal with `user@computer:~$ ./weather.sh`

**Screenshot of the forecast table**
![image](https://github.com/Gavin6305/My-Coding-Projects/assets/96839346/678abdd8-67a7-44a3-bf6d-dbfc19c1126f)
