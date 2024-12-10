#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Scroll.h"

extern UINT16 y_eje_actual;

typedef struct {
	COMMON_FIELDS_T common;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	memset((CUSTOM_DATA*)(THIS->custom_data), 0, CUSTOM_DATA_SIZE);

	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->common.estado = 1; //Necesario para las colisiones
	if ((INT16)THIS->y > (INT16)y_eje_actual) { 
		data->common.vy = -1;
		data->common.tocado = 0;		//Use "tocado" as Y offset to detect collision with ground
		THIS->mirror = H_MIRROR;
	}else{
		data->common.vy = 1;
		data->common.tocado = 16;
		THIS->mirror = NO_MIRROR;
	}
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->common.life = GetScrollTile((THIS->x+8)>>3, (THIS->y+data->common.tocado)>>3); //Use life as auxiliar variable to save the tile numer
	if(data->common.life >= MAX_TILE_TRASPASABLE) THIS->y += data->common.vy;
}

void DESTROY(void) { 
}