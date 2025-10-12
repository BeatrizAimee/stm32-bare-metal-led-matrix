#variables
PREFIX    = arm-none-eabi-
CC        = $(PREFIX)gcc # Compiler
AS        = $(PREFIX)as # Assembler
GDB       = gdb-multiarch # Debugger

ASFLAGS   = -g # Assembler flags
CFLAGS    = -c -g -O1 -ffreestanding -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 # Compiler flags
LDFLAGS   = -nostdlib -T ld_ram.lds # Linker flags

# Finds all source files in the current directory
SOURCES_C = $(wildcard *.c) # C source files
SOURCES_S = $(wildcard *.s) # Assembly source files

# Convert source files to object files
OBJS	  = $(SOURCES_C:.c=.o) $(SOURCES_S:.s=.o) # Object files

OUTPUT    = main.elf

# Calls the build process
all: $(OUTPUT)

# Compile the files building the output
$(OUTPUT): $(OBJS) #Output depends on object files
	$(CC) $(LDFLAGS) $(OBJS) -o $@

# Starts the J-Link GDB server
connect:
	JLinkGDBServer -device STM32L475VG -endian little -if SWD -speed auto -ir -LocalhostOnly

# Starts GDB and connects to the GDB server
startgdbserver: $(OUTPUT) #the action depends on the executable
	$(GDB) -x stm32-4se.gdb $(OUTPUT) 

#Removes object files and the output
clean:
	rm -f $(OBJS) $(OUTPUT)

	 
