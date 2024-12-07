#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"

extern unsigned char has_key;

/*
has_key = 0 -- NO TIENE LA LLAVE
has_key = 1 -- SI TIENE LA LLAVE
has_key = 2 -- SI TIENE LA LLAVE Y HA GUARDADO CHECKPOINT
*/

typedef struct {
	COMMON_FIELDS_T common;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	if (has_key > 0) {
		SpriteManagerRemove(THIS_IDX);
		return;
	}
		
	data->common.estado = 1; //Necesario para las colisiones
	THIS->x += 4;
}

void UPDATE(void) {	
}

void DESTROY(void) { 
}