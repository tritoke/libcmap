#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "cmap.h"

struct colourmap * read_map(const char * mapfile) {
	/*
	 * Read a colourmap file into a colourmap struct
	 */

	/* check we actually have a file */
	struct stat mapfile_stat;
	stat(mapfile, &mapfile_stat);
	if (!S_ISREG(mapfile_stat.st_mode)) {
		fputs("Mapfile is not a regular file, exiting.\n", stderr);
		exit(EXIT_FAILURE);
	}

	/* Set up the file IO */
	FILE * fp;
	if ((fp = fopen(mapfile, "r")) == NULL) {
		fputs("Failed to open mapfile, exiting.\n", stderr);
		exit(EXIT_FAILURE);
	}

	/* Initialise the pixel we will read items into */
	size_t currlim = 256;
	Pixel * pixels = calloc(currlim, sizeof(Pixel));

	/* Parse the contents of the file */
	char * buf = NULL;
	size_t n = 0,
				 lineno = 0,
				 colourno = 0;
	ssize_t chars_read;
	while ((chars_read = getline(&buf, &n, fp)) != -1) {
		if (currlim == colourno) {
			/* Pixels array is full, double the space we have allocated */
			currlim <<= 2;
			if ((pixels = reallocarray(pixels, currlim, sizeof(Pixel))) == NULL) {
				goto reallocarray_error;
			}
		}

		int items_parsed;
		Pixel * pixel = &pixels[colourno];
 		/* lines starting with a hashtag are colourcodes */
		if (buf[0] == '#') {
			if (!isgraph(buf[4])) { /* short format hexcode #abc */
				items_parsed = sscanf(buf, "#%1hx%1hx%1hx", &pixel->red, &pixel->green, &pixel->blue);
				pixel->red   |= pixel->red   << 4;
				pixel->green |= pixel->green << 4;
				pixel->blue  |= pixel->blue  << 4;
			} else { /* long formax hex code #aabbcc */
				items_parsed = sscanf(buf, "#%2hx%2hx%2hx", &pixel->red, &pixel->green, &pixel->blue);
			}

			if (items_parsed != 3) {
				/* parse error */
				fprintf(stderr, "[read_map]\tFailed to parse line %ld of colourmap file %s\n", lineno, mapfile);
				exit(EXIT_FAILURE);
			}

			/* Set pixel as opaque */
			pixels[colourno++].alpha = UINT16_MAX;
		}

		lineno++;
	}

	/* Check errno after getline fails */
	if (errno == ENOMEM || errno == EINVAL) {
		fputs("[read_map]\tgetline failed to read line\n", stderr);
		exit(EXIT_FAILURE);
	}

	/* free the buffer used while reading in the lines */
	free(buf);

	/* close the file */
	fclose(fp);

	/* trim off the unused space in pixels array */
	if ((pixels = reallocarray(pixels, colourno, sizeof(Pixel))) == NULL) {
reallocarray_error:
		fputs("[read_map]\treallocarray failed to resize the pixels array -- out of memory", stderr);
		exit(EXIT_FAILURE);
	}

	/* Create the colourmap */
	struct colourmap * cmap = calloc(1, sizeof(struct colourmap));
	cmap->size = colourno;
	cmap->colours = pixels;
	return cmap;
}

void free_cmap(struct colourmap * cmap) { 
	/*
	 * Free the colourmap struct
	 */
	free(cmap->colours);
	free(cmap);
}
