CC = gcc
CFLAGS = -W -Wall
TARGET = 20181703.out
OBJECTS = 20181703.o dir.o history.o opcode_list.o opcode_search.o dump.o change_to_dec.o edit_dump.o type.o assemble_pass_1.o symbol.o assemble_pass_2.o free.o linking_pass_1.o linking_pass_2.o run.o

all : $(TARGET)

$(TARGET): $(OBJECTS)
		$(CC) $(CLFAGS) -o $@ $^

clean :
		rm *.o 20181703.out
