include Make/files.mk 
include Make/flags.mk
include Make/tools.mk

all: fmt comptests $(ISO) mbtest

$(ISO): $(ELF)
	@$(SAY) "$(MAGENTA)[CP]$(CLEAR)"
	@$(CPY) $(ELF) $(ELFTARG)

	@$(SAY) "$(MAGENTA)[CP]$(CLEAR)"
	@$(CPY)  $(GRUBCFG) $(GRUBCFGPATH)
	@$(SAY) "$(MAGENTA)[GRUBMK] $@$(CLEAR)"
	@$(GRUBMK) -o $@ iso $(SILENCEALL)


$(ELF): $(OBJS)
	@$(SAY) "$(YELLOW)[LD] $@$(CLEAR)"
	@$(LD) $(LDFLAGS) -T $(LS) -o $@ $^


objs/%.o: %.c
	@$(SAY) "$(GREEN)[CC] $<$(CLEAR)"
	@$(CC) $(CCFLAGS) -c $< -o $@ -MF $@.d


objs/%.o: %.asm
	@$(SAY) "$(RED)[NASM] $<$(CLEAR)"
	@$(NASM) $(NASMFLAGS) $< -o $@

-include $(DEPFILES)

.PHONY: all

include Make/fakes.mk