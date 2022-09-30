#include "Player.h"

extern LPDIRECT3DDEVICE9		g_pd3dDevice; // Our rendering device
extern LPD3DXSPRITE				g_pSprite;
extern PLAYER					player;

extern DWORD					dwElapsedTime;

extern PLAYER_BULLET			bullet[ADD_BULLET_NUM][MAX_BULLET_NUM];

extern GAME_STATE				g_GameState;

extern IMAGE					shield;
extern IMAGE					effect;
extern FLOAT					effectTimer;
extern BOOL						bStageEnd;
extern INT						g_Score;

extern FLOAT					shieldTextTimer;

BOOL isKeyDown[4];

VOID PlayerInit()
{
	player.image.bVisible = TRUE;
	player.image.position = { SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.8f, 0 };
	player.image.rect = { 0, 0, 31, 48 };
	player.image.center = { 31 / 2, 48 / 2, 0 };
	player.image.color = { 1, 1, 1, 1 };

	player.prop.addBulletNum = 1;
	player.prop.originDamage = 3;
	player.prop.EXP = 0;
	player.prop.originSpeed = 300;
	player.prop.fireTimer = 0;
	player.prop.HP = 10;
	player.prop.isDirectional = TRUE;
	player.prop.isInvincible = FALSE;
	player.prop.lv = 1;
	player.prop.maxEXP = 10;
	player.prop.maxHP = 10;
	player.prop.v = 0.5f;

	shield.bVisible = FALSE;
	shield.color = { 1, 1, 1, 1 };
	shield.position = player.image.position;
	shield.rect = { 0, 0, 125, 120 };
	shield.center = { 125 / 2, 120 / 2, 0 };
	for (int i = 0; i < 4; i++)
	{
		isKeyDown[i] = FALSE;
	}
}

VOID PlayerRelease()
{
	if (player.image.texture != NULL)
		player.image.texture->Release();
}

