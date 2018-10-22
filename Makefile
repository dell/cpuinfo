all: cpuinfo

cpuinfo: cpuinfo.c
	$(CC) $(CFLAGS)-o $@ $<

install:
	cp cpuinfo /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/cpuinfo

clean:
	rm -f cpuinfo
