CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

TARGET = mSim

OBJ = mSim.o cJSON.o time.o

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)


mSim.o: mSim.c cJSON.h time.h
	$(CC) $(CFLAGS) -c mSim.c

cJSON.o: cJSON.c cJSON.h
	$(CC) $(CFLAGS) -c cJSON.c

time.o: time.c time.h
	$(CC) $(CFLAGS) -c time.c

clean: 
	rm -f *.o $(TARGET)

run: $(TARGET)
	./$(TARGET) customers.json products.json

