CC ?= gcc
CFLAGS += -Wall -Wextra -O2
CFLAGS += -I../ta/include
LDLIBS += -lteec

all: optee-ca

optee-ca: main.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	$(RM) *.o optee-ca

