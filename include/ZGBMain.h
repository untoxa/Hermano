#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#include <gbdk/platform.h>

typedef struct {
	UINT8 posee, estado, contador_tiempo, tocado, life;
	INT16 vy;
} COMMON_FIELDS_T;

#define STATES \
_STATE(StateMenu)\
_STATE(StateTituloNivel)\
_STATE(StateGameOver)\
_STATE(StateGame)\
_STATE(StateIntro)\
_STATE(StateCredits)\
STATE_DEF_END

/*
1 - Parca
2 - Murcielago
3 - Mano
4 - Zombi
5 - Llave
6 - Calabaza
7 - Cojon
8 - Flecha IN
9 - Pajaro
10 - Topo

*/


#define SPRITES \
_SPRITE(SpriteBombaItem,         bomba_item,   FLIP_NONE)\
_SPRITE(SpriteRompible,          rompibles,    FLIP_NONE)\
_SPRITE(SpriteEnemigo1,          parca,        FLIP_X)\
_SPRITE(SpriteEnemigo2,          murcielago,   FLIP_X)\
_SPRITE(SpriteEnemigo3,          mano,         FLIP_NONE)\
_SPRITE(SpriteZombie,            zombie,       FLIP_X)\
_SPRITE(SpriteLlave,             llave,        FLIP_NONE)\
_SPRITE(SpriteCalabaza,          calabaza,     FLIP_X)\
_SPRITE(SpriteCojon,             bola,         FLIP_X)\
_SPRITE(SpriteFlechaIn,          flecha_in,    FLIP_NONE)\
_SPRITE(SpritePajaro,            pajarraco,    FLIP_X)\
_SPRITE(SpriteTopo,              topo,         FLIP_X)\
_SPRITE(SpritePlataforma,        platform,     FLIP_NONE)\
_SPRITE(Sprite1up,               item_1up,     FLIP_NONE)\
_SPRITE(SpriteAntorcha,          antorcha,     FLIP_NONE)\
_SPRITE(SpriteCorazonItem,       corazon_item, FLIP_NONE)\
_SPRITE(SpritePlayer,            calavera,     FLIP_NONE)\
_SPRITE(SpriteCraneo,            craneo,       FLIP_NONE)\
_SPRITE(SpriteBomba,             bomba,        FLIP_NONE)\
_SPRITE(SpriteEnemyProyectile,   enemy_bullet, FLIP_NONE)\
_SPRITE(SpritePress,             press,        FLIP_NONE)\
_SPRITE(SpriteOjillos,           ojos,         FLIP_NONE)\
_SPRITE(SpriteSoul,              soul,         FLIP_NONE)\
_SPRITE(SpriteTitulo,            titulos,      FLIP_NONE)\
_SPRITE(SpriteAutoScroll,        enemy_bullet, FLIP_NONE)\
_SPRITE(SpriteAutoScrollCredits, enemy_bullet, FLIP_NONE)\
SPRITE_DEF_END

#include "ZGBMain_Init.h"

#define MAX_TILE_SOLIDO	27
#define MAX_TILE_TRASPASABLE 35

#endif