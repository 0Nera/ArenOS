@Echo off


SET CC=i686-elf-gcc

SET CCFLAGS=-g -I include -ffreestanding -Wall -Wextra 
SET LDFLAGS=%CCFLAGS% -nostdlib -lgcc



RMDIR "bin" /S /Q
mkdir bin



%CC% %CCFLAGS% -c kernel.c -o bin/kernel.o

%CC% %CCFLAGS% -c src/vesa.c -o bin/vesa.o

%CC% %CCFLAGS% -c src/starter.s -o bin/starter.o



%CC% %LDFLAGS% -T link.ld -o kernel.elf bin/kernel.o bin/vesa.o bin/starter.o