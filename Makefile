all: cpuinfo

cpuinfo: cpuinfo.c
	$(CC) $(CFLAGS)-o $@ $<

clean: rm -f cpuinfo
