//#include "ch.h"
//#include "hal.h"
//#include <chprintf.h>
//#include <usbcfg.h>
//
//#include <main.h>
//#include <camera/po8030.h>
//
//#include <process_image.h>
//
//
////static float distance_cm = 0;
////static uint16_t line_position = IMAGE_BUFFER_SIZE/2;	//middle
////
//////semaphore
////static BSEMAPHORE_DECL(image_ready_sem, TRUE);
////
/////*
//// *  Returns the line's width extracted from the image buffer given
//// *  Returns 0 if line not found
//// */
////uint16_t extract_line_width(uint8_t *buffer){
////
////	uint16_t i = 0, begin = 0, end = 0, width = 0;
////	uint8_t stop = 0, wrong_line = 0, line_not_found = 0;
////	uint32_t mean = 0;
////
////	static uint16_t last_width = PXTOCM/GOAL_DISTANCE;
////
////	//performs an average
////	for(uint16_t i = 0 ; i < IMAGE_BUFFER_SIZE ; i++){
////		mean += buffer[i];
////	}
////	mean /= IMAGE_BUFFER_SIZE;
////
////	do{
////		wrong_line = 0;
////		//search for a begin
////		while(stop == 0 && i < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE))
////		{
////			//the slope must at least be WIDTH_SLOPE wide and is compared
////		    //to the mean of the image
////		    if(buffer[i] > mean && buffer[i+WIDTH_SLOPE] < mean)
////		    {
////		        begin = i;
////		        stop = 1;
////		    }
////		    i++;
////		}
////		//if a begin was found, search for an end
////		if (i < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE) && begin)
////		{
////		    stop = 0;
////
////		    while(stop == 0 && i < IMAGE_BUFFER_SIZE)
////		    {
////		        if(buffer[i] > mean && buffer[i-WIDTH_SLOPE] < mean)
////		        {
////		            end = i;
////		            stop = 1;
////		        }
////		        i++;
////		    }
////		    //if an end was not found
////		    if (i > IMAGE_BUFFER_SIZE || !end)
////		    {
////		        line_not_found = 1;
////		    }
////		}
////		else//if no begin was found
////		{
////		    line_not_found = 1;
////		}
////
////		//if a line too small has been detected, continues the search
////		if(!line_not_found && (end-begin) < MIN_LINE_WIDTH){
////			i = end;
////			begin = 0;
////			end = 0;
////			stop = 0;
////			wrong_line = 1;
////		}
////	}while(wrong_line);
////
////	if(line_not_found){
////		begin = 0;
////		end = 0;
////		width = last_width;
////	}else{
////		last_width = width = (end - begin);
////		line_position = (begin + end)/2; //gives the line position.
////	}
////
////	//sets a maximum width or returns the measured width
////	if((PXTOCM/width) > MAX_DISTANCE){
////		return PXTOCM/MAX_DISTANCE;
////	}else{
////		return width;
////	}
////}
////
////static THD_WORKING_AREA(waCaptureImage, 256);
////static THD_FUNCTION(CaptureImage, arg) {
////
////    chRegSetThreadName(__FUNCTION__);
////    (void)arg;
////
////	//Takes pixels 0 to IMAGE_BUFFER_SIZE of the line 10 + 11 (minimum 2 lines because reasons)
////	po8030_advanced_config(FORMAT_RGB565, 0, 10, IMAGE_BUFFER_SIZE, 2, SUBSAMPLING_X1, SUBSAMPLING_X1);
////	dcmi_enable_double_buffering();
////	dcmi_set_capture_mode(CAPTURE_ONE_SHOT);
////	dcmi_prepare();
////
////    while(1){
////        //starts a capture
////		dcmi_capture_start();
////		//waits for the capture to be done
////		wait_image_ready();
////		//signals an image has been captured
////		chBSemSignal(&image_ready_sem);
////    }
////}
////
////
////static THD_WORKING_AREA(waProcessImage, 1024);
////static THD_FUNCTION(ProcessImage, arg) {
////
////    chRegSetThreadName(__FUNCTION__);
////    (void)arg;
////
////	uint8_t *img_buff_ptr;
////	uint8_t image[IMAGE_BUFFER_SIZE] = {0};
////	uint16_t lineWidth = 0;
////
////	bool send_to_computer = true;
////
////    while(1){
////    	//waits until an image has been captured
////        chBSemWait(&image_ready_sem);
////		//gets the pointer to the array filled with the last image in RGB565
////		img_buff_ptr = dcmi_get_last_image_ptr();
////
////		//Extracts only the red pixels
////		for(uint16_t i = 0 ; i < (2 * IMAGE_BUFFER_SIZE) ; i+=2){
////			//extracts first 5bits of the first byte
////			//takes nothing from the second byte
////			image[i/2] = (uint8_t)img_buff_ptr[i]&0xF8;
////		}
////
////		//search for a line in the image and gets its width in pixels
////		lineWidth = extract_line_width(image);
////
////		//converts the width into a distance between the robot and the camera
////		if(lineWidth){
////			distance_cm = PXTOCM/lineWidth;
////		}
////
////		if(send_to_computer){
////			//sends to the computer the image
////			SendUint8ToComputer(image, IMAGE_BUFFER_SIZE);
////		}
////		//invert the bool
////		send_to_computer = !send_to_computer;
////    }
////}
////
////float get_distance_cm(void){
////	return distance_cm;
////}
////
////uint16_t get_line_position(void){
////	return line_position;
////}
////
////void process_image_start(void){
////	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
////	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
////}
//
//
//
//
//static float distance_cm = 0;
//static uint16_t apple_position = IMAGE_BUFFER_SIZE/2;	//middle
//
////semaphore
//static BSEMAPHORE_DECL(image_ready_sem, TRUE);
//
///*
// *  Detects if there is an apple
// *  Returns 0 if apple not found
// */
///*
//uint16_t extract_apple_width(uint8_t *buffer){
//
//	uint16_t i = 0, begin = 0, end = 0, width = 0;
//	uint8_t stop = 0, wrong_apple, apple_not_found = 0;
//	uint32_t mean = 0;
//
//	static uint16_t last_width = PXTOCM/GOAL_DISTANCE;
//
//	//performs an average
//	for(uint16_t i = 0 ; i < IMAGE_BUFFER_SIZE ; i++){
//		mean += buffer[i];
//	}
//	mean /= IMAGE_BUFFER_SIZE;
//
//	do{
//		wrong_apple = 0;
//		//search for a begin
//		while(stop == 0 && i < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE))
//		{
//			//the slope must at least be WIDTH_SLOPE wide and is compared
//		    //to the mean of the image
//		    if(buffer[i] > mean && buffer[i+WIDTH_SLOPE] < mean)
//		    {
//		        begin = i;
//		        stop = 1;
//		    }
//		    i++;
//		}
//		//if a begin was found, search for an end
//		if (i < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE) && begin)
//		{
//		    stop = 0;
//
//		    while(stop == 0 && i < IMAGE_BUFFER_SIZE)
//		    {
//		        if(buffer[i] > mean && buffer[i-WIDTH_SLOPE] < mean)
//		        {
//		            end = i;
//		            stop = 1;
//		        }
//		        i++;
//		    }
//		    //if an end was not found
//		    if (i > IMAGE_BUFFER_SIZE || !end)
//		    {
//		        apple_not_found = 1;
//		    }
//		}
//		else//if no begin was found
//		{
//		    apple_not_found = 1;
//		}
//
//		//if a line too small has been detected, continues the search
//		if(!apple_not_found && (end-begin) < MIN_LINE_WIDTH){
//			i = end;
//			begin = 0;
//			end = 0;
//			stop = 0;
//			wrong_apple = 1;
//		}
//	}while(wrong_apple);
//
//	if(apple_not_found){
//		begin = 0;
//		end = 0;
//		width = last_width;
//	}else{
//		last_width = width = (end - begin);
//		apple_position = (begin + end)/2; //gives the apple position.
//	}
//
//	//sets a maximum width or returns the measured width
//	if((PXTOCM/width) > MAX_DISTANCE){
//		return PXTOCM/MAX_DISTANCE;
//	}else{
//		return width;
//	}
//}
//*/
//
//uint8_t image_g[IMAGE_BUFFER_SIZE] = {0};
//uint8_t image_b[IMAGE_BUFFER_SIZE] = {0};
//
//uint8_t buffer1[IMAGE_BUFFER_SIZE] = {0};
//uint8_t buffer2[IMAGE_BUFFER_SIZE] = {0};
//
//bool send_to_computer = true;
//
//while(1){
//	//waits until an image has been captured
//    chBSemWait(&image_ready_sem);
//	//gets the pointer to the array filled with the last image in RGB565
//	img_buff_ptr = dcmi_get_last_image_ptr();
//
//	//Extracts RGB pixels
//	for(uint16_t i = 0 ; i < (2 * IMAGE_BUFFER_SIZE) ; i++){
//		if(i%2){
//			image_r[i/2] = (uint8_t)img_buff_ptr[i]&(0xF8 >> 3); //red mask
//			buffer1[i/2] = (uint8_t)img_buff_ptr[i]&(0x07 << 3); //green mask on first line
//		}
//		else{
//			image_b[i/2+1] = (uint8_t)img_buff_ptr[i]&0x1F; //blue mask
//			buffer2[i/2+1] = (uint8_t)img_buff_ptr[i]&(0xE0 >> 5); //green mask on second line
//		}
//
//		image_g[i/2] = (buffer1[i/2] + buffer2[i/2+1])*(32/64); //conversion 6 bits on 5 bits
//	}
//
//	//Read the tab
//	for (uint16_t i = 0 ; i < IMAGE_BUFFER_SIZE ; i++){
//		image[i] = rgb_to_hsv(image_r[i], image_g[i], image_b[i]);
//	}
//
//	if(send_to_computer){
//		//sends to the computer the image
//		SendUint8ToComputer(image, IMAGE_BUFFER_SIZE);
//	}
//	//invert the bool
//	send_to_computer = !send_to_computer;
//}
//}
//
//uint8_t rgb_to_hsv(uint8_t r, uint8_t g, uint8_t b){
//// RGB values are divided by 255
//// to change the range from 0..255 to 0..1:
//uint8_t h, s, v;
//r /= 255;
//g /= 255;
//b /= 255;
//uint8_t cmax = max_value(r,g,b);
//uint8_t cmin = min_value(r,g,b);
//uint8_t diff = cmax-cmin;
//uint8_t color;
//
//// compute h
//if (cmax == cmin)
//      h = 0;
//else if (cmax == r)
//      h = (60 * ((g - b) / diff) + 360) % 360;
//else if (cmax == g)
//      h = (60 * ((b - r) / diff) + 120) % 360;
//else if (cmax == b)
//      h = (60 * ((r - g) / diff) + 240) % 360;
//
//// compute s
//if (cmax == 0)
//      s = 0;
//else
//      s = (diff / cmax) * 100;
//
//// compute v
//v = cmax * 100;
//
//if (v > 75){
//	if (s > 70){
//		if ((h < 20/360) && (h > 340/360))
//			color = 0;
//		else if ((h > 80/360) && (h < 160/360))
//			color = 1;
//		else if ((h > 200/360) && (h < 260/360))
//			color = 2;
//	}
//}
//
//return color;
//}
//
//uint8_t max_value(uint8_t a, uint8_t b, uint8_t c){

