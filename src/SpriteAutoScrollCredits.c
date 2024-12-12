#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Scroll.h"

typedef struct {
	COMMON_FIELDS_T common;
	UINT32 y_scroll;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	memset((CUSTOM_DATA*)(THIS->custom_data), 0, CUSTOM_DATA_SIZE);

	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	THIS->y = 40;
	data->y_scroll = (UINT32)(THIS->y) << 6;
	data->common.vy = 20; 
	THIS->visible = 0;
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	if (THIS->y < scroll_y + SCREEN_HEIGHT){
		data->y_scroll += data->common.vy;
		THIS->y = (UINT16)(data->y_scroll >> 6);
	}else{
		SpriteManagerRemove(THIS_IDX);
		return;
	}
}

void DESTROY(void) { 
	SetState(StateMenu);
}