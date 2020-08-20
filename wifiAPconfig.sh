#!/bin/bash

# REF: https://www.raspberrypi.org/documentation/configuration/wireless/access-point-routed.md

# In order to work as an access point, the Raspberry Pi needs to have the hostapd access point software package installed
sudo apt install hostapd

# Enable the wireless access point service and set it to start when your Raspberry Pi boots
sudo systemctl unmask hostapd
sudo systemctl enable hostapd

# In order to provide network management services (DNS, DHCP) to wireless clients, the Raspberry Pi needs to have the dnsmasq software package installed
sudo apt install dnsmasq

# Finally, install netfilter-persistent and its plugin iptables-persistent. This utilty helps by saving firewall rules and restoring them when the Raspberry Pi boots
sudo DEBIAN_FRONTEND=noninteractive apt install -y netfilter-persistent iptables-persistent

# The Raspberry Pi runs a DHCP server for the wireless network; this requires static IP configuration for the wireless interface (wlan0) in the Raspberry Pi.
# The Raspberry Pi also acts as the router on the wireless network, and as is customary, we will give it the first IP address in the network: 192.168.4.1.
sudo echo -e "interface wlan0" >> /etc/dhcpcd.conf
sudo echo -e "static ip_address=192.168.4.1/24" >> /etc/dhcpcd.conf
sudo echo -e "nohook wpa_supplicant /etc/dhcpcd.conf" >> /etc/dhcpcd.conf

# To enable routing, i.e. to allow traffic to flow from one network to the other in the Raspberry Pi
sudo touch /etc/sysctl.d/routed-ap.conf
sudo echo -e "net.ipv4.ip_forward=1" >> /etc/sysctl.d/routed-ap.conf
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
sudo netfilter-persistent save

#The DHCP and DNS services are provided by dnsmasq.
# The default configuration file serves as a template for all possible configuration options, whereas we only need a few. It is easier to start from an empty file.
sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig
sudo touch /etc/dnsmasq.conf
sudo echo -e "interface=wlan0" >>  /etc/dnsmasq.conf # Listening interface
sudo echo -e "dhcp-range=192.168.4.2,192.168.4.20,255.255.255.0,24h" # Pool of IP addresses served via DHCP
sudo echo -e "domain=wlan" # Local wireless DNS domain
sudo echo -e "address=/gw.wlan/192.168.4.1"  # Alias for this router

# Create the hostapd configuration file, located at /etc/hostapd/hostapd.conf, to add the various parameters for your wireless network.
sudo touch /etc/hostapd/hostapd.conf

sudo echo -e "country_code=ES" >> /etc/hostapd/hostapd.conf
sudo echo -e "interface=wlan0" >> /etc/hostapd/hostapd.conf
sudo echo -e "ssid=RaspiHouse" >> /etc/hostapd/hostapd.conf
sudo echo -e "hw_mode=g" >> /etc/hostapd/hostapd.conf
sudo echo -e "channel=7" >> /etc/hostapd/hostapd.conf
sudo echo -e "macaddr_acl=0" >> /etc/hostapd/hostapd.conf
sudo echo -e "auth_algs=1" >> /etc/hostapd/hostapd.conf
sudo echo -e "ignore_broadcast_ssid=0" >> /etc/hostapd/hostapd.conf
sudo echo -e "wpa=2" >> /etc/hostapd/hostapd.conf
sudo echo -e "wpa_passphrase=MyIoTProject" >> /etc/hostapd/hostapd.conf
sudo echo -e "wpa_key_mgmt=WPA-PSK" >> /etc/hostapd/hostapd.conf
sudo echo -e "wpa_pairwise=TKIP" >> /etc/hostapd/hostapd.conf
sudo echo -e "rsn_pairwise=CCMP" >> /etc/hostapd/hostapd.conf

sudo reboot