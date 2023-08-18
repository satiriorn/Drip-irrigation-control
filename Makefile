TARGET = main
CC = avr-gcc
OBJCOPY = avr-objcopy
DEVICE = atmega328p
CFLAGS = -Os -mmcu=$(DEVICE)
LDFLAGS = -mmcu=$(DEVICE)
PORT = \\.\COM5

PREF_SRC = ./src/
PREF_OBJ = ./object/
PREF_ELF = ./elf/

SRC = $(wildcard $(PREF_SRC)*.c) 
OBJ = $(patsubst $(PREF_SRC)%.c, $(PREF_OBJ)%.o, $(SRC))

$(TARGET) : $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(TARGET)

$(PREF_OBJ)%.o: $(PREF_SRC)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PREF_ELF)%.elf : $(PREF_OBJ)%.o
	$(CC) -o $< $@

%.hex: %.elf
	 $(OBJCOPY) -j .text -j .data -O hex $< $@

deploy: 
	avrdude -F -V -c arduino -p ATMEGA328P -P ${PORT} -b 115200 -U flash:w:main

clean:
	del .\object\*.o

show:
	echo $(SRC)