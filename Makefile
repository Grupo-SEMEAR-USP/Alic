
COM ?= /dev/ttyACM0

FQBN  ?= arduino:avr:uno
BOARD ?= arduino.avr.uno
CPU   ?= atmega328p


BUILDD      ?= ./inobuild
INO_BUILDD  ?= $(BUILDD)/build

SRC          = ./src
ARDUINO_SRC  = $(SRC)/arduino

RES         ?= ./res

PY_SRC       = $(SRC)/computador
PY_DRAW_NAME = PICdraw.py
PY_TR_NAME   = turtletest.py
PY_TEST_SVG  = $(RES)/Logo\ NRE.svg
EXTRA_FLAGS ?= -Wall -Wextra


OUT_FILE ?= arduino.ino.with_bootloader.hex
HEX_FILE ?= $(INO_BUILDD)/$(BOARD)/$(OUT_FILE)



.PHONY: all clean ino debug flash pytest

all: ino

clean:
	@rm -rf $(BUILDD)


ino: EXTRA_FLAGS+=-DNODEBUG
ino: $(HEX_FILE)

debug: $(HEX_FILE)


flash:
	cd $(INO_BUILDD)/$(BOARD)/ && avrdude -p $(CPU) -c arduino -U flash:w:$(OUT_FILE):i -F -P $(COM)

pytest:
	cd $(PY_SRC) && python $(PY_DRAW_NAME) $(COM) $(PWD)/$(PY_TEST_SVG) 10

pyturtle:
	cd $(PY_SRC) && python $(PY_TR_NAME) $(COM) $(PWD)/$(PY_TEST_SVG) 10


$(HEX_FILE): $(INO_FILES)
	arduino-cli compile --fqbn $(FQBN) $(ARDUINO_SRC) --build-property "build.extra_flags=$(EXTRA_FLAGS)" --export-binaries
	@mkdir $(BUILDD) 2>/dev/null | true
	@rm -rf $(BUILDD)/build
	@mv -fu $(ARDUINO_SRC)/build $(BUILDD)

