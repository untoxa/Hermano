#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
#include "Scroll.h"

//Animations
const UINT8 anim_flying[] = VECTOR(0, 1, 2, 1);

extern UINT8 delta_time;
extern unsigned int x_player;
extern signed int y_player;
void CreateEnemyProyectile(UINT16 x, UINT16 y, INT8 vx_, INT16 vy_, INT8 gravity_) BANKED;

extern unsigned char half_life;
typedef struct {
	COMMON_FIELDS_T common;
	INT8   vx;
	UINT16 x_scroll_base;
	UINT8  disparado;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	memset((CUSTOM_DATA*)(THIS->custom_data), 0, CUSTOM_DATA_SIZE);

	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;

	THIS->attr_add |= S_PALETTE;

	data->common.estado = 1; //Necesario para las colisiones
	THIS->lim_x = 16;
	THIS->lim_y = 0;
	
	data->x_scroll_base = scroll_x;
	if (THIS->x > scroll_x){
		data->vx = -1;
		THIS->mirror = V_MIRROR;
	}else{
		data->vx = 1;
		THIS->mirror = NO_MIRROR;
	}
	
	data->disparado = 0;
	
	THIS->y -= 4;
	data->common.life = 0;	
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	switch (data->common.estado) {
		
		case 1:
		
			//Tocado y muere
			if (data->common.tocado > 0) {
				data->common.estado = 99;
			}
			
			if (data->disparado == 0){
				if( THIS->x > (x_player - 10)){
					if( THIS->x < (x_player + 10))
					{
						CreateEnemyProyectile(THIS->x+8, THIS->y + 16, 0, 0, 10);
						data->disparado = 1;
					}
				}
			}
	
			THIS->x += data->vx << delta_time;

			SetSpriteAnim(THIS, anim_flying, 20u);
		
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