#include "Background.h"

extern LPDIRECT3DDEVICE9		g_pd3dDevice;  // Our rendering device
extern LPD3DXSPRITE				g_pSprite;

extern IMAGE					background[2];

extern DWORD					dwElapsedTime;

extern BOSS						stage1Boss;
VOID BackgroundInit()
{
	for (int i = 0; i < 2; i++)
	{
		D3DXCreateTextureFromFileEx(g_pd3dDevice, L"\Resource/Background.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
			D3DX_FILTER_NONE, 0, 0, 0, 0, &background[i].texture);
		background[i].bVisible = TRUE;
		background[i].center = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0 };
		background[i].color = { 1, 1, 1, 1 };
		background[i].position = { (FLOAT)(SCREEN_WIDTH / 2), (FLOAT)(SCREEN_HEIGHT / 2 + SCREEN_HEIGHT * i), 0 };
		background[i].rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	}
}

VOID BackgroundRelease()
{
	for (int i = 0; i < 2; i++)
	{
		if (background[i].texture != NULL)
			background[i].texture->Release();
	}
}

VOID BackgroundUpdate()
{
	for (int i = 0; i < 2; i++)
	{
		if (stage1Boss.image.bVisible == FALSE)
		{
			background[i].position.y += 1.5f;
			if (background[i].position.y >= SCREEN_HEIGHT / 2 + SCREEN_HEIGHT)
				background[i].position.y = -SCREEN_HEIGHT / 2;
		}
	}
}

VOID BackgroundRender()
{
	for (int i = 0; i < 2; i++)
	{
		if (background[i].bVisible == TRUE)
			g_pSprite->Draw(background[i].texture, &background[i].rect, &background[i].center, &background[i].position, background[i].color);
	}
}
