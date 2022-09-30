#include "Enemy.h"

extern LPDIRECT3DDEVICE9		g_pd3dDevice; // Our rendering device
extern LPD3DXSPRITE				g_pSprite;

extern ENEMY					enemy[MAX_ENEMY_NUM];
extern ENEMY_BULLET				enemyBullet[MAX_BULLET_NUM];

extern DWORD					dwCurTime;
extern DWORD					dwStartTime;
extern DWORD					dwElapsedTime;

extern PLAYER_BULLET			bullet[ADD_BULLET_NUM][MAX_BULLET_NUM];
extern PLAYER					player;

extern INT						g_Score;

extern BOSS						stage1Boss;
extern BOSS						stage1Sub;

extern IMAGE					shield;

extern BOOL						mainBoss;
extern IMAGE					enemy1;
extern IMAGE					enemy2;
extern IMAGE					effect;
extern FLOAT					effectTimer;

extern BOOL						bStageEnd;

INT spawnInterval;

INT nEnemyIdx;

VOID EnemyInit()
{
	spawnInterval = 2000;
	for (int i = 0; i < MAX_ENEMY_NUM; i++)
	{
		enemy[i].prop.appearTime = 3000 + (i * spawnInterval);
		enemy[i].prop.HP = 0;
		enemy[i].prop.v = 0.25f;
		enemy[i].prop.fireSpeed = 1000;
	}

	enemyBullet[0].image.texture = bullet[0][0].image.texture;
	enemyBullet[0].image.bVisible = FALSE;
	enemyBullet[0].image.rect = { 58, 138, 58 + 10, 138 + 10 };
	enemyBullet[0].image.color = { 1, 1, 1, 1 };
	enemyBullet[0].image.center = { 10 / 2, 10 / 2, 0 };
	enemyBullet[0].image.position = { -10, -10, 0 };

	for (int i = 0; i < MAX_BULLET_NUM; i++)
	{
		enemyBullet[i].image = enemyBullet[0].image;

		enemyBullet[i].v = 0.7f;
	}

	nEnemyIdx = 0;
}

VOID EnemyRelease()
{
	for (int i = 0; i < MAX_ENEMY_NUM; i++)
	{
		if (enemy[i].image.texture != NULL)
		{
			enemy[i].image.texture->Release();
		}
	}
}