#include <main.h>
#include <camera/po8030.h>

#include <process_image.h>

static uint8_t color_number = 10;

//semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);

static THD_WORKING_AREA(waCaptureImage, 256);
static THD_FUNCTION(CaptureImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	//Takes pixels 0 to IMAGE_BUFFER_SIZE of the line 10 + 11 (minimum 2 lines because reasons)
	po8030_advanced_config(FORMAT_RGB565, 0, 50, IMAGE_BUFFER_SIZE, 2, SUBSAMPLING_X1, SUBSAMPLING_X1);

	po8030_set_rgb_gain(94, 64, 93); //Sets white balance red, green, blue gain
	po8030_set_contrast(42); //Sets the contrast of the camera

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
	static uint8_t image[IMAGE_BUFFER_SIZE] = {0};

	static uint8_t buffer1[IMAGE_BUFFER_SIZE] = {0};
	static uint8_t buffer2[IMAGE_BUFFER_SIZE] = {0};

	//bool send_to_computer = true;

    while(1){
    	//waits until an image has been captured
        chBSemWait(&image_ready_sem);
		//gets the pointer to the array filled with the last image in RGB565
		img_buff_ptr = dcmi_get_last_image_ptr();

		//Extracts RGB pixels
		for(uint16_t i = 0 ; i < (2 * IMAGE_BUFFER_SIZE) ; i++){
			if(i%2 == 0){
				image_r[i/2] = (((uint8_t)img_buff_ptr[i]&0xF8) >> 3)*255 / 31; //red mask
				buffer1[i/2] = ((uint8_t)img_buff_ptr[i]&0x07) << 3; //green mask on first line
			}
			else{
				image_b[(i-1)/2] = ((uint8_t)img_buff_ptr[i]&0x1F)*255 / 31; //blue mask
				buffer2[(i-1)/2] = ((uint8_t)img_buff_ptr[i]&0xE0) >> 5; //green mask on second line
			}
		}

		//converts the rgb values to hsv
		for (uint16_t i = 0 ; i < IMAGE_BUFFER_SIZE ; i+=2){
			image_g[i] = ((buffer1[i] | buffer2[i]))*255 / 63; //conversion 6 bits on 5 bits
			if(i == 320)
				image[i] = rgb_to_hsv(image_r[i], image_g[i], image_b[i]);
		}
    }
}

