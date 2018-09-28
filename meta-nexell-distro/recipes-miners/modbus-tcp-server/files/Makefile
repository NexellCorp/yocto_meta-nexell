OBJECTS = modbus-tcp-server

CFLAGS += `pkg-config --cflags libmodbus`

LDFLAGS += `pkg-config --libs libmodbus` -lpthread

all:$(OBJECTS)

$(OBJECTS):$(OBJECTS).o
	$(CC) $(LDFLAGS) $^ -Wall -o $@

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean:
	$(RM) *.o $(OBJECTS)
