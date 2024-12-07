#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Sound.h"
#include "Scroll.h"


extern unsigned char half_life;
extern UINT8 delta_time;

typedef struct {
	COMMON_FIELDS_T common;
	INT8   vx;
	INT8   inc_x;
	INT8   inc_y;
	INT8   gravedad;
	INT16  bomb_y;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void CreateEnemyProyectile(UINT16 x, UINT16 y, INT8 vx_, INT16 vy_, INT8 gravity_) BANKED {
	Sprite* spr = SpriteManagerAdd(SpriteEnemyProyectile, x, y);
	CUSTOM_DATA* data = (CUSTOM_DATA*)spr->custom_data;
	
	data->vx = vx_;
	data->common.vy = vy_;
	data->gravedad = gravity_;
	data->bomb_y = y << 6;
	THIS->lim_y = 32;
	PlayFx(CHANNEL_1, 5, 0x3d, 0x4a, 0x6d, 0x99, 0xc6);
}


void START(void) { 
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->common.estado = 1;
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	if(data->common.estado == 1) {
		
		SetFrame(THIS, 0);
		
		data->common.vy += data->gravedad;
		data->bomb_y += data->common.vy << delta_time;
		
		if (half_life == 0) THIS->x += (INT16)data->vx << delta_time;
		THIS->y = data->bomb_y >> 6;
		
	} else {
		SpriteManagerRemove(THIS_IDX);
		return;
	}
}

void DESTROY(void) { 
}