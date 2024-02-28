.PHONY: run monitor

ard = "/home/gruvw/Applications/Arduino/bin/arduino-cli"

run:
	$(ard) compile
	$(ard) upload

monitor:
	$(ard) monitor -p /dev/ttyUSB0
