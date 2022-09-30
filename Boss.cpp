#include "Boss.h"

extern LPDIRECT3DDEVICE9		g_pd3dDevice; // Our rendering device
extern LPD3DXSPRITE				g_pSprite;

extern BOSS						stage1Sub;
extern BOSS						stage1Boss;

extern DWORD					dwCurTime;
extern DWORD					dwStartTime;
extern DWORD					dwElapsedTime;
extern PLAYER					player;
extern IMAGE					effect;
extern FLOAT					effectTimer;

extern IMAGE					shield;

extern BOSS_BULLET				bossBullet[BOSS_ADD_BULLET_NUM][MAX_BULLET_NUM];

extern BOOL						bStageEnd;

extern INT						g_Score;

INT nBulletIdx;

INT subMovement;
INT subAddNum;

BOOL mainBoss;

VOID BossInit()
{
	mainBoss = TRUE;

	subMovement = 1;

	stage1Sub.image.rect = { 0, 0, 134, 199 };
	stage1Sub.image.center = { 134 / 2, 199 / 2, 0 };
	stage1Sub.image.position = { SCREEN_WIDTH / 2, -199 / 2, 0 };
	stage1Sub.image.color = { 1, 1, 1, 1 };
	stage1Sub.image.bVisible = FALSE;
	stage1Sub.prop.isPatPlaying = TRUE;

	stage1Sub.prop.HP = 600;
	stage1Sub.prop.maxHP = 600;
	stage1Sub.prop.v = 0.2f;
	stage1Sub.prop.appearTime = 60000;
	stage1Sub.deadEvent = FALSE;

	stage1Sub.prop.fireSpeed = 400;

	//

	stage1Boss.image.rect = { 0, 0, 398, 225 };
	stage1Boss.image.position = { SCREEN_WIDTH / 2, -225 / 2, 0 };
	stage1Boss.image.color = { 1, 1, 1, 1 };
	stage1Boss.image.center = { 398 / 2, 225 / 2, 0 };
	stage1Boss.image.bVisible = FALSE;
	stage1Boss.prop.isPatPlaying = TRUE;

	stage1Boss.prop.HP = 1000;
	stage1Boss.prop.maxHP = 1000;
	stage1Boss.prop.pattern = LEFT_TO_RIGHT;
	stage1Boss.prop.v = 0.2f;
	stage1Boss.prop.appearTime = 5000;
	stage1Boss.deadEvent = FALSE;
	stage1Boss.prop.fireSpeed = 50;
	
	bossBullet[0][0].image.rect = { 128, 134, 128 + 14, 134 + 14 };
	bossBullet[0][0].image.bVisible = FALSE;
	bossBullet[0][0].image.center = { 14 / 2, 14 / 2, 0 };
	bossBullet[0][0].image.position = { -10, -10, 0 };
	bossBullet[0][0].image.color = { 1, 1, 1, 1 };

	bossBullet[0][0].v = 0.7f;
	bossBullet[0][0].pattern = NONE;

	subAddNum = 3;
	for (int i = 0; i < BOSS_ADD_BULLET_NUM; i++)
	{
		for (int j = 0; j < MAX_BULLET_NUM; j++)
		{
			bossBullet[i][j] = bossBullet[0][0];
		}
	}
}

VOID BossRelease()
{
	if (stage1Boss.image.texture != NULL)
		stage1Boss.image.texture->Release();

	if (stage1Sub.image.texture != NULL)
		stage1Sub.image.texture->Release();

	for (int i = 0; i < BOSS_ADD_BULLET_NUM; i++)
	{
		for (int j = 0; j < MAX_BULLET_NUM; j++)
		{
			if (bossBullet[i][j].image.texture != NULL)
				bossBullet[i][j].image.texture->Release();
		}
	}
}

