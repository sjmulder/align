PREFIX?=	/usr/local
MANDIR?=	man

CFLAGS+=	-Wall -Wextra
LDLIBS+=	-licuio

all: center

clean:
	rm -f center

install: all
	install -d ${DESTDIR}${PREFIX}/bin
	install -d ${DESTDIR}${PREFIX}/${MANDIR}/man1
	install -m755 center ${DESTDIR}${PREFIX}/bin
	install -m644 center.1 ${DESTDIR}${PREFIX}/${MANDIR}/man1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/center
	rm -f ${DESTDIR}${PREFIX}/${MANDIR}/man1/center.1

.PHONY: all clean install uninstall
