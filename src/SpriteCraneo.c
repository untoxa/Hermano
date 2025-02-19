#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "Vector.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "Sound.h"
#include "Scroll.h"
#include "Math.h"

extern UINT8 disparando, half_life, bombas, energy, frame_counter;
extern INT16 y_player;
UINT8 offset_y, craneo_usado;


//Animations
const UINT8 anim_rotando[] = VECTOR(0, 1, 2, 3);
UINT8 probabilidad;
typedef struct {
	COMMON_FIELDS_T common;
	INT16 vx;
	INT16 init_x;
	INT16 limit_x;
	INT16 init_vx;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void CreateCraneo(UINT16 x, INT16 y, INT16 p_facing_, UINT8 offset_y_ ) BANKED {
	Sprite* spr = SpriteManagerAdd(SpriteCraneo, x, y);
	CUSTOM_DATA* data = (CUSTOM_DATA*)spr->custom_data;
	offset_y = offset_y_;
	data->init_x = x;
	data->limit_x = x + (p_facing_*51);
	data->vx = data->init_vx = p_facing_*3;
	PlayFx(CHANNEL_1, 5, 0x3d, 0x4a, 0x6d, 0x99, 0xc6);
}

void START(void) { 
	memset((CUSTOM_DATA*)(THIS->custom_data), 0, CUSTOM_DATA_SIZE);

	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	data->common.estado = 1; //Necesario para las colisiones
	THIS->lim_x = 100;
	THIS->lim_y = 100;
	craneo_usado = 0;
}

void UPDATE(void) {
	UINT8 i;
	Sprite* spr;
	
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;

	if(data->common.estado == 1) {
		SetSpriteAnim(THIS, anim_rotando, 30u);

		THIS->x += data->vx;
		if (THIS->x == data->limit_x) data->vx = -data->vx;

		if (half_life == 1){ // Solo frames impares

			SPRITEMANAGER_ITERATE(i, spr) {

				COMMON_FIELDS_T* idata = (COMMON_FIELDS_T*)spr->custom_data;
				if(CheckCollision(THIS, spr)) {
					switch(spr->type) {
						
						case SpritePlayer:
							if (DISTANCE(THIS->x, spr->x) < 10) {
								if (data->vx != data->init_vx) {
									data->common.estado = 2; 
									disparando = 0;
									if (idata->estado == 1) idata->estado = 0; // Si el player está disparando vuelve al estado 0 (si está tocado, no).
								}
							}
						break;

						case SpriteEnemigo1:
						case SpriteEnemigo2:
						case SpriteZombie:
						case SpriteCalabaza:
						case SpriteCojon:
						case SpritePajaro:
						case SpriteTopo:
						case SpriteAntorcha:
							if (craneo_usado == 0){
								if (idata->estado < 99){
									craneo_usado = 1;
									idata->life --;
									if (idata->life > 200){
										idata->estado = 99;
										
										// SALEN ITEMS
										probabilidad = 160 - (bombas<<5);
										
										if (frame_counter < probabilidad) {
											SpriteManagerAdd(SpriteBombaItem, spr->x, spr->y);
										}else{
											
											probabilidad = 210 + (energy<<3);
										
											if (frame_counter > probabilidad) {
												SpriteManagerAdd(SpriteCorazonItem, spr->x, spr->y);
											}
											
										}
										// END SALEN ITEMS
									}
									idata->tocado = 10;
									if (data->vx == data->init_vx){ //Solo colisiona en la ida
										data->vx = -data->vx; //Cambio direccion del craneo
									}
									PlayFx(CHANNEL_1, 8, 0x2f, 0x08, 0xf3, 0x62, 0x84);
								}
							}
						break;
						
					}
				}
				
			
			}		
		}
		
	} else {
		SpriteManagerRemove(THIS_IDX);
		return;
	}
	
	THIS->y = y_player + offset_y;
}

void DESTROY(void) { 	
}