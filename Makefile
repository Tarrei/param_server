CC=g++
CFLAGS=-g -Wall -O3 

TARGET=main  
SRCPPS =  $(wildcard *.cc)
  
OBJS = $(SRCPPS:.cc=.o)
  
$(TARGET):$(OBJS)
	$(CC) -o $@ $^ -std=c++11 -lzmq -pthread
  
clean:
	rm -rf $(TARGET) 
	rm -rf $(OBJS)

exec:clean $(TARGET)
	./$(TARGET)

%.o:%.cc
	$(CC) $(CFLAGS) -o $@ -c $< -std=c++11 -lzmq -pthread
