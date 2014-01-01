LIBS=-ltelldus-core
tdlisten: tdlisten.c
	gcc -o tdlisten $(LIBS) tdlisten.c

install: tdlisten
	install tdlisten $(DESTDIR)/usr/bin

