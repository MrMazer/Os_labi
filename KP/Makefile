CC = g++
CFLAGS = -std=c++11
SRC = dag_scheduler.cpp
EXEC = dag_scheduler

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC)
