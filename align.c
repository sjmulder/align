/* Copyright (c) 2020, Sijmen J. Mulder (see LICENSE.md) */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <err.h>

static FILE *copytmp(FILE *);
static int maxwidth(FILE *);
static void center(FILE *, FILE *, int, int);

int
main(int argc, char *argv[])
{
	int c;
	int inwidth;
	int outwidth = -1;
	char *str;
	FILE *input = stdin;

#if __OpenBSD__
	if (pledge("stdio tmppath", NULL) == -1)
		err(1, "pledge");
#endif

	while ((c = getopt(argc, argv, "w:")) != -1)
		switch (c) {
			case 'w': outwidth = atoi(optarg); break;
			default: return -1;
		}

	if (argc > optind) {
		fprintf(stderr, "usage: center [-w width]\n");
		return -1;
	}

	/*
	 * Use $COLUMNS as terminal width if no -w given, or 80 if
	 * unset.
	 */
	if (outwidth < 0 && (str = getenv("COLUMNS")))
		outwidth = strtol(str, NULL, 10);
	if (outwidth < 0)
		outwidth = 80;

	/*
	 * We're making 2 passes through the file, first to measure
	 * line lengths, then for the output, so we must be able to
	 * rewind. Test that. If we can't, copy the stream to a
	 * temporary file.
	 */
	if (fseek(input, SEEK_SET, 0) == -1)
		input = copytmp(input);

	inwidth = maxwidth(input);
	if (fseek(input, SEEK_SET, 0) == -1)
		err(1, NULL);

	center(input, stdout, inwidth, outwidth);

	return 0;
}

/*
 * Copies the given file stream to a temporary, unlinked file and
 * returns a handle to that file at pos 0.
 */
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

/*
 * Goes through the given file, starting at the current position, and
 * returns the character count of the longest line. Every grapheme
 * is considered a single character. Does not rewind the file.
 */
static int
maxwidth(FILE *f)
{
	int c, w = 0, mw = 0;

	while ((c = fgetc(f)) != EOF)
		switch (c) {
			case '\n': if (w > mw) mw = w; w = 0; break;
			case '\t': w = (w+8)/8 * 8; break;
			default: w++; break;
		}
	if (ferror(f))
		err(1, NULL);

	return mw;
}

/*
 * Goes through the given input file and copies it to the given output
 * file using spaces for indentation to center the text of width
 * inwidth in a container of outwidth. Tabs are expanded.
 */
static void
center(FILE *in, FILE *out, int inwidth, int outwidth)
{
	int i, c=0;

	while ((c = getc(in)) != EOF) {
		if (c == '\n') {
			fputc('\n', out);
			continue;
		}

		for (i = 0; i < (outwidth-inwidth)/2; i++)
			fputc(' ', out);
		do {
			if (c == '\t')
				fputs("        ", out);
			else
				fputc(c, out);
		} while (c != '\n' && (c = fgetc(in)) != EOF);
	}
}
