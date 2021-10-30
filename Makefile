# https://stackoverflow.com/questions/15189704/makefile-removes-object-files-for-no-reason
# https://www.gnu.org/software/make/manual/html_node/Chained-Rules.html
# https://unix.stackexchange.com/questions/517190/what-causes-make-to-delete-intermediate-files


all: mylinker hello.out

.PRECIOUS: %.elf %.textbin

mylinker: mylinker.c
	gcc -o $@ $^

%.elf : %.c startup.c
	gcc -masm=intel -nostdlib -nostartfiles -nostdinc -static-pie -T linker.lds -Os -g -fno-reorder-functions -o $@ $^

%.textbin: %.elf
	objcopy -O binary -j .text $^ $@

%.out: %.textbin
	./mylinker < $^ > $@ && chmod +x $@

.PHONY: clean
clean:
	rm -f *.textbin *.elf *.out mylinker

