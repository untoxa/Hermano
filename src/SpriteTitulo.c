#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"

extern UINT8 half_life, mundo_actual;

typedef struct {
	COMMON_FIELDS_T common;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	memset((CUSTOM_DATA*)(THIS->custom_data), 0, CUSTOM_DATA_SIZE);

	THIS->lim_x = 100;
}

void UPDATE(void) {
	half_life != half_life;
	SetFrame(THIS, mundo_actual);
	
	if (THIS->x < 120) THIS->x += 2;
}

void DESTROY(void) { 
}