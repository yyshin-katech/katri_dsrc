#
# Makefile for v2x-rsu
#  

.SUFFIXES : .c .o 

APP_SRCS += decodeSample.c

APP_OBJS = $(APP_SRCS:%c=%o)

LIBS	+= -lpthread

CFLAGS += -I.
CFLAGS += -I../include

LDFLAGS += -L../lib

LIBS += -ltrafficinfo1
LIBS += -lasn1ber
LIBS += -lasn1oer
LIBS += -lasn1per
LIBS += -lasn1rt

CC=gcc
TARGET= decodeSample

all: $(TARGET)


$(TARGET): $(APP_OBJS)
	$(CC) -o $@ $(APP_OBJS) $(CFLAGS) $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o
	rm -f $(TARGET)

