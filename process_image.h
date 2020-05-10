#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H

//camera configs
#define LINE				50
#define R_GAIN				94
#define G_GAIN				64
#define B_GAIN				93
#define CONTRAST			42
//masks
#define R_MASK				0xF8
#define G_MASK1				0x07
#define G_MASK2				0x1F
#define B_MASK				0xE0
//colors
#define BLACK				0
#define YELLOW				1
#define RED					2
//other values
#define MAX_PIXEL			255
#define DEC_RANGE5			31
#define DEC_RANGE6			63
#define LINE_WIDTH			40

uint8_t rgb_color(uint8_t r, uint8_t g, uint8_t b);
uint8_t max_value(uint8_t a, uint8_t b, uint8_t c);
uint8_t get_color(void);
void process_image_start(void);

#endif /* PROCESS_IMAGE_H */