VOID EnemyUpdate()
{
	for (int i = nEnemyIdx; i < MAX_ENEMY_NUM; i++)
	{
		if (dwCurTime - dwStartTime > enemy[i].prop.appearTime)
		{
			if (stage1Boss.image.bVisible == TRUE || stage1Sub.image.bVisible == TRUE || bStageEnd == TRUE)
			{
				enemy[i].prop.HP = 0;
				enemy[i].image.bVisible = FALSE;
				nEnemyIdx++;
				break;
			}
			if (enemy[i].image.bVisible == FALSE)
			{
				enemy[i].prop.HP = 9 + (player.prop.lv - 1) * 2;
				
				if (mainBoss == FALSE)
				{
					enemy[i].image = enemy1;
					enemy[i].prop.enemyType = TYPE1;
				}
				else
				{
					enemy[i].image = enemy2;
					enemy[i].prop.enemyXmovement = 1;
					enemy[i].prop.enemyType = TYPE2;
					enemy[i].prop.HP -= 5;
				}
				enemy[i].image.bVisible = TRUE;
				enemy[i].image.position = { (FLOAT)(rand() % 1000 + 200), 0, 0 };

				nEnemyIdx++;
			}
		}
		else
			break;
	}

	for (int i = 0; i < nEnemyIdx; i++)
	{
		//·¹º§¾÷
		if (enemy[i].prop.HP <= 0 && enemy[i].image.bVisible == TRUE)
		{
			if (player.prop.lv <= 4)
			{
				player.prop.EXP++;
			}
			g_Score += 50;
			enemy[i].image.bVisible = FALSE;
		}

		if (enemy[i].image.bVisible == TRUE)
		{
			if (enemy[i].prop.damageTime > 0)
			{
				enemy[i].image.color = { 1, 0, 0, 1 };
				enemy[i].prop.damageTime -= dwElapsedTime;
			}
			else
			{
				enemy[i].image.color = { 1, 1, 1, 1 };
			}
			switch (enemy[i].prop.enemyType)
			{
			case TYPE1:
				enemy[i].image.position.y += (dwElapsedTime * enemy[i].prop.v);
				break;
			case TYPE2:
				enemy[i].prop.enemyOldTime += dwElapsedTime;
				if (enemy[i].prop.enemyOldTime > 500)
				{
					enemy[i].prop.enemyOldTime = 0;
					enemy[i].prop.enemyXmovement *= -1;
				}
				enemy[i].image.position.y += (dwElapsedTime * (enemy[i].prop.v / 2));
				enemy[i].image.position.x += (dwElapsedTime * enemy[i].prop.v * enemy[i].prop.enemyXmovement);
				break;
			default:
				break;
			}
			enemy[i].prop.fireTimer += dwElapsedTime;
			if (enemy[i].prop.fireTimer >= enemy[i].prop.fireSpeed)
			{
				enemy[i].prop.fireTimer = 0;
				for (int j = 0; j < MAX_BULLET_NUM; j++)
				{
					if (enemyBullet[j].image.bVisible == FALSE)
					{
						enemyBullet[j].image.position = enemy[i].image.position;
						enemyBullet[j].aimPos = player.image.position;
						enemyBullet[j].dX = enemyBullet[j].aimPos.x - enemyBullet[j].image.position.x;
						enemyBullet[j].dY = enemyBullet[j].aimPos.y - enemyBullet[j].image.position.y;
						enemyBullet[j].dis = sqrtf(enemyBullet[j].dX * enemyBullet[j].dX + enemyBullet[j].dY * enemyBullet[j].dY);
						enemyBullet[j].stepX = enemyBullet[j].dX / enemyBullet[j].dis;
						enemyBullet[j].stepY = enemyBullet[j].dY / enemyBullet[j].dis;

						enemyBullet[j].image.bVisible = TRUE;

						break;
					}
				}
			}

			

			if (Collision(enemy[i].image.position, 20, player.image.position, 20) && player.image.bVisible == TRUE && enemy[i].image.bVisible == TRUE)
			{
				enemy[i].prop.HP = 0;
				enemy[i].image.position = { -10, -10, 0 };

				if (player.prop.isInvincible == FALSE)
				{
					player.prop.damageTime = 200;
					player.prop.HP -= 2;
				}
			}
		}

		if (enemy[i].image.position.y > SCREEN_HEIGHT)
		{
			enemy[i].prop.HP = 0;
			enemy[i].image.bVisible = FALSE;
		}
	}

	for (int i = 0; i < MAX_BULLET_NUM; i++)
	{
		if (enemyBullet[i].image.bVisible == TRUE)
		{
			if (Collision(enemyBullet[i].image.position, enemyBullet[i].image.center.x, shield.position, shield.center.x) && enemyBullet[i].image.bVisible == TRUE && shield.bVisible == TRUE)
			{
				enemyBullet[i].image.position = { -10, -10, 0 };
				enemyBullet[i].image.bVisible = FALSE;
			}

			if (Collision(enemyBullet[i].image.position, 15, player.image.position, 24) && enemyBullet[i].image.bVisible == TRUE && player.image.bVisible == TRUE)
			{
				enemyBullet[i].image.position = { -10, -10, 0 };
				enemyBullet[i].image.bVisible = FALSE;
				if (player.prop.isInvincible == FALSE)
				{
					effect.color = { 1, 0, 0, 0 };
					effectTimer = 500;
					player.prop.HP--;
					player.prop.damageTime = 100;
				}
			}
			enemyBullet[i].image.position.x += (dwElapsedTime * enemyBullet[i].v * enemyBullet[i].stepX);
			enemyBullet[i].image.position.y += (dwElapsedTime * enemyBullet[i].v * enemyBullet[i].stepY);
		}

		if (enemyBullet[i].image.position.x > SCREEN_WIDTH || enemyBullet[i].image.position.x < 0 || enemyBullet[i].image.position.y > SCREEN_HEIGHT || enemyBullet[i].image.position.y < 0)
		{
			enemyBullet[i].image.bVisible = FALSE;
			enemyBullet[i].image.position = { -10, -10, 0 };
		}
	}
}

VOID EnemyRender()
{
	for (int i = 0; i < nEnemyIdx; i++)
	{
		if (enemy[i].image.bVisible == TRUE)
			g_pSprite->Draw(enemy[i].image.texture, &enemy[i].image.rect, &enemy[i].image.center, &enemy[i].image.position, enemy[i].image.color);
	}
	for (int i = 0; i < MAX_BULLET_NUM; i++)
	{
		if(enemyBullet[i].image.bVisible == TRUE)
			g_pSprite->Draw(enemyBullet[i].image.texture, &enemyBullet[i].image.rect, &enemyBullet[i].image.center, &enemyBullet[i].image.position, enemyBullet[i].image.color);
	}
}
