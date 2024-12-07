#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
const UINT8 anim_antorcha[] = {3, 0, 1, 2};
// const UINT8 anim_apagada[] = {1, 3};

typedef struct {
	COMMON_FIELDS_T common;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	((CUSTOM_DATA*)(THIS->custom_data))->common.estado = 1; //Necesario para las colisiones
	((CUSTOM_DATA*)(THIS->custom_data))->common.life = 0;
	THIS->lim_x = 64;
	THIS->lim_y = 8;
	THIS->x += 2;
	THIS->y += 8;
	SetSpriteAnim(THIS, anim_antorcha, 10u);
}

void UPDATE(void) {	
	if (((CUSTOM_DATA*)(THIS->custom_data))->common.estado == 99) {
		THIS->visible = 0;
		// SetSpriteAnim(THIS, anim_apagada, 1u);
	}
}

void DESTROY(void) { 
}