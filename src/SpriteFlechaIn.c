#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"

extern unsigned char has_key;

typedef struct {
	COMMON_FIELDS_T common;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	memset((CUSTOM_DATA*)(THIS->custom_data), 0, CUSTOM_DATA_SIZE);

	if (has_key == 0) {
		SpriteManagerRemove(THIS_IDX);
		return;
	}
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;	
	data->common.contador_tiempo++;
	
	if (data->common.contador_tiempo > 20){
		THIS->visible = 0;
		if (data->common.contador_tiempo > 40) data->common.contador_tiempo = 0;
	}else THIS->visible = 1;
	
}

void DESTROY(void) { 
}