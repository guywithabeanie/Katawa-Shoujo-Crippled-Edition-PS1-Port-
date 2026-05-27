# vim: set noet

SHELL := /bin/bash

OUTPUT := katawa

# --- Config ---
CC := mipsel-linux-gnu-gcc
OBJCOPY := mipsel-linux-gnu-objcopy
# make PYTHON="python"
PYTHON := python3
MKPSXISO := mkpsxiso

CFLAGS :=\
-march=mips1 -mabi=32 -EL\
-mno-gpopt -mno-abicalls -fno-pic -mno-shared -mfp32\
-fno-builtin -fno-strict-aliasing -fno-stack-protector\
-ffreestanding -ffunction-sections -fomit-frame-pointer\
-Wno-attributes -G0 -O0 -g\
-I./psyq/include -I./nugget 

LDFLAGS :=\
-nostdlib -static\
-T./nugget/default.ld\
-T./nugget/ps-exe.ld\
-L./psyq/lib\
-Wl,--gc-sections\
-Wl,--oformat=elf32-tradlittlemips\
-march=mips1 -mabi=32 -EL\
-mno-abicalls -fno-pic -mno-shared -mfp32\
-ffreestanding


LIBS :=\
-Wl,--start-group\
-lapi -lc -lc2 -lcard -lcomb -lds -letc -lgpu -lgs -lgte\
-lgun -lhmd -lmath -lmcrd -lmcx -lpad -lpress -lsio\
-lsnd -lspu -ltap -lcd\
-Wl,--end-group

.PHONY: all clean

C_FILES := $(wildcard *.c)
OBJ_FILES := $(addprefix bin/, $(notdir $(C_FILES:.c=.obj)))
OBJS := $(patsubst %.c,bin/%.obj,$(wildcard *.c))
$(info C_FILES = $(C_FILES))
$(info OBJ_FILES = $(OBJ_FILES))
$(info OBJS = $(OBJS))

all: cdrom

bin/crt0.obj: nugget/common/crt0/crt0.s | bin
	$(CC) $(CFLAGS) -c nugget/common/crt0/crt0.s -o bin/crt0.obj

$(OBJS): bin/%.obj: %.c
	$(CC) $(CFLAGS) -c $< -o $@

link: bin/crt0.obj $(OBJ_FILES) | bin
	$(CC) $^ -o bin/$(OUTPUT).elf $(LDFLAGS) $(LIBS)

exe: link | bin
	$(OBJCOPY) -O binary bin/$(OUTPUT).elf CDROM/SCES_313.37
	@echo "Build complete: $(OUTPUT).ps-exe"

cdrom: exe | bin
	$(PYTHON) buildXML.py -c CDROM -o CDROM/isoconfig.xml -prob $(OUTPUT) -pub 4LeafStudios -v KatawaShoujo
	@echo "CDROM created: $(OUTPUT).cue"
	$(MKPSXISO) -y ./CDROM/isoconfig.xml

bin:
	mkdir -p $@

clean: | bin
	rm -rf bin
	rm -f CDROM/isoconfig.xml
