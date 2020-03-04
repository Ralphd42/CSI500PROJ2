CC = gcc
CFLAGS =  
LDFLAGS = -lpthread
OBJFILES = prog.o consumer.o producer.c
TARGET = prog.exe
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~