#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
const UINT8 anim_pincho[] = {2, 0, 1};

typedef struct {
	COMMON_FIELDS_T common;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->common.estado = 1; //Necesario para las colisiones
	THIS->lim_x = 255;
	THIS->lim_y = 255;
	THIS->y -= 2;
	data->common.life = 0;
	SetSpriteAnim(THIS, anim_pincho, 12u);
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;	
	if(data->common.estado == 99) {
		THIS->visible = 0;
	}
}

void DESTROY(void) { 
}