HEADERS = $(wildcard ugame/*.h)

PREFIX ?= /usr/local
INCLUDEDIR ?= $(DESTDIR)$(PREFIX)/include

# Test program.
test: test.c $(HEADERS)
	gcc -o test test.c -fsanitize=address

.PHONY: clean
clean:
	rm test

# Install/Uninstall.
.PHONY: install uninstall
install:
	mkdir -p $(INCLUDEDIR)/ugame
	cp -f $(HEADERS) $(INCLUDEDIR)/ugame/

uninstall:
	rm -rf $(INCLUDEDIR)/ugame
