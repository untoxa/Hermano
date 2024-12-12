#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "Music.h"
#include <Palette.h>

IMPORT_MAP(titulo_cementerio);
IMPORT_MAP(titulo_pueblo);
IMPORT_MAP(titulo_selva);
IMPORT_MAP(entra_piramide);
DECLARE_MUSIC(level_start);

IMPORT_TILES(titlefont);

extern UINT16 tiempo;
UINT8 parpadeo;
extern UINT8 level, mundo_actual, vidas;
void pausa(UINT16 time) BANKED;

UINT8 pos_y_titulo [] = {41, 30,  41,  8}; // Coordenada Y del sprite para el título del nivel

#ifdef MASTERSYSTEM
#define OFFSET_X 6
#define OFFSET_Y 3
#else
#define OFFSET_X 0
#define OFFSET_Y 0
#endif

UINT8 font_tiles; 
	
void print_texts(void) {
	if (parpadeo == 0) {
		set_bkg_tile_xy(OFFSET_X + 6,  OFFSET_Y + 17, (mundo_actual + font_tiles + 1));
		set_bkg_tile_xy(OFFSET_X + 7,  OFFSET_Y + 17, (font_tiles + 10));
		set_bkg_tile_xy(OFFSET_X + 8,  OFFSET_Y + 17, (level + font_tiles + 1));
		set_bkg_tile_xy(OFFSET_X + 19, OFFSET_Y + 17, (vidas + font_tiles));
	}
	
	if (parpadeo == 15) {
		set_bkg_tile_xy(OFFSET_X + 6, OFFSET_Y + 17, (font_tiles));
		set_bkg_tile_xy(OFFSET_X + 7, OFFSET_Y + 17, (font_tiles));
         	set_bkg_tile_xy(OFFSET_X + 8, OFFSET_Y + 17, (font_tiles));
	}
	parpadeo ++;
	
	if (parpadeo == 31) parpadeo = 0;

}

void START(void) {
#ifdef NINTENDO
	//Change Sprite Palettes
	OBP0_REG = PAL_DEF(0,0,3,2); //Cambia la paleta 0
#endif	

	switch (mundo_actual){
		case 0:  
		InitScroll(BANK(titulo_cementerio), &titulo_cementerio, 0, 0);
		break;
		
		case 1: 
		InitScroll(BANK(titulo_pueblo), &titulo_pueblo, 0, 0);
		break;
		
		case 2: 
		InitScroll(BANK(titulo_selva), &titulo_selva, 0, 0);
		break;
	
		case 3: 
		InitScroll(BANK(entra_piramide), &entra_piramide, 0, 0);
		break;	
	}


	PlayMusic(level_start, 0);
	

	SpriteManagerAdd(SpriteTitulo, 0, (OFFSET_Y << 3) + pos_y_titulo[mundo_actual]);

	tiempo = 240;
	parpadeo = 0;
	
	font_tiles = ScrollSetTiles(last_tile_loaded, BANK(titlefont), &titlefont);	
}

void UPDATE(void) {	
	if (mundo_actual < 3) print_texts();
	
	
	tiempo --;
		
	if (tiempo == 0) {
		if (mundo_actual < 3)
			SetState(StateGame);
		else{ 
			StopMusic;
			pausa(230);
			SetState(StateCredits);
		}
	}
}