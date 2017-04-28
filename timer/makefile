DIR_INC = .
DIR_SRC = .
DIR_OBJ = .
DIR_BIN = .

SRC = $(wildcard $(DIR_SRC)/*.c)
OBJ = $(patsubst %.c,$(DIR_OBJ)/*.o,$(notdir $(SRC)))

TARGET = libtimer.a
BIN_TARGET = $(DIR_BIN)/$(TARGET)

object = Timer.o TimerEngine.o UpTime.o ThreadlessTimerEngine.o

CC = gcc
CFLAGS = -g ../system/libthread.a -I ../ -I ../include -I /usr/local/include -lpthread -lcstl

$(BIN_TARGET):$(object)
	ar rcs $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(wildcard $(DIR_OBJ)/*.o) $(BIN_TARGET)

test:
	$(CC) test_timer.c $(TARGET) $(CFLAGS) -o test_timer
