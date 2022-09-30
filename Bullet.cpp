#include "Bullet.h"

extern LPDIRECT3DDEVICE9		g_pd3dDevice; // Our rendering device
extern LPD3DXSPRITE				g_pSprite;
extern PLAYER					player;

extern DWORD					dwElapsedTime;

extern PLAYER_BULLET			bullet[ADD_BULLET_NUM][MAX_BULLET_NUM];

extern ENEMY					enemy[MAX_ENEMY_NUM];
extern INT						nEnemyIdx;
extern INT						g_Score;
extern BOSS						stage1Sub;
extern BOSS						stage1Boss;

VOID BulletInit()
{
	bullet[0][0].image.bVisible = FALSE;
	bullet[0][0].image.position = player.image.position;
	bullet[0][0].image.rect = { 58, 99, 58 +10, 99 + 10 };
	bullet[0][0].image.center = { 10 / 2, 10 / 2, 0 };
	bullet[0][0].image.color = { 1, 1, 1, 1 };

	bullet[0][0].v = 1.5f;

	for (int i = 0; i < ADD_BULLET_NUM; i++)
	{
		for (int j = 0; j < MAX_BULLET_NUM; j++)
		{
			bullet[i][j] = bullet[0][0];
		}
	}
}

VOID BulletRelease()
{
	for (int i = 0; i < ADD_BULLET_NUM; i++)
	{
		for (int j = 0; j < MAX_BULLET_NUM; j++)
		{
			if (bullet[i][j].image.texture != NULL)
			{
				bullet[i][j].image.texture->Release();
			}
		}
	}
}

VOID BulletUpdate()
{
	for (int i = 0; i < ADD_BULLET_NUM; i++)
	{
		for (int j = 0; j < MAX_BULLET_NUM; j++)
		{
			for (int k = 0; k < nEnemyIdx; k++)
			{
				if (Collision(enemy[k].image.position, enemy[k].image.center.y, bullet[i][j].image.position, bullet[i][j].image.center.x) && enemy[k].image.bVisible == TRUE && bullet[i][j].image.bVisible == TRUE)
				{
					g_Score += 5;
					enemy[k].prop.damageTime = 200;
					enemy[k].prop.HP -= player.prop.damage;
					bullet[i][j].image.position = player.image.position;
					bullet[i][j].image.bVisible = FALSE;
				}
			}

			if (Collision(stage1Sub.image.position, stage1Sub.image.center.x, bullet[i][j].image.position, bullet[i][j].image.center.y) && stage1Sub.image.bVisible == TRUE && bullet[i][j].image.bVisible == TRUE)
			{
				g_Score += 5;
				stage1Sub.prop.damageTime = 100;
				stage1Sub.prop.HP -= player.prop.damage;
				bullet[i][j].image.position = player.image.position;
				bullet[i][j].image.bVisible = FALSE;
			}

			if (Collision(stage1Boss.image.position, stage1Boss.image.center.x, bullet[i][j].image.position, bullet[i][j].image.center.y) && stage1Boss.image.bVisible == TRUE && bullet[i][j].image.bVisible == TRUE)
			{
				g_Score += 5;
				stage1Boss.prop.damageTime = 100;
				stage1Boss.prop.HP -= player.prop.damage;
				bullet[i][j].image.position = player.image.position;
				bullet[i][j].image.bVisible = FALSE;
			}

			if (bullet[i][j].image.bVisible == TRUE)
			{
				bullet[i][j].image.position.x += (dwElapsedTime * bullet[i][j].v * cosf(D3DXToRadian(270 - 5 * (player.prop.addBulletNum / 2 + 1) + 5 * (i + 1))));
				bullet[i][j].image.position.y += (dwElapsedTime * bullet[i][j].v * sinf(D3DXToRadian(270 - 5 * (player.prop.addBulletNum / 2 + 1) + 5 * (i + 1))));
			}

			if (bullet[i][j].image.position.x > SCREEN_WIDTH || bullet[i][j].image.position.x < 0 || bullet[i][j].image.position.y > SCREEN_HEIGHT || bullet[i][j].image.position.y < 0)
			{
				bullet[i][j].image.position = player.image.position;
				bullet[i][j].image.bVisible = FALSE;
			}
		}
	}
}

VOID BulletRender()
{
	for (int i = 0; i < ADD_BULLET_NUM; i++)
	{
		for (int j = 0; j < MAX_BULLET_NUM; j++)
		{
			if (bullet[i][j].image.bVisible == TRUE)
				g_pSprite->Draw(bullet[i][j].image.texture, &bullet[i][j].image.rect, &bullet[i][j].image.center, &bullet[i][j].image.position, bullet[i][j].image.color);
		}
	}
}
