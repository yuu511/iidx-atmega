MMCU = atmega32u4
CLKSPEED = 16000000 

SRCDIR = src
INCDIR = include
BUILDFILES = build

MODULES = iidx_state 
SRC = ${MODULES:=.cpp} main.cpp
OBJ = ${SRC:.cpp=.o}
CPPOBJ = ${addprefix ${BUILDFILES}/, ${OBJ}}

BIN = iidx
CPPBIN = ${addprefix ${BUILDFILES}/,${BIN}}
HEXFILE = ${BIN:=.hex}

CC = avr-gcc
GENFLAGS = -Wall -Os
MMCUFLAG = -mmcu=${MMCU}
CLKFLAG = -DF_CPU=${CLKSPEED}
INCLUDEFLAG = -I${INCDIR}
FLAGS = ${GENFLAGS} ${MMCUFLAG} ${CLKFLAG} ${INCLUDEFLAG}

all : ${BUILDFILES} ${HEXFILE} 

${BUILDFILES} :
	mkdir $@

${HEXFILE} : ${CPPBIN}
	avr-objcopy -O ihex -R .eeprom $< $@
    
${CPPBIN} : ${CPPOBJ}
	${CC} ${MMCUFLAG} $^ -o $@ 

${CPPOBJ} : ${BUILDFILES}/%.o : ${SRCDIR}/%.cpp
	${CC} ${FLAGS} -c $< -o $@

upload : all
	./setup_ports.sh
    
clean :
	rm -r ${HEXFILE} ${BUILDFILES}
 
hexonly : ${HEXFILE}
	rm ${BIN} ${OBJECTS}
