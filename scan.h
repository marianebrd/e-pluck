/*
 * source.h
 *
 *  Created on: 6 mai 2020
 *      Author: Elio Ovide Sanchez
 */

#ifndef SCAN_H_
#define SCAN_H_

#include <stdlib.h>
#include <stdbool.h>

#define SCAN_ANGLE					180	// amplitude du scan [deg]
#define SCAN_RATE					3.8f // angle effectué par un a-coup lors du scan
#define NB_TREES_MAX				3	// nombre max d'arbres disposés sur le terrain
#define TREE_DISTANCE_MAX			250 // mm (from TOF o tree)
#define ANGLE_MIN_BETWEEN_TREES 	20 	// minimal angle between two trees when scaning [deg]
#define NB_LIGNS_TOF_TAB			48 // division entiere de SCAN_ANGLE/SCAN_RATE (à modifier si necessaire!)

uint16_t* scaning(void);

#endif /* SCAN_H_ */
