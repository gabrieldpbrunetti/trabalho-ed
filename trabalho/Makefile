# Definições do compilador e flags
CC      := gcc
CFLAGS  := -Wall -Wextra -O2 -Iincludes

# Arquivos fonte e objeto
SRCS    := src/main.c src/bd_pacientes.c
OBJS    := $(SRCS:.c=.o)

# Nome do executável final
TARGET  := programa

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
