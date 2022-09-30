#include "UI.h"

extern LPDIRECT3DDEVICE9		g_pd3dDevice; // Our rendering device
extern LPD3DXSPRITE				g_pSprite;
extern DWORD					dwElapsedTime;

extern IMAGE					result;
extern IMAGE					HPBar;
extern IMAGE					effect;
extern IMAGE					shieldCoolImage;

extern PLAYER					player;

extern FLOAT					effectTimer;

extern BOOL						mainBoss;
extern BOOL						bStageEnd;

extern BOSS						stage1Sub;
extern BOSS						stage1Boss;

VOID UIInit()
{
	HPBar.bVisible = FALSE;
	HPBar.center = { 200 / 2, 25 / 2, 0 };
	HPBar.color = { 1, 1, 1, 1 };
	HPBar.position = { SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.8, 0 };
	HPBar.rect = { 0, 0, 200, 25 };

	effect.bVisible = FALSE;
	effect.center = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0 };
	effect.color = { 1, 1, 1, 1 };
	effect.position = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0 };
	effect.rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	shieldCoolImage.bVisible = TRUE;
	shieldCoolImage.center = { 80 / 2, 64 / 2, 0 };
	shieldCoolImage.color = { 1, 1, 1, 1 };
	shieldCoolImage.position = { 80 / 2, 750, 0 };
	shieldCoolImage.rect = { 0, 0, 80, 64 };
}

VOID UIRelease()
{
	if (HPBar.texture != NULL)
		HPBar.texture->Release();

	if (effect.texture != NULL)
		effect.texture->Release();

	if (shieldCoolImage.texture != NULL)
		shieldCoolImage.texture->Release();
}

VOID UIUpdate()
{
	if (bStageEnd == TRUE)
		result.bVisible = TRUE;
	else
		result.bVisible = FALSE;


	if (player.prop.shieldCooltime > 0 || player.prop.lv < 3)
		shieldCoolImage.color = { 1, 1, 1, 0 };
	else
		shieldCoolImage.color = { 1,1,1,1 };


	if (effectTimer > 0)
	{
		effect.bVisible = TRUE;
		effectTimer -= dwElapsedTime;
		effect.color.a = (effectTimer / 500) / 2;
	}
	else
	{
		effect.bVisible = FALSE;
	}
	if (stage1Sub.image.bVisible == TRUE || stage1Boss.image.bVisible == TRUE)
	{
		HPBar.bVisible = TRUE;

		if(mainBoss == FALSE)
			HPBar.rect.right = stage1Sub.prop.HP / 600 * 200;
		else
			HPBar.rect.right = stage1Boss.prop.HP / 1000 * 200;
	}
	else
	{
		HPBar.bVisible = FALSE;
	}
}

VOID UIRender()
{
	if(HPBar.bVisible == TRUE)
		g_pSprite->Draw(HPBar.texture, &HPBar.rect, &HPBar.center, &HPBar.position, HPBar.color);

	if(shieldCoolImage.bVisible == TRUE)
		g_pSprite->Draw(shieldCoolImage.texture, &shieldCoolImage.rect, &shieldCoolImage.center, &shieldCoolImage.position, shieldCoolImage.color);

	if(effect.bVisible == TRUE)
		g_pSprite->Draw(effect.texture, &effect.rect, &effect.center, &effect.position, effect.color);

	if(result.bVisible == TRUE)
		g_pSprite->Draw(result.texture, &result.rect, &result.center, &result.position, result.color);
}
