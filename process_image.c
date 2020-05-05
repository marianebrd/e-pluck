#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <usbcfg.h>

#include <main.h>
#include <camera/po8030.h>

#include <process_image.h>

//semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);

static THD_WORKING_AREA(waCaptureImage, 256);
static THD_FUNCTION(CaptureImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	//Takes pixels 0 to IMAGE_BUFFER_SIZE of the line 10 + 11 (minimum 2 lines because reasons)
	po8030_advanced_config(FORMAT_RGB565, 0, 10, IMAGE_BUFFER_SIZE, 2, SUBSAMPLING_X1, SUBSAMPLING_X1);
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
	uint8_t image[IMAGE_BUFFER_SIZE] = {0};
	uint8_t image_r[IMAGE_BUFFER_SIZE] = {0};
	uint8_t image_g[IMAGE_BUFFER_SIZE] = {0};
	uint8_t image_b[IMAGE_BUFFER_SIZE] = {0};

	uint8_t buffer1[IMAGE_BUFFER_SIZE] = {0};
	uint8_t buffer2[IMAGE_BUFFER_SIZE] = {0};

	bool send_to_computer = true;

    while(1){
    	//waits until an image has been captured
        chBSemWait(&image_ready_sem);
		//gets the pointer to the array filled with the last image in RGB565
		img_buff_ptr = dcmi_get_last_image_ptr();

		//Extracts RGB pixels
		for(uint16_t i = 0 ; i < (2 * IMAGE_BUFFER_SIZE) ; i++){
			if(i%2){
				image_r[i/2] = (uint8_t)img_buff_ptr[i]&(0xF8 >> 3); //red mask
				buffer1[i/2] = (uint8_t)img_buff_ptr[i]&(0x07 << 3); //green mask on first line
			}
			else{
				image_b[i/2+1] = (uint8_t)img_buff_ptr[i]&0x1F; //blue mask
				buffer2[i/2+1] = (uint8_t)img_buff_ptr[i]&(0xE0 >> 5); //green mask on second line
			}

			image_g[i/2] = (buffer1[i/2] + buffer2[i/2+1])*(32/64); //conversion 6 bits on 5 bits
		}

		//Read the tab
		for (uint16_t i = 0 ; i < IMAGE_BUFFER_SIZE ; i++){
			image[i] = rgb_to_hsv(image_r[i], image_g[i], image_b[i]);
		}

		if(send_to_computer){
			//sends to the computer the image
			SendUint8ToComputer(image, IMAGE_BUFFER_SIZE);
		}
		//invert the bool
		send_to_computer = !send_to_computer;
    }
}

uint8_t rgb_to_hsv(uint8_t r, uint8_t g, uint8_t b){
	// RGB values are divided by 255
	// to change the range from 0..255 to 0..1:
	uint8_t h, s, v;
	r /= 255;
	g /= 255;
	b /= 255;
	uint8_t cmax = max_value(r,g,b);
	uint8_t cmin = min_value(r,g,b);
	uint8_t diff = cmax-cmin;
	uint8_t color;

	// compute h
	if (cmax == cmin)
	      h = 0;
	else if (cmax == r)
	      h = (60 * ((g - b) / diff) + 360) % 360;
	else if (cmax == g)
	      h = (60 * ((b - r) / diff) + 120) % 360;
	else if (cmax == b)
	      h = (60 * ((r - g) / diff) + 240) % 360;

	// compute s
	if (cmax == 0)
	      s = 0;
	else
	      s = (diff / cmax) * 100;

	// compute v
	v = cmax * 100;

	if (v > 75){
		if (s > 70){
			if ((h < 20/360) && (h > 340/360))
				color = 0;
			else if ((h > 80/360) && (h < 160/360))
				color = 1;
			else if ((h > 200/360) && (h < 260/360))
				color = 2;
		}
	}

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

uint8_t min_value(uint8_t a, uint8_t b, uint8_t c){
	uint8_t min = 0;
	if (a < b){
		min = a;
		if (c < min)
			min = c;
	}
	else{
		min = b;
		if(c < min)
			min = c;
	}

	return min;
}

void process_image_start(void){
	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
}
