#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "Vector.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "Sound.h"
#include "Scroll.h"


//Animations
const UINT8 anim_explosion[] = VECTOR(1);
extern UINT8 delta_time;
extern UINT8 bombas_disparadas;

typedef struct {
	COMMON_FIELDS_T common;
	INT8  vx;
	// UINT8 control;
	INT8  gravedad;
	INT32 bomb_y;
	UINT8 cont_explosion;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void CreateBomb(UINT16 x, UINT16 y, INT8 vx_, INT16 vy_, INT8 gravity_) BANKED {
	Sprite* spr = SpriteManagerAdd(SpriteBomba, x, y);
	CUSTOM_DATA* data = (CUSTOM_DATA*)spr->custom_data;
	
	data->vx = vx_;
	data->common.vy = vy_;
	data->gravedad = gravity_;
	data->bomb_y = y << 6;

	PlayFx(CHANNEL_1, 7, 0x7f, 0x0a, 0xf4, 0xa4, 0x86);
	
}

void explotar_bomba(void){
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->common.estado = 1; //estado explosion
	data->cont_explosion = 20;
	PlayFx(CHANNEL_4, 10, 0x38, 0xc2, 0x56, 0x80);
	
}

void START(void) { 
	memset((CUSTOM_DATA*)(THIS->custom_data), 0, CUSTOM_DATA_SIZE);

	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->common.estado = 0;
	THIS->lim_y = 32;
	THIS->lim_x = 8;
	
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	UINT8 i;
	Sprite* spr;
	
	if(data->common.estado == 0) {
		
		SetFrame(THIS, 0);
		
		SPRITEMANAGER_ITERATE(i, spr) {
			COMMON_FIELDS_T* idata = (COMMON_FIELDS_T*)spr->custom_data;
			if (idata->estado < 99){
				if(CheckCollision(THIS, spr)) {
					switch(spr->type) {
						
						case SpriteRompible:
						case SpriteEnemigo1:
						case SpriteEnemigo2:
						case SpriteEnemigo3:
						case SpriteZombie:
						case SpriteCalabaza:
						case SpriteCojon:
						case SpritePajaro:
						case SpriteTopo:	
							if (idata->estado < 99){
									idata->life -= 2;
									if (idata->life > 200){
										idata->estado = 99;
									}
									idata->tocado = 10;
								}
							
							explotar_bomba();
						break;
						
					}
				}
			}
		}
		
		
		data->common.vy += data->gravedad;
		data->bomb_y += (INT16)data->common.vy;

		THIS->x += (INT16)data->vx << delta_time;
		THIS->y = data->bomb_y >> 6;
		
		//Colision con escenario
		// data->control = 0;
		// if (data->gravedad > 0){
			// if (data->common.vy > 0) data->control = 1;
		// }else {
			// if (data->common.vy < 0) data->control = 1;
		// }
			
		// if (data->control){ //IF IS FALLING CAN COLLIDE
			if(GetScrollTile((THIS->x + 8)>> 3, (THIS->y + 8) >> 3) < MAX_TILE_SOLIDO) {
				data->common.estado = 1; //estado explosion
				data->cont_explosion = 10;
				PlayFx(CHANNEL_4, 10, 0x38, 0xc2, 0x50, 0x80);
			}
		// }
		
		
	} else {
		data->cont_explosion --;
		SetSpriteAnim(THIS, anim_explosion, 20u);
		if (data->cont_explosion == 0) {
			
			SpriteManagerRemove(THIS_IDX);
			
			return;
			
		}
	}
}

void DESTROY(void) { 
	bombas_disparadas = 0;
}