#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "camera/dcmi_camera.h"
#include "msgbus/messagebus.h"
#include "parameter/parameter.h"


//constants for the differents parts of the project
#define IMAGE_BUFFER_SIZE		640
#define MOVE_OFFSET_CORR		5.0f
#define MOVE_SLOPE_CORR			0.1f
#define ANGLE_CORR				1.02
#define THREE_QUARTERS_TURN		270
#define DISTANCE_TO_BASKET 		20

/** Robot wide IPC bus. */
extern messagebus_t bus;

extern parameter_namespace_t parameter_root;

void SendUint8ToComputer(uint8_t* data, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif
