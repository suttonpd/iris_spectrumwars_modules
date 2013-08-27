#!/bin/bash

#Run this script with the interface as the first parameter and the ip address as the second. 
#For example, "./setupdirectlink eth0 192.168.2.1"

#Bring up the interface and set IP
sudo ifconfig $1 up
sudo ifconfig $1 $2 netmask 255.255.255.0 
echo Interface up

#Add a route to direct all usrp traffic to the correct interface
sudo route add -net 192.168.2.0 netmask 255.255.255.0 gw 192.168.2.2 dev $1
echo Route added


