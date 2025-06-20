include Make/files.mk 
include Make/flags.mk
include Make/tools.mk

all: fmt comptests $(ISO) mbtest

$(ISO): $(ELF)
	@echo "$(MAGENTA)[CP]$(CLEAR)"
	@cp $(ELF) iso/boot/$(ELF)

	@echo "$(MAGENTA)[CP]$(CLEAR)"
	@cp grub/grub.cfg iso/boot/grub/grub.cfg

	@echo "$(MAGENTA)[GRUBMK] $@$(CLEAR)"
	@$(GRUBMK) -o $@ iso $(SILENCEALL)


$(ELF): $(OBJS)
	@echo "$(YELLOW)[LD] $@$(CLEAR)"
	@$(LD) $(LDFLAGS) -o $@ $^


objs/%.o: %.c
	@echo "$(GREEN)[CC] $<$(CLEAR)"
	@$(CC) $(CCFLAGS) -c $< -o $@ -MF $@.d


objs/%.o: %.asm
	@echo "$(RED)[NASM] $<$(CLEAR)"
	@$(NASM) $(NASMFLAGS) $< -o $@

-include $(DEPFILES)

.PHONY: all

include Make/fakes.mk