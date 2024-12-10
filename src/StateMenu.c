#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "Music.h"
#include <Palette.h>

IMPORT_MAP(menu);
IMPORT_MAP(tutorial_screen);
DECLARE_MUSIC(title);

UINT8 tutorial = 0;

extern UINT8 level, mundo_actual, vidas, has_key;
extern UINT16 x_checkpoint, y_checkpoint;

void START(void) {	
	//Init SFX
	INIT_SOUND();
	
#ifdef NINTENDO
	//Change Sprite Palettes
	OBP0_REG = PAL_DEF(0,1,3,2); //Cambia la paleta 0
	OBP1_REG = PAL_DEF(1,0,3,2); //Cambia la paleta 1
#endif
	
	mundo_actual = 0;
	level = 0;
	
	has_key = 0;
	vidas = 3;
	
	x_checkpoint = 0;
	y_checkpoint = 0;

	InitScroll(BANK(menu), &menu, 0, 0);

	PlayMusic(title, 0);
	SpriteManagerAdd(SpritePress, 40, 94);
	SpriteManagerAdd(SpriteOjillos, 131, 38);
}

void UPDATE(void) {	
	if(KEY_TICKED(J_START)) {
		
		if (tutorial == 0){
			tutorial = 1;
			DISPLAY_OFF;
			StopMusic;
			SpriteManagerReset();
			InitScroll(BANK(tutorial_screen), &tutorial_screen, 0, 0);
			DISPLAY_ON;
		}else SetState(StateTituloNivel);
	}
	
	if(KEY_TICKED(J_SELECT)) {
		SetState(StateCredits);
	}
	
}