#include <stdint.h> 
/* Definition of a pixel value in farbfeld image spec */
typedef struct {
	uint16_t red;
	uint16_t green;
	uint16_t blue;
	uint16_t alpha;
} Pixel;

struct colourmap {
	size_t size;
	Pixel * colours;
};

struct colourmap * read_map(const char *);
void free_cmap(struct colourmap *);
struct colourmap * gen_random_map(size_t size);
