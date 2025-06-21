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


mbtest:
	@echo "$(BLUE)[EXEC TEST] multiboot-check$(CLEAR)"
	@./tsts/multiboot-check.sh


fmt:
	@echo "$(CYAN)[FMT]$(CLEAR)"
	@clang-format -i $(FMTHEADS) $(FMTSRCS)

didconf:
	@if [ -f ranconf ]; then \
		echo "Already Ran Configure"; \
	else \
		echo "Running Configure"; \
		./configure.sh; \
	fi

.PHONY: clean test git comptests fmt didconf