VOID PlayerUpdate()
{
	if (player.prop.isInvincible == TRUE)
		player.image.color = { 1, 0, 0, 1 };
	else
		player.image.color = { 1, 1, 1, 1 };
	if (player.prop.EXP >= player.prop.maxEXP)
	{
		if (player.prop.lv <= 4)
		{
			effect.color = { 1, 1, 1, 0 };
			effectTimer = 500;

			player.prop.maxHP += (INT)player.prop.maxHP * 0.2f;
			player.prop.HP = (INT)player.prop.maxHP;
			player.prop.originDamage += (INT)player.prop.damage * 0.2f;
			player.prop.originSpeed -= (INT)player.prop.fireSpeed * 0.2f;
			player.prop.maxEXP += 2;
			player.prop.lv++;
		}
		player.prop.EXP = 0;
	}
	if (player.prop.isDirectional)
	{
		player.prop.addBulletNum = 1;
		player.prop.fireSpeed = player.prop.originSpeed;
		player.prop.damage = player.prop.originDamage;
	}
	else
	{
		player.prop.addBulletNum = 3;
		player.prop.fireSpeed = player.prop.originSpeed / 0.8f;
		player.prop.damage = (INT)player.prop.originDamage * 0.33f;
	}

	if (player.prop.shieldCoolDown > 0)
	{
		player.prop.shieldCoolDown -= dwElapsedTime;
		shield.position = player.image.position;
		shield.bVisible = TRUE;
		shield.color = { 1, 1, 1, player.prop.shieldCoolDown / 3000 };
	}
	else
	{
		if (player.prop.shieldCooltime > 0)
			player.prop.shieldCooltime -= dwElapsedTime;
		shield.bVisible = FALSE;
	}

	if (bStageEnd == TRUE)
	{
		if (GetAsyncKeyState(VK_SPACE) & 0xffff)
		{
			g_Score = 0;
			player.prop.isInvincible = FALSE;
			bStageEnd = FALSE;
			g_GameState = LOADING;
		}
	}

	if (player.prop.HP <= 0)
	{
		bStageEnd = TRUE;
		player.image.bVisible = FALSE;


	}
	else
	{
		if (bStageEnd == FALSE)
		{
			if (player.prop.damageTime > 0)
			{
				player.image.color = { 1, 0, 0, 1 };
				player.prop.damageTime -= dwElapsedTime;
			}
			else
			{
				player.image.color = { 1, 1, 1, 1 };
			}
			{
				if (GetAsyncKeyState(VK_LEFT) & 0xffff)
				{
					if (player.image.position.x - player.image.center.x > 0)
						player.image.position.x -= (dwElapsedTime * player.prop.v);
				}

				if (GetAsyncKeyState(VK_RIGHT) & 0xffff)
				{
					if (player.image.position.x + player.image.center.x < SCREEN_WIDTH)
						player.image.position.x += (dwElapsedTime * player.prop.v);
				}

				if (GetAsyncKeyState(VK_UP) & 0xffff)
				{
					if (player.image.position.y - player.image.center.y > 0)
						player.image.position.y -= (dwElapsedTime * player.prop.v);
				}

				if (GetAsyncKeyState(VK_DOWN) & 0xffff)
				{
					if (player.image.position.y + player.image.center.y < SCREEN_HEIGHT)
						player.image.position.y += (dwElapsedTime * player.prop.v);
				}

				if (GetAsyncKeyState('A') & 0xffff)
				{
					player.prop.fireTimer += dwElapsedTime;
					if (player.prop.fireTimer >= player.prop.fireSpeed)
					{
						player.prop.fireTimer = 0;
						for (int i = 0; i < player.prop.addBulletNum; i++)
						{
							for (int j = 0; j < MAX_BULLET_NUM; j++)
							{
								if (bullet[i][j].image.bVisible == FALSE)
								{
									if (player.prop.isDirectional == TRUE)
										bullet[i][j].image.rect = { 58, 99, 58 + 10, 99 + 10 };
									else
										bullet[i][j].image.rect = { 58, 158, 58 + 10, 158 + 10 };
									bullet[i][j].image.position = player.image.position;
									bullet[i][j].image.bVisible = TRUE;
									break;
								}
							}
						}
					}
				}
				else
				{
					player.prop.fireTimer = 0;
				}

				if (GetAsyncKeyState('D') & 0xffff && player.prop.lv >= 3)
				{
					if (player.prop.shieldCooltime <= 0)
					{
						player.prop.shieldCooltime = 10000;
						player.prop.shieldCoolDown = 3000;
					}
					else
					{
						shieldTextTimer = 500;
					}
				}

				if (GetAsyncKeyState('S') & 0xffff)
				{
					if (isKeyDown[0] == FALSE)
					{
						isKeyDown[0] = TRUE;
						player.prop.isDirectional = !player.prop.isDirectional;
					}
				}
				else
				{
					isKeyDown[0] = FALSE;
				}

				if (GetAsyncKeyState(VK_F1) & 0xffff)
				{
					if (isKeyDown[1] == FALSE)
					{
						isKeyDown[1] = TRUE;
						player.prop.isInvincible = !player.prop.isInvincible;
					}
				}
				else
				{
					isKeyDown[1] = FALSE;
				}
				if (GetAsyncKeyState(VK_F2) & 0xffff)
				{
					if (isKeyDown[2] == FALSE)
					{
						isKeyDown[2] = TRUE;
						if (player.prop.lv < 5)
							player.prop.EXP = player.prop.maxEXP;
					}
				}
				else
				{
					isKeyDown[2] = FALSE;
				}

				if (GetAsyncKeyState(VK_F3) & 0xffff)
				{
					if (isKeyDown[3] == FALSE)
					{
						isKeyDown[3] = TRUE;
						player.prop.isShowLv = !player.prop.isShowLv;
					}
				}
				else
				{
					isKeyDown[3] = FALSE;
				}
			}
			if (GetAsyncKeyState(VK_F3) & 0xffff)
			{
				g_GameState = LOADING;
			}
		}
	}
}
VOID PlayerRender()
{
	if (player.image.bVisible == TRUE)
		g_pSprite->Draw(player.image.texture, &player.image.rect, &player.image.center, &player.image.position, player.image.color);

	if (shield.bVisible == TRUE)
		g_pSprite->Draw(shield.texture, &shield.rect, &shield.center, &shield.position, shield.color);
}