VOID BossUpdate()
{
	if (mainBoss != TRUE)		//서브
	{
		if(dwCurTime - dwStartTime > stage1Sub.prop.appearTime)
		{
			if (stage1Sub.prop.HP <= 0)
			{
				if (stage1Sub.deadEvent == FALSE)
				{
					stage1Sub.deadEvent = TRUE;
					g_Score += 100;
					player.prop.HP = player.prop.maxHP;
					stage1Boss.prop.appearTime = dwCurTime - dwStartTime + 60000;
					mainBoss = TRUE;
				}
					stage1Sub.image.bVisible = FALSE;
			}
			else
			{
				stage1Sub.image.bVisible = TRUE;
			}
			stage1Sub.prop.oldTime = 0;
		}

		if (stage1Sub.image.bVisible == TRUE)
		{
			if (stage1Sub.prop.damageTime > 0)
			{
				stage1Sub.image.color = D3DXCOLOR(1, 0, 0, 1);
				stage1Sub.prop.damageTime -= dwElapsedTime;
			}
			else
			{
				stage1Sub.image.color = D3DXCOLOR(1, 1, 1, 1);
			}

			if (stage1Sub.image.position.y <= stage1Sub.image.rect.bottom)
			{
				stage1Sub.image.position.y += (dwElapsedTime * stage1Sub.prop.v);
			}
			else
			{
				subAddNum =  (INT)(5 - (stage1Sub.prop.HP / stage1Sub.prop.maxHP) * 4) * 2 - 1;
				stage1Sub.prop.fireSpeed = 200 + (100 * (stage1Sub.prop.HP / stage1Sub.prop.maxHP) * 4 * 2 - 1);

				stage1Sub.image.position.x += (dwElapsedTime * stage1Sub.prop.v * subMovement);

				if (stage1Sub.image.position.x < 0 + stage1Sub.image.center.x)
				{
					subMovement = 1;
				}
				else if(stage1Sub.image.position.x > SCREEN_WIDTH - stage1Sub.image.center.x)
				{
					subMovement = -1;
				}
				stage1Sub.prop.fireTimer += dwElapsedTime;
				if (stage1Sub.prop.fireTimer > stage1Sub.prop.fireSpeed)
				{
					stage1Sub.prop.fireTimer = 0;
					for (int i = 0; i < subAddNum; i++)
					{
						for (int j = 0; j < MAX_BULLET_NUM; j++)
						{
							if (bossBullet[i][j].image.bVisible == FALSE)
							{
								bossBullet[i][j].image.position = stage1Sub.image.position;
								bossBullet[i][j].image.bVisible = TRUE;
								bossBullet[i][j].pattern = ADD;
								break;
							}
						}
					}

				}
			}
		}

	}
	else						//메인
	{
		if (dwCurTime - dwStartTime > stage1Boss.prop.appearTime)
		{
			if (stage1Boss.prop.HP <= 0)
			{
				if (stage1Boss.deadEvent == FALSE)
				{
					stage1Boss.deadEvent = TRUE;
					g_Score += 100;
					g_Score += player.prop.HP * 100;
					bStageEnd = TRUE;
					player.prop.isInvincible = TRUE;
					mainBoss = TRUE;
				}
				stage1Boss.image.bVisible = FALSE;
			}
			else
			{
				stage1Boss.image.bVisible = TRUE;
			}
			stage1Boss.prop.oldTime = 0;
		}

		if (stage1Boss.image.bVisible == TRUE)
		{
			if (stage1Boss.prop.damageTime > 0)
			{
				stage1Boss.image.color = D3DXCOLOR(1, 0, 0, 1);
				stage1Boss.prop.damageTime -= dwElapsedTime;
			}
			else
			{
				stage1Boss.image.color = D3DXCOLOR(1, 1, 1, 1);
			}

			if (stage1Boss.image.position.y <= stage1Boss.image.rect.bottom)
			{
				stage1Boss.image.position.y += (dwElapsedTime * stage1Boss.prop.v);
			}
			else
			{
				if (stage1Boss.prop.isPatPlaying)
					switch (stage1Boss.prop.pattern)
					{
					case LEFT_TO_RIGHT:
						if (nBulletIdx > 60)
						{
							nBulletIdx = 0;
							stage1Boss.prop.isPatPlaying = FALSE;
						}
						stage1Boss.prop.fireTimer += dwElapsedTime;
						for (int k = 0; k < 3; k++)
						{
							for (int i = 0; i < BOSS_ADD_BULLET_NUM; i++)
							{
								for (int j = 0; j < MAX_BULLET_NUM; j++)
								{
									if (bossBullet[i][j].image.bVisible == FALSE && stage1Boss.prop.fireTimer > 50)
									{
										stage1Boss.prop.fireTimer = 0;
										nBulletIdx++;
										bossBullet[i][j].image.position = stage1Boss.image.position;
										bossBullet[i][j].pattern = LEFT_TO_RIGHT;
										bossBullet[i][j].image.bVisible = TRUE;
									}
									break;
								}
							}
						}
						break;
					case RIGHT_TO_LEFT:
						if (nBulletIdx > 60)
						{
							nBulletIdx = 0;
							stage1Boss.prop.isPatPlaying = FALSE;
						}
						stage1Boss.prop.fireTimer += dwElapsedTime;
						for (int i = 0; i < BOSS_ADD_BULLET_NUM; i++)
						{
							for (int j = 0; j < MAX_BULLET_NUM; j++)
							{
								if (bossBullet[i][j].image.bVisible == FALSE && stage1Boss.prop.fireTimer > 50)
								{
									stage1Boss.prop.fireTimer = 0;
									nBulletIdx++;
									bossBullet[i][j].image.position = stage1Boss.image.position;
									bossBullet[i][j].pattern = RIGHT_TO_LEFT;
									bossBullet[i][j].image.bVisible = TRUE;

								}
								break;
							}
						}
						break;

					case CIRCLE:
						if (nBulletIdx > 20)
						{
							nBulletIdx = 0;
							stage1Boss.prop.isPatPlaying = FALSE;
						}
						stage1Boss.prop.fireTimer += dwElapsedTime;
						if (stage1Boss.prop.fireTimer > 150)
						{
							nBulletIdx++;
							stage1Boss.prop.fireTimer = 0;
							for (int i = 0; i < BOSS_ADD_BULLET_NUM; i++)
							{
								for (int j = 0; j < MAX_BULLET_NUM; j++)
								{

									if (bossBullet[i][j].image.bVisible == FALSE)
									{
										bossBullet[i][j].image.position = stage1Boss.image.position;
										bossBullet[i][j].pattern = CIRCLE;
										bossBullet[i][j].image.bVisible = TRUE;
										break;
									}
								}
							}
						}
						break;

					default:
						break;
					}
				}
			}
		else
		{
			stage1Boss.prop.fireTimer += dwElapsedTime;
			if (stage1Boss.prop.fireTimer > 100)
			{
				stage1Boss.prop.fireTimer = 0;
				for (int i = 0; i < BOSS_ADD_BULLET_NUM; i++)
				{
					for (int j = 0; j < MAX_BULLET_NUM; j++)
					{
						if (bossBullet[i][j].image.bVisible == FALSE)
						{
							bossBullet[i][j].image.position = stage1Boss.image.position;
							bossBullet[i][j].image.bVisible = TRUE;
							bossBullet[i][j].aimPos = player.image.position;
							bossBullet[i][j].pattern = TRACE;

							bossBullet[i][j].dX = bossBullet[i][j].aimPos.x - bossBullet[i][j].image.position.x;
							bossBullet[i][j].dY = bossBullet[i][j].aimPos.y - bossBullet[i][j].image.position.y;
							bossBullet[i][j].dis = sqrtf(bossBullet[i][j].dX * bossBullet[i][j].dX + bossBullet[i][j].dY * bossBullet[i][j].dY);

							bossBullet[i][j].stepX = bossBullet[i][j].dX / bossBullet[i][j].dis;		//방향코사인
							bossBullet[i][j].stepY = bossBullet[i][j].dY / bossBullet[i][j].dis;		//방향코사인
							break;
						}
					}
				}
			}
			stage1Boss.prop.nextPatDelay += dwElapsedTime;
		}
	}
	for (int i = 0; i < BOSS_ADD_BULLET_NUM; i++)
	{
		for (int j = 0; j < MAX_BULLET_NUM; j++)
		{
			if (bossBullet[i][j].image.bVisible == TRUE)
			{
				switch (bossBullet[i][j].pattern)
				{
				case LEFT_TO_RIGHT:
					bossBullet[i][j].image.position.x += (dwElapsedTime * bossBullet[i][j].v * cosf(D3DXToRadian(i * 9)));
					bossBullet[i][j].image.position.y += (dwElapsedTime * bossBullet[i][j].v * sinf(D3DXToRadian(i * 9)));
					break;
				case RIGHT_TO_LEFT:
					bossBullet[i][j].image.position.x += (dwElapsedTime * bossBullet[i][j].v * cosf(D3DXToRadian(180 - (i * 9))));
					bossBullet[i][j].image.position.y += (dwElapsedTime * bossBullet[i][j].v * sinf(D3DXToRadian(180 - (i * 9))));
					break;
				case CIRCLE:
					bossBullet[i][j].image.position.x += (dwElapsedTime * bossBullet[i][j].v * cosf(D3DXToRadian(i * 18 + j)));
					bossBullet[i][j].image.position.y += (dwElapsedTime * bossBullet[i][j].v * sinf(D3DXToRadian(i * 18 + j)));
					break;
				case TRACE:
					bossBullet[i][j].image.position.x += (dwElapsedTime * bossBullet[i][j].v * bossBullet[i][j].stepX);
					bossBullet[i][j].image.position.y += (dwElapsedTime * bossBullet[i][j].v * bossBullet[i][j].stepY);
					break;
				case ADD:
					bossBullet[i][j].image.position.x += (dwElapsedTime * bossBullet[i][j].v * cosf(D3DXToRadian(90 - 15 * (subAddNum / 2 + 1) + 15 * (i + 1))));
					bossBullet[i][j].image.position.y += (dwElapsedTime * bossBullet[i][j].v * sinf(D3DXToRadian(90 - 15 * (subAddNum / 2 + 1) + 15 * (i + 1))));
					break;
				default:
					break;
				}
				if (mainBoss != TRUE)
				{
					if (sqrtf(((stage1Sub.image.position.x - bossBullet[i][j].image.position.x) * (stage1Sub.image.position.x - bossBullet[i][j].image.position.x))
						+ ((stage1Sub.image.position.y - bossBullet[i][j].image.position.y) * (stage1Sub.image.position.y - bossBullet[i][j].image.position.y))) > 1000)
					{
						bossBullet[i][j].image.bVisible = FALSE;
						bossBullet[i][j].pattern = NONE;
						bossBullet[i][j].image.position = { -10, -10, 0 };
					}
				}
				else
				{
					if (sqrtf(((stage1Boss.image.position.x - bossBullet[i][j].image.position.x) * (stage1Boss.image.position.x - bossBullet[i][j].image.position.x))
						+ ((stage1Boss.image.position.y - bossBullet[i][j].image.position.y) * (stage1Boss.image.position.y - bossBullet[i][j].image.position.y))) > 1000)
					{
						bossBullet[i][j].image.bVisible = FALSE;
						bossBullet[i][j].pattern = NONE;
						bossBullet[i][j].image.position = { -10, -10, 0 };
					}
				}
				if (Collision(bossBullet[i][j].image.position, bossBullet[i][j].image.center.x, shield.position, shield.center.x) && bossBullet[i][j].image.bVisible == TRUE && shield.bVisible == TRUE)
				{
					bossBullet[i][j].image.position = { -10, -10, 0 };
					bossBullet[i][j].image.bVisible = FALSE;
				}
				if (Collision(bossBullet[i][j].image.position, bossBullet[i][j].image.center.x, player.image.position, player.image.center.x) && player.image.bVisible == TRUE && bossBullet[i][j].image.bVisible == TRUE)
				{
					bossBullet[i][j].image.bVisible = FALSE;
					bossBullet[i][j].pattern = NONE;
					bossBullet[i][j].image.position = { -10, -10, 0 };
					if (player.prop.isInvincible == FALSE)
					{
						player.prop.HP--;
						effect.color = { 1, 0, 0, 0 };
						effectTimer = 500;
						player.prop.damageTime = 100;
					}
				}

			}
		}
	}
}

VOID BossRender()
{
	if (stage1Sub.image.bVisible == TRUE)
		g_pSprite->Draw(stage1Sub.image.texture, &stage1Sub.image.rect, &stage1Sub.image.center, &stage1Sub.image.position, stage1Sub.image.color);

	if (stage1Boss.image.bVisible == TRUE)
		g_pSprite->Draw(stage1Boss.image.texture, &stage1Boss.image.rect, &stage1Boss.image.center, &stage1Boss.image.position, stage1Boss.image.color);

	for (int i = 0; i < BOSS_ADD_BULLET_NUM; i++)
	{
		for (int j = 0; j < MAX_BULLET_NUM; j++)
		{
			if(bossBullet[i][j].image.bVisible == TRUE)
				g_pSprite->Draw(bossBullet[i][j].image.texture, &bossBullet[i][j].image.rect, &bossBullet[i][j].image.center, &bossBullet[i][j].image.position, bossBullet[i][j].image.color);
		}
	}
}
