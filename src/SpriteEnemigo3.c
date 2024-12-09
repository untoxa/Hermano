#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
// #include "Scroll.h"

// MANOS EN SUELO

//Animations
const UINT8 anim_enem3[] = VECTOR(0, 1, 2, 1);

typedef struct {
	COMMON_FIELDS_T common;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	memset((CUSTOM_DATA*)(THIS->custom_data), 0, CUSTOM_DATA_SIZE);

	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	SetSpriteAnim(THIS, anim_enem3, 12u);
	data->common.estado = 1;
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	//Tocado y muere
	if (data->common.tocado > 0) {
		data->common.estado = 99;
	}
	
	if(data->common.estado != 1) {
		SpriteManagerRemove(THIS_IDX);
		return;
	}
}

void DESTROY(void) { 	
}