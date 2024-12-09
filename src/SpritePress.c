#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"

typedef struct {
	COMMON_FIELDS_T common;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	memset((CUSTOM_DATA*)(THIS->custom_data), 0, CUSTOM_DATA_SIZE);

	THIS->attr_add |= S_PALETTE;
	THIS->lim_y = 10;
	SetFrame(THIS, 0);
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->common.estado ++;
	
	if (data->common.estado > 40) data->common.estado = 0;
	
	if(data->common.estado < 20) {
		THIS->y = 94;
	} else {
		THIS->y = 144;
	}
}

void DESTROY(void) { 
}