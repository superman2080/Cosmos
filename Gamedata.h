#pragma once
#include <d3d9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include <d3dx9.h>
#include <time.h>
#include "Collision.h"



#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 900

#define ADD_BULLET_NUM 5
#define BOSS_ADD_BULLET_NUM 21
#define MAX_BULLET_NUM 200

#define MAX_ENEMY_NUM 500

VOID GameInit();
VOID GameRelease();
VOID GameUpdate();
VOID GameRender();

enum GAME_STATE
{
	LOADING,
	READY,
	STAGE1,
	STAGE2
};

enum BOSS_PAT
{
	LEFT_TO_RIGHT,
	RIGHT_TO_LEFT,
	CIRCLE,
	TRACE,
	ADD,
	NONE
};

enum ENEMY_TYPE
{
	TYPE1,
	TYPE2,
	TYPE3
};

struct IMAGE
{
	BOOL bVisible;
	RECT rect;
	D3DXCOLOR color;
	D3DXVECTOR3 center;
	D3DXVECTOR3 position;
	LPDIRECT3DTEXTURE9 texture;
};

struct PLAYER_PROPERTY
{
	INT lv;
	FLOAT EXP;
	FLOAT maxEXP;

	FLOAT HP;
	FLOAT maxHP;

	FLOAT v;
	FLOAT damage;
	FLOAT originDamage;

	INT addBulletNum;
	FLOAT fireSpeed;
	FLOAT originSpeed;
	FLOAT fireTimer;

	BOOL isDirectional;
	BOOL isInvincible;
	BOOL isShowLv;

	FLOAT shieldCoolDown;
	FLOAT shieldCooltime;

	FLOAT damageTime;
};

struct PLAYER
{
	IMAGE image;
	PLAYER_PROPERTY prop;
};

struct PLAYER_BULLET
{
	IMAGE image;
	FLOAT v;
};

struct ENEMY_PROPERTY
{
	FLOAT HP;
	FLOAT v;
	FLOAT enemyXmovement;

	FLOAT appearTime;

	FLOAT fireSpeed;
	FLOAT fireTimer;

	DWORD enemyOldTime;

	FLOAT damageTime;

	ENEMY_TYPE enemyType;
};

struct ENEMY_BULLET
{
	D3DXVECTOR3 aimPos;
	FLOAT stepX, stepY;
	FLOAT dX, dY;
	FLOAT dis;

	FLOAT v;
	IMAGE image;
};

struct ENEMY
{
	ENEMY_PROPERTY prop;
	IMAGE image;
};

struct BOSS_PROPERTY
{
	FLOAT appearTime;
	FLOAT oldTime;

	FLOAT HP;
	FLOAT maxHP;
	FLOAT v;

	BOSS_PAT pattern;

	BOOL isPatPlaying;
	FLOAT fireTimer;
	FLOAT fireSpeed;

	FLOAT nextPatDelay;

	FLOAT damageTime;

};

struct BOSS_BULLET
{
	D3DXVECTOR3 aimPos;
	FLOAT stepX, stepY;
	FLOAT dX, dY;
	FLOAT dis;

	BOSS_PAT pattern;

	FLOAT v;
	IMAGE image;
};

struct BOSS
{
	BOOL deadEvent;
	IMAGE image;
	BOSS_PROPERTY prop;
};

