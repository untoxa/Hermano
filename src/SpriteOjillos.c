#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"

typedef struct {
	COMMON_FIELDS_T common;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	THIS->attr_add |= S_PALETTE;
	THIS->lim_y = 10;
	SetFrame(THIS, 0);
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->common.estado ++;
	
	if (data->common.estado > 110) data->common.estado = 0;
	
	if(data->common.estado < 100) {
		THIS->y = 144;
	} else {
		THIS->y = 38;
	}
}

void DESTROY(void) { 
}