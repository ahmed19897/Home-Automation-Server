<<<<<<< HEAD
<<<<<<< HEAD
# Home-Automation-Server
A Home automation server running on Ubuntu Server and an ESP32 Node running RTOS for gas sensing and communicates with the server using socket communication



# in the Node_Code is the RTOS based code that runs on the esp32 that does the follwing 
1) run socket communication task every 3.5 seconds and it is implmented in a way that if the communcation dropped from the node side it will re-establish communication automatically with the server with no intervention
=======
# in the Node_Code is the RTOS based code that runs on the esp32 that does the follwing 
 1) run socket communication task every 3.5 seconds and it is implmented in a way that if the communcation dropped from the node side it will re-establish communication automatically with the server with no intervention
>>>>>>> 873d4b2 (adding README)
2) run a ADC conversion task every 1 second that reads a butane gas sensor reading to send it to the server

# in the Server_Code is the 
1).cpp server program that handle the communication with the sensor node
2)service file for the server program that restarts every 60 seconds to re-establish communication with node if the communication dropped


# the user will interact with the server through an application opened from chrome using your domain name along with a user name and password to read the latest gas sensor reading 
<<<<<<< HEAD
=======
in the Node_Code is the RTOS based code that runs on the esp32 that does the follwing : 1) run socket communication task every 3.5 seconds and it is implmented in a way that if the communcation dropped from the node side it will re-establish communication automatically with the server with no intervention
2) run a ADC conversion task every 1 second that reads a butane gas sensor reading to send it to the server

in the Server_Code is the : 1).cpp server program that handle the communication with the sensor node
2)service file for the server program that restarts every 60 seconds to re-establish communication with node if the communication dropped


the user will interact with the server through an application opened from chrome using your domain name along with a user name and password to read the latest gas sensor reading 
>>>>>>> e5c26ea (First Commit for Home automation Server with Nodes Running Rtos based OS using esp32)
=======
>>>>>>> 873d4b2 (adding README)




<<<<<<< HEAD
<<<<<<< HEAD
# steps to configure and run the ubuntu server station
=======
//steps to configure and run the ubuntu server station
>>>>>>> e5c26ea (First Commit for Home automation Server with Nodes Running Rtos based OS using esp32)
=======
# steps to configure and run the ubuntu server station
>>>>>>> 873d4b2 (adding README)
1-dhcp reservation in the router to static reserve the ip for the server using the server mac address
2-port forward the port 80 to 10000 on the internal host "our server" in the router setting
3-install ubuntu server
4-rf kill was blocking the network card
5-install rf kill on usb and mount /dev/sdb1 /media/usb
6- sudo dpkg rfkill
7- check if soft block using rfkill 
8- use rfkill to remove the block 
9-connect to wifi
10-install webmin
11-install noip-duc on linux
12-create a noip account and create a host name and activate it
13- run the command  "noip-duc -g <domain you creatd on noip.com> -u <username> -p <Password> &"   to run the ddns service on noiop 
14-test the connection from outside network to the server using the noip host name
15-run the server.cpp code and wait for a client connection


<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 873d4b2 (adding README)
# run the noip service to continously update the ddns with the latest real ip of the server
noip-duc -g <domain you creatd on noip.com> -u <username> -p <Password> &

# run the Server application to handle socket communication from different nodes and take the data and save it in the server
./Server


# stop the noip service whenever needed
<<<<<<< HEAD
=======
//run the noip service to continously update the ddns with the latest real ip of the server
noip-duc -g <domain you creatd on noip.com> -u <username> -p <Password> &

//run the Server application to handle socket communication from different nodes and take the data and save it in the server
./Server


//stop the noip service whenever needed
>>>>>>> e5c26ea (First Commit for Home automation Server with Nodes Running Rtos based OS using esp32)
=======
>>>>>>> 873d4b2 (adding README)
ps aux | grep noip
kill -9 <noip process number>

