
```
sudo modprobe btusb
sudo systemctl start bluetooth.service
sudo hciconfig hci0 up
sudo setcap cap_net_raw+eip $(eval readlink -f `which node`)
```
