#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Sound.h"
// #include "Scroll.h"

extern UINT16 y_eje_actual;
extern UINT8 vidaextra, vidas;

typedef struct {
	COMMON_FIELDS_T common;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	memset((CUSTOM_DATA*)(THIS->custom_data), 0, CUSTOM_DATA_SIZE);

	((CUSTOM_DATA*)(THIS->custom_data))->common.estado = 1; //Necesario para las colisiones
	// if ((INT16)THIS->y > (INT16)y_eje_actual) { 
		// THIS->mirror = H_MIRROR;
	// }else{
		// THIS->mirror = NO_MIRROR;
	// }
	
	if (vidaextra){
		SpriteManagerRemove(THIS_IDX);
		return;
	}
}

void UPDATE(void) {
	UINT8 i;
	Sprite* spr;
	SPRITEMANAGER_ITERATE(i, spr) {		
		if (spr->type == SpritePlayer){
			if(CheckCollision(THIS, spr)) {
				vidas++; vidaextra = 1;
				PlayFx(CHANNEL_1, 8, 0x27, 0x0b, 0xdf, 0xa4, 0x86);
				SpriteManagerRemove(THIS_IDX);
				return;
			}
		}
	}

}

void DESTROY(void) { 
}