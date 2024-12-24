#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "Vector.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "Scroll.h"
#include "Math.h"

//Animations
const UINT8 anim_enem2_walk[] = VECTOR(0, 1, 2, 1);

extern UINT8 half_life;
extern UINT16 y_eje_actual;
extern UINT8 delta_time;

typedef struct {
	COMMON_FIELDS_T common;
	INT8   vx;
	INT16  y_abs;
	INT8   aceleracion_v;
	INT16  init_y;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	memset((CUSTOM_DATA*)(THIS->custom_data), 0, CUSTOM_DATA_SIZE);

	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->common.estado = 1; //Necesario para las colisiones
	// THIS->lim_x = 64;
	// THIS->lim_y = 64;
	
	if (THIS->x > scroll_x) data->vx = -1; else data->vx = 1;
	data->aceleracion_v = 8;
	data->init_y = THIS->y;

	data->common.life = 0;	
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	if(data->common.estado == 1) {
		
		//Tocado y muere
		if (data->common.tocado > 0) {
			data->common.estado = 99;
		}
		
		//Horizontal movement
		if (data->vx < 0){
			THIS->mirror = 2;
		}else{
			THIS->mirror = 0;
		}		
		
		if ((INT16)THIS->y > (INT16)y_eje_actual) THIS->mirror += 1;
		
		//Vertical movement
		data->y_abs = THIS->y << 6;

		if (THIS->y < (data->init_y - 24)) data->aceleracion_v = 8;
		if (THIS->y > (data->init_y + 24)) data->aceleracion_v = -8;
		
		data->common.vy += data->aceleracion_v;
		
		if (data->common.vy > 128) data->common.vy = 128;
		if (data->common.vy < -128) data->common.vy = -128;
		
		if (half_life == 0) THIS->x += data->vx << delta_time;
		
		data->y_abs += data->common.vy << delta_time;

		THIS->y = data->y_abs >> 6;
		
		SetSpriteAnim(THIS, anim_enem2_walk, 12u);
		
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