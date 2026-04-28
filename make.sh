#!/bin/bash
set -e

OUTPUT=katawa

# --- Config ---
CC=mipsel-linux-gnu-gcc
OBJCOPY=mipsel-linux-gnu-objcopy

CFLAGS="
-march=mips1 -mabi=32 -EL
-mno-gpopt -mno-abicalls -fno-pic -mno-shared -mfp32
-fno-builtin -fno-strict-aliasing -fno-stack-protector
-ffreestanding -ffunction-sections -fomit-frame-pointer
-Wno-attributes -G0 -O0 -g
-I./psyq/include -I./nugget
"

LDFLAGS="
-nostdlib -static
-T./nugget/default.ld
-T./nugget/ps-exe.ld
-L./psyq/lib
-Wl,--gc-sections
-Wl,--oformat=elf32-tradlittlemips
-march=mips1 -mabi=32 -EL
-mno-abicalls -fno-pic -mno-shared -mfp32
-ffreestanding
"

LIBS="
-Wl,--start-group
-lapi -lc -lc2 -lcard -lcomb -lds -letc -lgpu -lgs -lgte
-lgun -lhmd -lmath -lmcrd -lmcx -lpad -lpress -lsio
-lsnd -lspu -ltap -lcd
-Wl,--end-group
"

# --- Compile ---
$CC $CFLAGS -c ./nugget/common/crt0/crt0.s -o bin/crt0.o

CFILES=(*.c)
OBJFILES=()

for i in "${CFILES[@]}"; do
    $CC $CFLAGS -c "$i" -o "bin/${i%.c}.obj"
    OBJFILES+=("bin/${i%.c}.obj")
done

# --- Link ---
$CC bin/crt0.o "${OBJFILES[@]}" -o "bin/$OUTPUT.elf" \
    $LDFLAGS $LIBS

# --- Convert to PS-X EXE ---
$OBJCOPY -O binary "bin/$OUTPUT.elf" CDROM/SCES_313.37

echo "---------------------------"
echo "Build complete: $OUTPUT.ps-exe"
echo "---------------------------"

python3 buildXML.py -cd CDROM -proj katawa -pub 4LeafStudios -v KatawaShoujo
mkpsxiso -y ./CDROM/isoconfig.xml

echo "---------------------------"
echo "CDROM created: $OUTPUT.cue"
echo "---------------------------"
