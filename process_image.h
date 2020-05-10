//#ifndef PROCESS_IMAGE_H
//#define PROCESS_IMAGE_H
//
//float get_distance_cm(void);
//uint16_t get_line_position(void);
//void process_image_start(void);
//
//#endif /* PROCESS_IMAGE_H */

uint8_t rgb_to_hsv(uint8_t r, uint8_t g, uint8_t b);
uint8_t max_value(uint8_t a, uint8_t b, uint8_t c);
uint8_t min_value(uint8_t a, uint8_t b, uint8_t c);
uint8_t get_color(void);
void process_image_start(void);

#endif /* PROCESS_IMAGE_H */