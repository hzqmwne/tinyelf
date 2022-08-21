# https://stackoverflow.com/questions/15189704/makefile-removes-object-files-for-no-reason
# https://www.gnu.org/software/make/manual/html_node/Chained-Rules.html
# https://unix.stackexchange.com/questions/517190/what-causes-make-to-delete-intermediate-files


all: mylinker mylinker_tiny hello.out   # minsha.tiny minsha2.tiny shms.tiny shmc.tiny

.PRECIOUS: %.elf %.textbin %.elf_nostart %.textbin_nostart


mylinker: mylinker.c
	gcc -o $@ $^


%.elf : %.c startup.c
	gcc -masm=intel -nostdlib -nostartfiles -nostdinc -static-pie -T linker.lds -Os -g -fno-reorder-functions -o $@ $^

%.textbin: %.elf
	objcopy -O binary -j .text $^ $@

%.out: %.textbin
	./mylinker < $^ > $@ && chmod +x $@


mylinker_tiny: mylinker_tiny.c
	gcc -o $@ $^

%.elf_nostart : %.c
	gcc -masm=intel -nostdlib -nostartfiles -nostdinc -static-pie -T linker_nostart.lds -Os -g -fno-reorder-functions -o $@ $^

%.textbin_nostart: %.elf_nostart
	objcopy -O binary -j .text $^ $@

%.tiny: %.textbin_nostart
	./mylinker_tiny < $^ > $@ && chmod +x $@


.PHONY: clean
clean:
	rm -f *.textbin *.elf *.out mylinker
	rm -f *.textbin_nostart *.elf_nostart *.tiny mylinker_tiny

