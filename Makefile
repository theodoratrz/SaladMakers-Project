# paths
MODULES = ./modules
INCLUDE = ./include

# compiler
CC = gcc

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -Wall -g -I$(INCLUDE)

# Αρχεία .o
OBJS_C =  $(MODULES)/chef.o $(MODULES)/writing_data.o $(MODULES)/reading_data.o

OBJS_S = $(MODULES)/saladmaker.o $(MODULES)/writing_data.o

OBJS_R = $(MODULES)/reading_data.o

# Το εκτελέσιμο πρόγραμμα
EXEC_C = chef

EXEC_S = saladmaker

EXEC_R = reading_data

$(EXEC_C): $(OBJS_C)
	$(CC) $(OBJS_C) -o $(EXEC_C) -pthread

$(EXEC_S): $(OBJS_S)
	$(CC) $(OBJS_S) -o $(EXEC_S) -pthread

$(EXEC_R): $(OBJS_R)
	$(CC) $(OBJS_R) -o $(EXEC_R)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)