PREFIX?=	/usr/local
MANDIR?=	man

CFLAGS+=	-Wall -Wextra

all: align

clean:
	rm -f align

install: all
	install -d ${DESTDIR}${PREFIX}/bin
	install -d ${DESTDIR}${PREFIX}/${MANDIR}/man1
	install -m755 align ${DESTDIR}${PREFIX}/bin
	install -m644 align.1 ${DESTDIR}${PREFIX}/${MANDIR}/man1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/align
	rm -f ${DESTDIR}${PREFIX}/${MANDIR}/man1/align.1

.PHONY: all clean install uninstall
