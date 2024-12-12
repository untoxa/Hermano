#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "Keys.h"
#include "SpriteManager.h"

#include "Sound.h"
#include "Music.h"
#include "Sprite.h"
#include "Scroll.h"

DECLARE_MUSIC(muerte);
DECLARE_MUSIC(stageclear);

typedef struct {
	COMMON_FIELDS_T common;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void CreateBomb(UINT16 x, UINT16 y, INT8 vx_, INT16 vy_, INT8 gravity_) BANKED;
void CreateCraneo(UINT16 x, INT16 y, INT16 p_facing_, UINT8 offset_y_) BANKED;

#define PLAYER_ENERGY_MAX   5
#define MAX_BOMBS           5

#define X_SPEED_MAX         88
#define X_SPEED_INC         12
#define X_FRICTION          4
#define GRAVITY             10
#define Y_SPEED_MAX         220
#define JUMP_INC            220

//Player variables
INT16 aux1,aux2;
INT16 cx1,cx2,cy1,cy2;
UINT8 pt1, pt2;
UINT32 p_x;
UINT32 p_y;
INT16 p_vx, p_vy, platform_vy;
INT8 gravity;
INT16 jump_inc;
INT8 p_facing;
UINT8 last_mirror_player, pausado;


//Animations
const UINT8 anim_player_walk[]       = VECTOR(0, 1, 2, 1, 0);
const UINT8 anim_player_jump[]       = VECTOR(3);
const UINT8 anim_player_quietor[]    = VECTOR(1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1);
const UINT8 anim_player_tocado[]     = VECTOR(4, 5);
const UINT8 anim_player_muerto[]     = VECTOR(4);
const UINT8 anim_player_muerto2[]    = VECTOR(7);
const UINT8 anim_player_tocado2[]    = VECTOR(7, 5); // THIS->tocado cuando ha disparado la cabeza
const UINT8 anim_player_disparando[] = VECTOR(6);

// DATOS NIVELES
extern UINT8 level, mundo_actual, vidas, has_key, bombas_disparadas, vidaextra;
extern UINT8 half_life, frame_counter, disparando, bombas, autoscroll;
extern INT8 energy;
extern UINT16 x_player;
extern INT16 y_player;
extern UINT8 bocabajo;
extern UINT16 y_eje_actual;
extern UINT16 x_checkpoint, y_checkpoint;

//Funciones extra

void UpdateHudLife(void) {
#ifdef NINTENDO
	for (INT8 i = 0; i < PLAYER_ENERGY_MAX; ++i)
		UPDATE_HUD_TILE(0 + i, 0, (i < energy) ? 3 : 2);
	
	if (has_key > 0) UPDATE_HUD_TILE(10, 0, 1);
#endif
}

void UpdateHudBombs(void) {
#ifdef NINTENDO
	for (UINT8 i = 0; i < MAX_BOMBS; ++i)
		UPDATE_HUD_TILE(19 - i, 0, (i < bombas) ? 4 : 0);
#endif
}

void check_2_points(void) {
	pt1 = GetScrollTile(cx1>>3, cy1>>3);
	pt2 = GetScrollTile(cx2>>3, cy2>>3);
}

void pausa(UINT16 time) BANKED {
	if (time) while (time--) vsync();	
}

void loss_energy(void) {	
//	return; // invincibility

	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;

	PlayFx(CHANNEL_1, 8, 0x3b, 0x48, 0xb2, 0x17, 0x84);
	energy--;
	UpdateHudLife();
	
	if (energy < 1) {
		data->common.tocado = 1;
		vidas --;
		pausa(30);
		PlayMusic(muerte, 0);
		data->common.estado = 4;
		scroll_target = 0; //Quito el scroll al player
		p_vy = jump_inc; //rebote al morir
		data->common.contador_tiempo = 180;
	}else {
	
		data->common.tocado = 70;
		data->common.estado = 3;
		
	}
}

void rebote_tocado(void) {
	//rebote al recibir golpe
	p_vy = jump_inc>>1; 
	p_vx = -p_vx;
}

void animate_player(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	switch (data->common.estado) {
		
		case 0:
		
			if (data->common.posee){
				//Walk / Idle
				if (p_vx != 0)
					SetSpriteAnim(THIS, anim_player_walk, 12u);
				else
					SetSpriteAnim(THIS, anim_player_quietor, 12);
				
			}else{
				//Jump
				SetSpriteAnim(THIS, anim_player_jump, 2);
			}
			
			break;
		
		case 1:
		case 3:
		
			if (data->common.tocado){
				//Parpadeando THIS->tocado
				if (disparando)
					SetSpriteAnim(THIS, anim_player_tocado2, 30u);
				else
					SetSpriteAnim(THIS, anim_player_tocado, 30u);
			
			}else{
				SetSpriteAnim(THIS, anim_player_disparando, 0);
			}	
			break;
			
		case 4: //Muerte
			if (disparando)
					SetSpriteAnim(THIS, anim_player_muerto2, 30u);
				else
			SetSpriteAnim(THIS, anim_player_muerto, 30u);
		break;
	
	}
}

void check_sprites_collisions(void) {
	
	UINT8 i;
	Sprite* spr;
	platform_vy = 0;
	SPRITEMANAGER_ITERATE(i, spr) {
		COMMON_FIELDS_T* idata = (COMMON_FIELDS_T*)spr->custom_data;
		if (idata->estado < 99){
		// if (THIS->estado < 4){
			if(CheckCollision(THIS, spr)) {
				
				switch(spr->type){ 
				
					case SpriteBombaItem:
						PlayFx(CHANNEL_1, 8, 0x15, 0x8a, 0xff, 0x43, 0x85);
						SpriteManagerRemove(i);
						if (bombas < MAX_BOMBS) bombas ++;
						UpdateHudBombs();					
					break;
					
					case SpriteCorazonItem:
						PlayFx(CHANNEL_1, 8, 0x15, 0x8a, 0xff, 0x43, 0x85);
						SpriteManagerRemove(i);
						if (energy < PLAYER_ENERGY_MAX) energy ++;
						UpdateHudLife();					
					break;
					
					case SpriteLlave:
						PlayFx(CHANNEL_1, 8, 0x17, 0x4b, 0xda, 0x08, 0x87);
						SpriteManagerRemove(i);
						has_key = 1;
						UpdateHudLife();
					break;
					
					case SpriteRompible:
					case SpriteAntorcha:
						if (idata->estado < 4){
							rebote_tocado();
						}
					case SpriteEnemyProyectile:
						if (idata->tocado == 0){
							loss_energy();
						}
						idata->estado = 2;
					break;
					
					case SpriteEnemigo1:
					case SpriteEnemigo2:
					case SpriteEnemigo3:
					case SpriteZombie:
					case SpriteCalabaza:
					case SpriteCojon:
					case SpritePajaro:
					case SpriteTopo:
						if (idata->estado < 2){ //estados 0 y 1
							rebote_tocado();
							loss_energy();
						}
					break;
					
					case SpritePlataforma:
			
							if (THIS->y >= spr->y - 25 && THIS->y < spr->y - 16){
								platform_vy = idata->vy;
								THIS->y = spr->y - 21;
								if (platform_vy < 0) THIS->y --;
								p_y = (UINT32)(THIS->y) << 6;
							}
											
					break;
					
					
					case SpriteCraneo:
			
							if (idata->estado == 4){
								idata->tocado = 0;
							}
											
					break;
					
					
				}
				
				
			}
		// }
		}
		
		
	}
	
	if (autoscroll) {
		if (THIS->y > scroll_y+160) {
			energy = 1;
			loss_energy();
		}
	}
	
}

void add_friction(void) {
	
	if (p_vx > 0) 
		p_vx -= X_FRICTION;
	else if (p_vx < 0)
		p_vx += X_FRICTION;
	
}

void add_gravity(void) {
	
	//gravity
	p_vy += gravity;
	
	//vertical speed limits
	if (p_vy > Y_SPEED_MAX) p_vy = Y_SPEED_MAX;
	if (p_vy < -Y_SPEED_MAX) p_vy = -Y_SPEED_MAX;
	
}


void check_gravity_axis(void) {
	
	if ((INT16)THIS->y > (INT16)y_eje_actual) { 
		gravity = -GRAVITY;
		jump_inc = JUMP_INC;
		bocabajo = 1;
	}else{
		gravity = GRAVITY;
		jump_inc = -JUMP_INC;
		bocabajo = 0;
	}	
	
}

#ifdef SEGA
void reload_flipped_data(UINT8 sprite_type, UINT8 flip) NONBANKED {
	UINT8 __save = CURRENT_BANK;
	SWITCH_ROM(spriteDataBanks[sprite_type]);
	set_sprite_data_flip(spriteIdxs[sprite_type], spriteDatas[sprite_type]->num_tiles, spriteDatas[sprite_type]->data, flip);
	SWITCH_ROM(__save);
}
#endif

//Funciones basicas

void START(void) {
	memset((CUSTOM_DATA*)(THIS->custom_data), 0, CUSTOM_DATA_SIZE);

	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	check_gravity_axis();
	
	data->common.posee = 1;
	p_vy = p_vx = 0;
	bombas = 0;
	p_facing = 1;
	last_mirror_player = 0;
	
	THIS->attr_add |= S_PALETTE;
	
	p_x = (UINT32)(THIS->x) << 6;
	p_y = (UINT32)(THIS->y) << 6;
	
	THIS->lim_y = 255;
	
	energy = PLAYER_ENERGY_MAX;
	
	data->common.estado = 0;
	data->common.tocado = 0;
	
	UpdateHudLife();
}

void UPDATE(void) {
	CUSTOM_DATA* data = (CUSTOM_DATA*)THIS->custom_data;
	
	half_life = !half_life;
	frame_counter ++;
	
	if (data->common.estado < 4){ //TODO EL CONTROL Y MOVIMIENTOS SE ANULAN EN ESTADO 4 (MUERTE)

			//VERTICAL MOVEMENT 
			
			add_gravity();
			
			//Player Jump
			if (data->common.posee) {
				if (disparando == 0){
						if(KEY_TICKED(J_A)) {
						p_vy = jump_inc; 
						platform_vy = 0;
						PlayFx(CHANNEL_1, 5, 0x27, 0x82, 0x9f, 0x0b, 0xc6);
					}
				}
				if (has_key == 1){
					has_key ++;
					x_checkpoint = THIS->x;
					y_checkpoint = THIS->y;
				}
			}

			// if (platform_vy) p_vy = platform_vy;
			p_y += p_vy;
			THIS->y = (p_y >> 6);

			data->common.posee = 0;
			
			if (platform_vy != 0){
				data->common.posee = 1;
			}
			
			if (p_vy > 0){
				cx1 = THIS->x+2 ; cy1 = THIS->y + 24;
				cx2 = THIS->x+13 ; cy2 = cy1;
				check_2_points();	
				if (pt1 < MAX_TILE_TRASPASABLE || pt2 < MAX_TILE_TRASPASABLE) {
					if (bocabajo == 0){
						data->common.posee = 1;
						THIS->y &= 0xFFF8;
						p_y = (UINT32)(THIS->y) << 6;
					}
					p_vy = 0;
				}else{
					if(KEY_PRESSED(J_DOWN)){ //Aceleración al caer
						p_vy += 2;
						if(KEY_TICKED(J_DOWN)) PlayFx(CHANNEL_1, 5, 0x27, 0x0b, 0xc2, 0x73, 0x86);
					}
				}
			}
			
			
			if (p_vy < 0){
				cx1 = THIS->x+2; cy1 = THIS->y;
				cx2 = THIS->x+13; cy2 = cy1;
				check_2_points();
				if (pt1 < MAX_TILE_SOLIDO || pt2 < MAX_TILE_SOLIDO) {
					if (bocabajo == 1){
						data->common.posee = 1;
						THIS->y += 4;
						THIS->y &= 0xFFF8;
						p_y = (UINT32)(THIS->y) << 6;
					}
					p_vy = 0;
				}
				
			}

	
			//HORIZONTAL MOVEMENT 
			
			
			//Player Control
			if(disparando == 0){
				if(KEY_PRESSED(J_RIGHT)){
					
					p_facing = 1;
					p_vx += X_SPEED_INC;
					if (p_vx > X_SPEED_MAX) p_vx = X_SPEED_MAX;
					last_mirror_player = NO_MIRROR;
					
				}else if(KEY_PRESSED(J_LEFT)){
					
					p_facing = -1;
					p_vx -= X_SPEED_INC;
					if (p_vx < -X_SPEED_MAX) p_vx = -X_SPEED_MAX;
					last_mirror_player = V_MIRROR;
					
				}else{

					add_friction();

				}
			}else{

				add_friction();

			}
			

#ifdef SEGA
			static UINT8 new_mirror;
			new_mirror = last_mirror_player | ((bocabajo) ? H_MIRROR : NO_MIRROR);
			if (THIS->mirror != new_mirror) {
				THIS->mirror = new_mirror;
				SwapOAMs();
				vsync();
				reload_flipped_data(THIS->type, new_mirror);
			}
#else
			THIS->mirror = last_mirror_player | ((bocabajo) ? H_MIRROR : NO_MIRROR);
#endif			

			//Horizontal collisions
			if (p_vx < 0){
				cx1 = THIS->x-1; cy1 = THIS->y + 2;
				cx2 = cx1; cy2 = THIS->y + 22;
				check_2_points();
				if (pt1 < MAX_TILE_SOLIDO || pt2 < MAX_TILE_SOLIDO) {
					p_vx = 0;
				}
			}
			
			if (p_vx > 0){
				cx1 = THIS->x+17; cy1 = THIS->y + 2;
				cx2 = cx1; cy2 = THIS->y + 22;
				check_2_points();
				if (pt1 < MAX_TILE_SOLIDO || pt2 < MAX_TILE_SOLIDO) {
					p_vx = 0;
				}
			}
			
			

			p_x += p_vx;
			
			
			//Limite izquierda
			if (p_x < (UINT32)(scroll_x + 4)<<6){
				p_x = (UINT32)(scroll_x + 4)<<6;
				p_vx = 0;
				
				// En la fase de auto-scroll comprueba si te estruja un bloque sólido.
				if (autoscroll){
				
					cx1 = THIS->x+10; cy1 = THIS->y;
					cx2 = THIS->x+10; cy2 = THIS->y + 16;
					check_2_points();
					if (pt1 < MAX_TILE_SOLIDO || pt2 < MAX_TILE_SOLIDO) {
					
						energy = 1;
						loss_energy();
						data->common.estado = 4;
					}
				}
			}
			
			//Limite derecha
			if (p_x > (UINT32)(scroll_x + (SCREEN_WIDTH - 20)) << 6) {
				p_x = (UINT32)(scroll_x + (SCREEN_WIDTH - 20)) << 6;
				p_vx = 0;
			}
			
			THIS->x = (UINT16)(p_x >> 6);
			
			
			

			
			
			check_gravity_axis();
			
			if(KEY_PRESSED(J_UP) && KEY_TICKED(J_B)) {
			//Drop bombs
			if (bombas){
				if (bombas_disparadas == 0){
					
						bombas--;
						bombas_disparadas++;
						UpdateHudBombs();
						CreateBomb(THIS->x + 4, THIS->y + (bocabajo<<4), p_facing<<1, (jump_inc>>1), gravity);
					}
				}
			}else
			
			//Lanzar craneo
			if (data->common.estado != 3){
					
					if(KEY_TICKED(J_B) && data->common.estado == 0) {
						// p_vx = 0;
						data->common.estado = 1; //estado Disparando cabezas
						disparando = 1;
						CreateCraneo(THIS->x , THIS->y-2, p_facing, bocabajo<<3);
					}
					
			}


			if (data->common.tocado){
				if (data->common.estado < 4){
					data->common.tocado --;
					if (data->common.tocado == 0) data->common.estado = 0;
					
				}
			}
			
			//Detect EXIT - puerta
			if (has_key) {				
				if(KEY_TICKED(J_UP)) { 
					cx1 = THIS->x ; cy1 = THIS->y ;
					cx2 = THIS->x+8 ; cy2 = THIS->y ;
					check_2_points();
					if (pt1 == MAX_TILE_TRASPASABLE || pt2 == MAX_TILE_TRASPASABLE) { // Exit gate upper left corner 
						has_key = 0;
						level ++;
						x_checkpoint = 0;
						y_checkpoint = 0;
						if (level == 2) {
							level = 0;
							vidaextra = 0;
							mundo_actual ++;
							// IR A FINAL SI PASAMOS TODOS LOS MUNDOS
							
							SetState(StateTituloNivel); 
						} else SetState(StateGame); 
						
						
						PlayMusic(stageclear, 0);
						pausa(240);
						SetWindowY(144);
					}
				
				}
					
			}
			
			
			
			if (half_life == 0 || (autoscroll)) check_sprites_collisions();


	}else{ //IF ESTADO == 4
	
			add_gravity();

			p_y += p_vy;
			THIS->y = (p_y >> 6);
			
			
			data->common.tocado = 0;
	
	}
	
	//ANIMACIONES PLAYER
	animate_player();
	x_player = THIS->x;
	y_player = THIS->y;
		
#ifndef MASTERSYSTEM
	if(KEY_TICKED(J_START)) {
		PlayFx(CHANNEL_1, 8, 0x74, 0x0a, 0xf7, 0x72, 0x86);
		pausa(50);
		waitpad(J_START);
		PlayFx(CHANNEL_1, 8, 0x74, 0x0a, 0xf7, 0x72, 0x86);
	}
#endif
	
}

void DESTROY(void) {		
	SetWindowY(144);
	if (vidas > 0){
		SetState(StateTituloNivel);
	}else{
		SetState(StateGameOver);
	}
	
}


