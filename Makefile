
#TODO: fazer um makefile bom 

all: src/arduino/arduino.ino
	arduino-cli compile --fqbn arduino:avr:uno src/arduino/arduino.ino --build-property "build.extra_flags=\"-DNODEBUG\"" --export-binaries

debug:
	arduino-cli compile --fqbn arduino:avr:uno src/arduino/arduino.ino --export-binaries

flash:
	cd src/arduino/build/arduino.avr.uno/ && sudo avrdude -p atmega328p -c arduino -U flash:w:arduino.ino.with_bootloader.hex:i -F -P /dev/ttyACM0

py:
	python ./src/computador/draw.py /dev/ttyACM0 res/Logo\ NRE.svg 3