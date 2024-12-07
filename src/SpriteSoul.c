#include "Banks/SetAutoBank.h"
#include "ZGBMain.h"

#include "SpriteManager.h"
#include "Sprite.h"
// #include "Scroll.h"

//Animations
const UINT8 anim_soul[] = {2, 0, 1};
extern UINT8 delta_time;

typedef struct {
	COMMON_FIELDS_T common;
} CUSTOM_DATA;
CHECK_CUSTOM_DATA_SIZE(CUSTOM_DATA);

void START(void) { 
	THIS->lim_x = 0;
	THIS->lim_y = 0;

}

void UPDATE(void) {
	SetSpriteAnim(THIS, anim_soul, 20u);
	THIS->y -=2 << delta_time;
		
}

void DESTROY(void) { 
}