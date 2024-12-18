#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "Music.h"
#include <Palette.h>

IMPORT_MAP(credits);
DECLARE_MUSIC(creditos);

void START(void) {
#ifdef NINTENDO
	BGP_REG = PAL_DEF(0, 1, 3, 2);
#endif	
	InitScroll(BANK(credits), &credits, 0, 0);
	PlayMusic(creditos, 0);
	scroll_target = SpriteManagerAdd(SpriteAutoScrollCredits, 0, 0);
}

void UPDATE(void) {
	if(KEY_TICKED(J_START)) {
		SetState(StateMenu);
	}

}