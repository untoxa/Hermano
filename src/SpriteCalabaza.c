#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "Vector.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "Scroll.h"

//Animations
const UINT8 anim_calabaza_walk[] = VECTOR(0, 1, 2);
const UINT8 anim_calabaza_idle[] = VECTOR(3);
extern UINT8 delta_time;
void CreateEnemyProyectile(UINT16 x, UINT16 y, INT8 vx_, INT16 vy_, INT8 gravity_) BANKED;

extern UINT8 half_life;
typedef struct {
	COMMON_FIELDS_T common;
	INT8  vx;
	INT8  offset_x;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	memset((CUSTOM_DATA*)(THIS->custom_data), 0, CUSTOM_DATA_SIZE);

	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;

	THIS->attr_add |= S_PALETTE;

	data->common.estado = 1; //Necesario para las colisiones
	THIS->lim_x = 64;
	THIS->lim_y = 64;
	
	if (THIS->x > scroll_x) data->vx = -1; else data->vx = 1;
	
	data->common.contador_tiempo = 10;
	
	THIS->y -= 2;
	
	data->common.life = 1;
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	// Tocado y resta vida
	if (data->common.tocado > 0) {
		data->common.tocado --;
		THIS->visible = half_life;
		if (data->common.tocado == 0) THIS->visible = 1u;
	}
	
	
	switch (data->common.estado) {
		
		case 1:
			
			if (data->vx < 0){
				THIS->mirror = V_MIRROR;
				data->offset_x = 0;
			}
			if (data->vx > 0){
				THIS->mirror = NO_MIRROR;
				data->offset_x = 12;
			}
			
			if(GetScrollTile((THIS->x + data->offset_x)>> 3, (THIS->y + 25) >> 3) > MAX_TILE_TRASPASABLE ||
				GetScrollTile((THIS->x + data->offset_x)>> 3, (THIS->y + 15) >> 3) < MAX_TILE_SOLIDO)
			{
				data->vx = -data->vx;
			}

			
			if (half_life == 0) THIS->x += data->vx << delta_time;
			
			SetSpriteAnim(THIS, anim_calabaza_walk, 12u);
			
			
			data->common.contador_tiempo --;
			if (data->common.contador_tiempo == 0) {
				data->common.estado = 2;
				data->common.contador_tiempo = 20;
			}
		break;
		
		case 2:
			SetSpriteAnim(THIS, anim_calabaza_idle, 20u);
			
			data->common.contador_tiempo --;
			if (data->common.contador_tiempo == 0) {
				data->common.estado = 1;
				data->common.contador_tiempo = 50;
				if (THIS->y > scroll_y)	CreateEnemyProyectile(THIS->x + data->offset_x, THIS->y + 4, data->vx<<1, -160, 10);
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