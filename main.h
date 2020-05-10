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
#define MOVE_OFFSET_CORR		11	// corrige la distance d'approche (variable b dans ax - b)
#define MOVE_SLOPE_CORR			0.1f // corrige la distance d'approche (variable a dans ax - b)
#define ANGLE_CORR				1.02 // correction de l'angle de rotation lors de l'approche
#define THREE_QUARTERS_TURN		270 // degrees
#define QUARTER_TURN			90 // degrees
#define DISTANCE_TO_BASKET 		24 // cm
#define SHORT_PAUSE				100 // ms
#define MIDDLE_PAUSE			500 // ms
#define LONG_PAUSE				1000 // ms
#define FRAME					3 // cm (half length of the e-puck frame)

/** Robot wide IPC bus. */
extern messagebus_t bus;

extern parameter_namespace_t parameter_root;

void SendUint8ToComputer(uint8_t* data, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif
