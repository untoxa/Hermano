#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Scroll.h"


//Animations
const UINT8 anim_enem_walk[] = VECTOR(0, 1, 2, 1);

extern unsigned char half_life;
extern unsigned int y_eje_actual;
extern UINT8 delta_time;

typedef struct {
	COMMON_FIELDS_T common;
	UINT8  facedown;
	INT8   vx;
	INT8   offset_x;
	INT16  offset_y;
	UINT8  last_mirror;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	memset((CUSTOM_DATA*)(THIS->custom_data), 0, CUSTOM_DATA_SIZE);

	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->common.estado = 1; //Necesario para las colisiones
	THIS->lim_x = 64;
	THIS->lim_y = 64;
	
	if (THIS->x > scroll_x) data->vx = -1; else data->vx = 1;
	
	if ((INT16)THIS->y > (INT16)y_eje_actual) { 
		data->facedown = 1;
		data->offset_y = THIS->y - 5;
	}else{
		data->facedown = 0;
		data->offset_y = THIS->y + 25;
	}
	
	data->last_mirror = 0;
	THIS->y -= 2;
	
	data->common.life = 2;
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	if(data->common.estado == 1) {
		
		if (data->common.tocado > 0) {
			if (data->common.tocado == 10){
			
				data->common.life --;
				if (data->common.life > 200){
					data->common.estado = 99;
				}
			}
			data->common.tocado --;
			THIS->visible = half_life;
			if (data->common.tocado == 0) THIS->visible = 1u;
		}
		
		if (data->vx < 0){
			data->last_mirror = V_MIRROR;
			data->offset_x = -4;
		}
		if (data->vx > 0){
			data->last_mirror = NO_MIRROR;
			data->offset_x = 20;
		}
		
		THIS->mirror = data->last_mirror + data->facedown;
		
		if (half_life == 0) {
			if( GetScrollTile((THIS->x + data->offset_x)>> 3, (data->offset_y) >> 3) >= MAX_TILE_TRASPASABLE ||
				GetScrollTile((THIS->x + data->offset_x)>> 3, (THIS->y + 12) >> 3) < MAX_TILE_SOLIDO || THIS->x < 8 )
			{
				data->vx = -data->vx;
			}
			THIS->x += data->vx << delta_time;
		}
		
		SetSpriteAnim(THIS, anim_enem_walk, 12u);
		
	} else {

		if (data->common.estado == 99){
			SpriteManagerAdd(SpriteSoul, THIS->x + 4, THIS->y + 4);
			data->common.estado ++;
			
		}
		THIS->visible = 0; //Hacerlo invisible
	}
}

void DESTROY(void) { 
}