#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Scroll.h"
#include "Print.h"
typedef struct {
	COMMON_FIELDS_T common;
	UINT16 init_y;
	INT16  platform_y;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;

	data->common.estado = 1; //Necesario para las colisiones
	data->init_y = THIS->y;
	data->platform_y = THIS->y << 6;
	data->common.vy = -32;
	
	THIS->lim_x = 16;
	THIS->lim_y = 32;
	
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->platform_y += data->common.vy;
	
	THIS->y = data->platform_y >> 6;
	
	if (THIS->y < data->init_y - 40 || THIS->y > data->init_y + 40) data->common.vy = - data->common.vy;
	
	DPRINT_POS(0, 0);
	DPrintf("x:%d y:%d  ", THIS->x, THIS->y);
}

void DESTROY(void) { 
}