include Make/files.mk
include Make/flags.mk
include Make/tools.mk

clean:
	@echo "[RM] $(CLNTARGS)"
	@rm -f $(CLNTARGS)

test:
	@echo "[QEMU]"
	@$(QEMU) $(QEMUFLAGS)

git: clean
	git add .
	git commit $(MSG) $(SILENCE)
	git push $(GITREM) $(SILENCE)


comptests:
	@echo "$(BLUE)[EXEC TEST] cc-arch$(CLEAR)"
	@./tsts/cc-arch-check.sh $(CC)

	@echo "$(BLUE)[EXEC TEST] nasm-test$(CLEAR)"
	@./tsts/nasm-sanity-check.sh $(NASM)


mbtest:
	@echo "$(BLUE)[EXEC TEST] multiboot-check$(CLEAR)"
	@./tsts/multiboot-check.sh


fmt:
	@echo "$(CYAN)[FMT]$(CLEAR)"
	@clang-format -i $(fmt_heads) $(fmt_srcs)


.PHONY: clean test git comptests fmt