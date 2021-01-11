/* Copyright (c) 2020, Sijmen J. Mulder (see LICENSE.md) */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <err.h>

static FILE *copytmp(FILE *);
static int getlnwidth(FILE *);

/* Mode, e.g. M_CENTER | M_BLOCK */
#define M_CENTER 0x01
#define M_RIGHT  0x02
#define M_BLOCK  0x10
#define M_LINE   0x20

int
main(int argc, char *argv[])
{
	int c, n, i;
	int inwidth=0, outwidth=-1;
	int mode=0;
	int npad, fst=1;
	char *str;
	FILE *input = stdin;
	fpos_t pos;

#if __OpenBSD__
	if (pledge("stdio tmppath", NULL) == -1)
		err(1, "pledge");
#endif

	while ((c = getopt(argc, argv, "CcRrw:")) != -1)
		switch (c) {
			case 'C': mode = M_CENTER | M_BLOCK; break;
			case 'c': mode = M_CENTER | M_LINE;  break;
			case 'R': mode = M_RIGHT  | M_BLOCK; break;
			case 'r': mode = M_RIGHT  | M_LINE;  break;
			case 'w': outwidth = atoi(optarg); break;
			default: return -1;
		}

	if (!mode || argc > optind)
		errx(1, "usage: align -C|c|R|r [-w width] <file");

	/* Use $COLUMN as default with, or 80 if unset */
	if (outwidth < 0 && (str = getenv("COLUMNS")))
		outwidth = strtol(str, NULL, 10);
	if (outwidth < 0)
		outwidth = 80;

	/* We must be able to seek */
	if (fseek(input, SEEK_SET, 0) == -1)
		input = copytmp(input);

	if (mode & M_BLOCK) {
		while (!feof(input))
			if ((n = getlnwidth(input)) > inwidth)
				inwidth = n;
		if (fseek(input, SEEK_SET, 0) == -1)
			err(1, NULL);
	}

	while ((c = fgetc(input)) != EOF) {
		if (fst) {
			if (mode & M_LINE) {
				if (fgetpos(input, &pos) == -1)
					err(1, NULL);

				ungetc(c, input);
				inwidth = getlnwidth(input);

				if (fsetpos(input, &pos) == -1)
					err(1, NULL);
			}

			if ((npad = outwidth-inwidth) > 0) {
				if (mode & M_CENTER)
					npad = (npad+1)/2;
				for (i = 0; i < npad; i++)
					fputc(' ', stdout);
			}

			fst = 0;
		}

		switch (c) {
			case '\t': fputs("        ", stdout); break;
			case '\n': fputc(c, stdout); fst=1; break;
			default:   fputc(c, stdout); break;
		}
	}

	return 0;
}

/* Makes a copy of the stream to an unnamed temporary file */
static FILE *
copytmp(FILE *f)
{
	static char buf[4096];
	FILE *tmp;
	size_t nr;

	if (!(tmp = tmpfile()))
		err(1, NULL);
	while ((nr = fread(buf, 1, sizeof(buf), f)))
		if (fwrite(buf, 1, nr, tmp) != nr)
			err(1, NULL);
	if (ferror(f))
		err(1, NULL);
	if (fseek(tmp, SEEK_SET, 0) == -1)
		err(1, NULL);

	return tmp;
}

/* Scans a line and returns its width */
static int
getlnwidth(FILE *f)
{
	int w=0, c;

	while ((c = fgetc(f)) != EOF && c != '\n')
		switch (c) {
			case '\t': w = (w+8)/8 * 8; break;
			default: w++; break;
		}
	if (ferror(f))
		err(1, NULL);

	return w;
}
