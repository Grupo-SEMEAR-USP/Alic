
COM ?= /dev/ttyACM0

FQBN  ?= arduino:avr:uno
BOARD ?= arduino.avr.uno
CPU   ?= atmega328p


BUILDD      ?= ./src/arduino/inobuild
ARDUINO_SRC  = ./src/arduino
INO_FILES    = $(wildcard $(ARDUINO_SRC)/*.ino)
HPP_FILES    = $(wildcard $(ARDUINO_SRC)/*.hpp)


HEX_FILE ?= $(BUILDD)/$(BOARD)/arduino.ino.with_bootloader.hex


.PHONY: all clean ino debug flash

all: ino

clean:
	@rm -rf $(BUILDD)

ino: EXTRA_FLAGS+=-DNODEBUG
ino: $(HEX_FILE)

debug: $(HEX_FILE)

flash:
	arduino-cli upload --fqbn $(FQBN) -p $(COM) -t $(ARDUINO_SRC)


$(HEX_FILE): $(INO_FILES) $(HPPFILES)
	arduino-cli compile --fqbn $(FQBN) $(ARDUINO_SRC) --build-property "build.extra_flags=$(EXTRA_FLAGS)" --export-binaries

