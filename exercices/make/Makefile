CC = arm-none-eabi-gcc
CFLAGS = -Wall -Werror -g -Og -mthumb
LFLAGS = -L/opt/mylibs -mthumb
LDLIBS = -lm 

VPATH = libs  # Add libs directory for wildcard to search
SRC   = $(wildcard *.c)
HEADERS = $(wildcard *.h) 
OBJS  = $(SRC:.c=.o) # Convert .c files to .o files in a implicit way (using CFLAGS declared)

OUTPUT = hello

all: $(OUTPUT)

$(OUTPUT): $(OBJS) stubs.o
	$(CC) $(LFLAGS) $^ $(LDLIBS) -o $@   

# Change in any header file will trigger recompilation of all .o files
$(OBJS): $(HEADERS)

# overriding CFLAGS
stubs.o: CFLAGS = -g -O2 -mthumb
# stubs recipe
stubs.o: stubs.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(OUTPUT) stubs.o

.PHONY: all clean 

#obs: $@ - target name $^ - all prerequisites $< - first prerequisite