uint8_t rgb_to_hsv(uint8_t r, uint8_t g, uint8_t b){
	// RGB values are divided by 255
	// to change the range from 0..255 to 0..1:
	volatile uint16_t h = 0;
	volatile uint8_t s = 0;
	volatile uint8_t v = 0;

	volatile uint8_t cmax = max_value(r,g,b);
	volatile uint8_t cmin = min_value(r,g,b);
	volatile uint8_t diff = cmax-cmin;

	// compute h (range from 0..1)
	if (cmax == cmin)
	      h = 0;
	else if (cmax == r)
	      h = ((60 * ((abs(g - b) * 100) / diff)) + 360) % 360;
	else if (cmax == g)
	      h = ((60 * ((abs(b - r) * 100) / diff)) + 240) % 360;
	else if (cmax == b)
	      h = ((60 * ((abs(r - g) * 100) / diff)) + 120) % 360;

	// compute s (range from 0..100)
	if (cmax == 0)
	      s = 0;
	else
	      s = ((diff * 100) / cmax);

	// compute v (range from 0..100)
	v = (cmax * 100) / 255;

	static uint8_t color = 10;

	if ((s > 45) && (v > 45))
		if ((h < 75) || (h > 200))
			color = 0; //red to yellow
		else color = 1; //other than red or yellow
	else color = 2; //black or white

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

uint8_t get_color(void){
	return color_number;
}

void process_image_start(void){
	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
}
