LIBS=-ltelldus-core
BINDIR=$(DESTDIR)/usr/bin

tdlisten: tdlisten.c
	gcc -o tdlisten $(LIBS) tdlisten.c

install: tdlisten
	install -d $(BINDIR)
	install tdlisten $(BINDIR)

