#variables
PREFIX    = arm-none-eabi-
CC        = $(PREFIX)gcc # Compiler
AS        = $(PREFIX)as # Assembler
GDB       = gdb-multiarch # Debugger
OBJCOPY   = arm-none-eabi-objcopy # Convert .raw into .obj files

CMSIS_DEV = CMSIS/Device/ST/STM32L4xx/Include
CMSIS_INC = CMSIS/Include


ASFLAGS   = -g -mcpu=cortex-m4 -mthumb # Assembler flags
CFLAGS    = -g -O1 -ffreestanding -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
			-I$(CMSIS_DEV) -I$(CMSIS_INC) # Compiler flags
LDFLAGS   = -nostdlib -T ld_ram.lds # Linker flags

# Finds all source files in the current directory
SOURCES_C = $(wildcard *.c) # C source files
SOURCES_S = $(wildcard *.s) # Assembly source files
SOURCES_RAW = $(wildcard *.raw) # Raw source files

# Convert source files to object files
OBJS	  = $(SOURCES_C:.c=.o) $(SOURCES_S:.s=.o) $(SOURCES_RAW:.raw=.o) # Object files

OUTPUT    = main.elf

# Calls the build process
all: $(OUTPUT)

# Compile the files building the output
$(OUTPUT): $(OBJS) #Output depends on object files
	$(CC) $(LDFLAGS) $(OBJS) -o $@

# Rule to convert .raw files to .o files
%.o: %.raw
	$(OBJCOPY) -I binary -O elf32-littlearm -B arm $< $@

# Starts the J-Link GDB server
connect:
	JLinkGDBServer -device STM32L475VG -endian little -if SWD -speed auto -ir -LocalhostOnly

# Starts GDB and connects to the GDB server
startgdbserver: $(OUTPUT) #the action depends on the executable
	$(GDB) -x stm32-4se.gdb $(OUTPUT) 

#Removes object files and the output
clean:
	rm -f $(OBJS) $(OUTPUT)

	 
