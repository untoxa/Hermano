#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Sound.h"
#include "Scroll.h"


//Animations
const UINT8 anim_cojon_quietor[]  = VECTOR(2);
const UINT8 anim_cojon_saltando[] = VECTOR(3);
const UINT8 anim_cojon_salta[]    = VECTOR(1, 0);
extern unsigned char half_life;
extern signed int x_player;

extern UINT8 delta_time;

typedef struct {
	COMMON_FIELDS_T common;
	INT8   vx;
	INT8   gravedad;
	UINT16 cojon_y;
	UINT16 init_y;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void check_direction(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	if (THIS->x > x_player){
		data->vx = -1; 
		THIS->mirror = V_MIRROR;
	}else{
		data->vx = 1;
		THIS->mirror = NO_MIRROR;
	}
	
}

void START(void) { 
	memset((CUSTOM_DATA*)(THIS->custom_data), 0, CUSTOM_DATA_SIZE);

	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->common.estado = 3;
	
	check_direction();
	
	THIS->y -= 4;
	
	data->common.vy = 0;
	data->gravedad = 8;
	data->cojon_y = THIS->y << 6;
	data->init_y = data->cojon_y;
	data->common.contador_tiempo = 15;
	data->common.life = 2;
	
	THIS->lim_x = 64;
	THIS->lim_y = 0;
	
	
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	data->common.contador_tiempo --;
	
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
	
	
	switch(data->common.estado) {
		
		case 1:
		
			SetSpriteAnim(THIS, anim_cojon_quietor, 30u);
			
			if (data->common.contador_tiempo == 0){
				data->common.contador_tiempo = 10;
				data->common.estado = 2;
				
			}
			
		break;
		
		case 2:
			SetSpriteAnim(THIS, anim_cojon_salta, 15u);
			if (data->common.contador_tiempo == 0){
				data->common.estado = 3;
				data->common.vy = -150;
			}
		
		break;
		
	    case 3:
			SetSpriteAnim(THIS, anim_cojon_saltando, 30u);
			data->common.vy += data->gravedad;
			data->cojon_y += data->common.vy;

			if (half_life == 1) THIS->x += (INT16)data->vx << delta_time;
			
			//Colision con escenario
			THIS->y = data->cojon_y >> 6;
			if(data->common.vy > 0) {
				if(GetScrollTile((THIS->x + 8)>> 3, (THIS->y + 20) >> 3) < MAX_TILE_TRASPASABLE) {
					data->common.estado = 1; //estado parado
					data->common.contador_tiempo = 30;
					// PlayFx(CHANNEL_4, 10, 0x38, 0xc2, 0x50, 0x80);
					THIS->y &= 0xFFFC;
					data->cojon_y = THIS->y << 6;
					check_direction();
				}
			}
		break;
		 
		default:
			
			if (data->common.estado == 99){
				SpriteManagerAdd(SpriteSoul, THIS->x + 4, THIS->y + 4);
				data->common.estado ++;
				
			}
			THIS->visible = 0; //Hacerlo invisible

		break;
	}
	
	
}

void DESTROY(void) { 	
}