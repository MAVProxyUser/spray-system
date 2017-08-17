#ifndef __CFG_DECODER_H
#define __CFG_DECODER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t     step;
	uint16_t    idx;
    uint16_t    len;
    uint16_t    max;
    uint8_t     ver;
	uint8_t     *p_data;
} cfg_decoder_object_t;

void cfg_decoder_init( cfg_decoder_object_t *obj, uint8_t *p, uint16_t max );
bool cfg_decoder_upack( cfg_decoder_object_t *obj, uint8_t data );
void  Flow_Sensor_Unpack( cfg_decoder_object_t *obj, uint8_t data );
uint8_t get_control_flag(void);
#endif
