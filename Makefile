# the compiler to use
CC = clang

# compiler flags:
CFLAGS  = -Wall -O3 -flto

# the name to use for both the target source file and the output file:
TARGET = server

# specify the path to sc_sock.c
DEPS = sc/socket/sc_sock.o handle.o sc/thread/sc_thread.o

# rule for building the final executable
all: $(TARGET)

# linking step for the final executable
$(TARGET): $(TARGET).o $(DEPS)
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o $(DEPS)

# rule for building the target's object file
$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c

# rule for building the sc_sock.o object file
sc/socket/sc_sock.o: sc/socket/sc_sock.c
	$(CC) $(CFLAGS) -c sc/socket/sc_sock.c -o sc/socket/sc_sock.o

sc/thread/sc_thread.o: sc/thread/sc_thread.c
	$(CC) $(CFLAGS) -c sc/thread/sc_thread.c -o sc/thread/sc_thread.o

# sc/queue/sc_queue.o: sc/queue/sc_queue.h
# 	$(CC) $(CFLAGS) -c sc/queue/sc_queue.h -o sc/queue/sc_queue.o

handle.o: handle.c
	$(CC) $(CFLAGS) -c handle.c -o handle.o

# clean up build files
clean:
	rm -f $(TARGET) $(TARGET).o $(DEPS)

