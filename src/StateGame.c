#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Music.h"
#include <Palette.h>

extern INT8 desplazamiento_y_camara;
extern UINT8 level, has_key;
extern UINT8 mundo_actual, autoscroll;
const UINT16 player_init_x[]    = {16,  16, 	16,  16, 	32,  16, 	632, 0};
const UINT16 player_init_y[]    = {96,  256,  	248, 112,	144, 596,  	64,  0};
const UINT16 y_eje_inframundo[] = {999, 390, 	999, 132, 	999, 999, 	999, 999};
extern UINT16 y_eje_actual;
extern UINT16 x_checkpoint, y_checkpoint;

// IMPORT MAPS FOR LEVELS
IMPORT_MAP(hud);
IMPORT_MAP(cementerio1);
IMPORT_MAP(cementerio2);
IMPORT_MAP(desierto);
IMPORT_MAP(pueblo2);
IMPORT_MAP(selvamap);
IMPORT_MAP(piramidemapa);
// IMPORT_MAP(piramide_dentro);


// SONGS DECLARATIONS
DECLARE_MUSIC(title);
DECLARE_MUSIC(cemetery);
DECLARE_MUSIC(deadboy);
DECLARE_MUSIC(selva);
// DECLARE_MUSIC(piramide_dentro);
 
struct MapInfoBanked {
	UINT8 bank;
	struct MapInfo* map;
};
#define BANKED_MAP(MAP) {BANK(MAP), &MAP}

// Arrays para definir los mundos y subniveles
const struct MapInfoBanked mundo0[] = {
	BANKED_MAP(cementerio1),
	BANKED_MAP(cementerio2)
};

const struct MapInfoBanked mundo1[] = {
	BANKED_MAP(pueblo2),
	BANKED_MAP(desierto)
};

const struct MapInfoBanked mundo2[] = {
	BANKED_MAP(selvamap),
	BANKED_MAP(piramidemapa)
};

const struct MapInfoBanked mundo3[] = {
	// BANKED_MAP(piramide_dentro),
	BANKED_MAP(cementerio1)
};

const struct MapInfoBanked* mundos[] = {mundo0, mundo1, mundo2, mundo3};



void START(void) {
	desplazamiento_y_camara = 0;
	y_eje_actual = y_eje_inframundo[level + (mundo_actual<<1)]; //Guardamos valor del eje de cambio de gravedad
	
#ifdef NINTENDO
	//Change Sprite Palettes
	OBP0_REG = PAL_DEF(1,0,2,3); //Cambia la paleta 0
	OBP1_REG = PAL_DEF(2,0,1,3); //Cambia la paleta 1
#endif	

	const struct MapInfoBanked* niveles = mundos[mundo_actual];
	const struct MapInfoBanked* fase = &niveles[level];
	
	if (x_checkpoint == 0) {
		x_checkpoint = player_init_x[level + (mundo_actual<<1)];
		y_checkpoint = player_init_y[level + (mundo_actual<<1)];
	}
	
	
	autoscroll = 0;
	
	switch (mundo_actual){
		
		case 0:  
			PlayMusic(cemetery, 1);
			break;
		
		case 1: 
			PlayMusic(deadboy, 1);
			break;
		
		case 2: 
			PlayMusic(selva, 1);
			if (level == 0) autoscroll = 1;
			break;	
/*		
		case 3: 
			PlayMusic(piramide_dentro, 1);
			if (level == 0) autoscroll = 1;
			break;
*/
	}
	
	// Initialize tiles
	ScrollInitTilesFromMap(0, fase->bank, fase->map);
	// Initialize HUD
	INIT_HUD(hud);
	// Set scroll target
	if (autoscroll) {
		scroll_target = SpriteManagerAdd(SpriteAutoScroll, 60, 136);
		SpriteManagerAdd(SpritePlayer, 32, 144);
		has_key = 0;
	} else {
		scroll_target = SpriteManagerAdd(SpritePlayer, x_checkpoint, y_checkpoint);
	}
	// Init Map
	ScrollSetMap(fase->bank, fase->map);
	// Init Collisions
	ScrollInitCollisions(NULL, NULL);
	// Redraw screen
	ScrollScreenRedraw();

}

void UPDATE(void) {
	
}
