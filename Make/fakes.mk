include Make/files.mk
include Make/flags.mk
include Make/tools.mk

clean:
	@echo "[RM] $(CLNTARGS)"
	@rm -f $(CLNTARGS)

test:
	@echo "[QEMU]"
	@$(QEMU) \
		-machine pc,accel=tcg \
		-drive file=mykern.iso,format=raw,media=cdrom,if=ide,index=2 \
		-drive file=fat16.img,format=raw,if=ide,index=0 \
		-boot d \
		-gdb tcp::1234 \
		-monitor stdio \
		-qmp tcp:localhost:4444,server,nowait \
		-no-reboot -no-shutdown
git: clean
	git add .
	git commit $(MSG) $(SILENCE)
	git push $(GITREM) $(SILENCE)

comptests:
	cd tsts && ./tstall.sh $(NASM) $(CC)

mbtest:
	./tsts/multiboot-check.sh


fmt_heads=$(patsubst src/%,/Users/william/coding/wkern/src/%, $(HEADS))
fmt_srcs=$(patsubst src/%,/Users/william/coding/wkern/src/%, $(SRCS))
fmt:
	@echo "[FMT]"
	@clang-format -i $(fmt_heads) $(fmt_srcs)


.PHONY: clean test git comptests fmt