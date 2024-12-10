#include "ZGBMain.h"
#include "Math.h"

//Custom variables TOTALES


UINT16 tiempo = 0;
UINT8 level, has_key, vidas;
INT8 energy;
INT8 desplazamiento_y_camara;
UINT8 bombas, frame_counter;
UINT8 mundo_actual, autoscroll, vidaextra;
UINT8 bombas_disparadas, half_life, disparando;
UINT16 x_player, x_checkpoint, y_checkpoint;
INT16 y_player;
UINT8 bocabajo = 0;

// UINT16 *inframundo_pointer;
UINT16 y_eje_actual;

UINT8 next_state = StateIntro;

UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {	
	if(current_state == StateGame) {
		if(U_LESS_THAN(255 - (UINT16)*tile_ptr, N_SPRITE_TYPES)) {
			*tile = *(tile_ptr+1);
			return 255 - (UINT16)*tile_ptr;
		}

		*tile = *tile_ptr;
	}
	return 255u;	
}

