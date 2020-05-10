#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <usbcfg.h>

#include <main.h>
#include <camera/po8030.h>

#include <process_image.h>

static uint8_t color_number = 0;

//semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);

static THD_WORKING_AREA(waCaptureImage, 256);
static THD_FUNCTION(CaptureImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	//Takes pixels 0 to IMAGE_BUFFER_SIZE of the line 10 + 11 (minimum 2 lines because reasons)
	po8030_advanced_config(FORMAT_RGB565, 0, LINE, IMAGE_BUFFER_SIZE, 2, SUBSAMPLING_X1, SUBSAMPLING_X1);

	po8030_set_rgb_gain(R_GAIN, G_GAIN, B_GAIN); //Sets white balance red, green, blue gain
	po8030_set_contrast(CONTRAST); //Sets the contrast of the camera

	dcmi_enable_double_buffering();
	dcmi_set_capture_mode(CAPTURE_ONE_SHOT);
	dcmi_prepare();

    while(1){
        //starts a capture
		dcmi_capture_start();
		//waits for the capture to be done
		wait_image_ready();
		//signals an image has been captured
		chBSemSignal(&image_ready_sem);
    }
}

static THD_WORKING_AREA(waProcessImage, 1024);
static THD_FUNCTION(ProcessImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	uint8_t *img_buff_ptr;
	static uint8_t image_r[IMAGE_BUFFER_SIZE] = {0};
	static uint8_t image_g[IMAGE_BUFFER_SIZE] = {0};
	static uint8_t image_b[IMAGE_BUFFER_SIZE] = {0};

	static uint8_t buffer1[IMAGE_BUFFER_SIZE] = {0};
	static uint8_t buffer2[IMAGE_BUFFER_SIZE] = {0};

    while(1){
    	//waits until an image has been captured
        chBSemWait(&image_ready_sem);
		//gets the pointer to the array filled with the last image in RGB565
		img_buff_ptr = dcmi_get_last_image_ptr();

		//Extracts RGB pixels
		for(uint16_t i = 0 ; i < (2 * IMAGE_BUFFER_SIZE) ; i++){
			if(i%2 == 0){
				image_r[i/2] = (((uint8_t)img_buff_ptr[i]&R_MASK) >> 3) * MAX_PIXEL / DEC_RANGE5; //red mask
				buffer1[i/2] = ((uint8_t)img_buff_ptr[i]&G_MASK1) << 3; //green mask on first line
			}
			else{
				image_b[(i-1)/2] = ((uint8_t)img_buff_ptr[i]&G_MASK2) * MAX_PIXEL / DEC_RANGE5; //blue mask
				buffer2[(i-1)/2] = ((uint8_t)img_buff_ptr[i]&B_MASK) >> 5; //green mask on second line
			}
		}

		//Links the two buffer with the green pixels values
		for (uint16_t i = 0 ; i < IMAGE_BUFFER_SIZE ; i++)
			image_g[i] = ((buffer1[i] | buffer2[i])) * MAX_PIXEL / DEC_RANGE6; //conversion 6 bits on 5 bits

		uint16_t red = 0;
		uint16_t green = 0;
		uint16_t blue = 0;

		//Averages the RGB values
		for (uint16_t i = ((IMAGE_BUFFER_SIZE/2) - (LINE_WIDTH/2)) ; i < ((IMAGE_BUFFER_SIZE/2) +( LINE_WIDTH/2)) ; i++){
			red += image_r[i];
			green += image_g[i];
			blue += image_b[i];
		}

		rgb_color(red/LINE_WIDTH, green/LINE_WIDTH, blue/LINE_WIDTH);
    }
}

uint8_t rgb_color(uint8_t r, uint8_t g, uint8_t b){
	static uint8_t color = 0;

	volatile uint8_t cmax = max_value(r,g,b);

	if (cmax == r){
		if (g > b)
			color = YELLOW;
		else
			color = RED;
	}
	else
		color = BLACK;

	color_number = color;

	return color;
}

uint8_t max_value(uint8_t a, uint8_t b, uint8_t c){
	uint8_t max = 0;
	if (a > b){
		max = a;
		if (c > max)
			max = c;
	}
	else{
		max = b;
		if(c > max)
			max = c;
	}

	return max;
}

uint8_t get_color(void){
	return color_number;
}

void process_image_start(void){
	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
}
