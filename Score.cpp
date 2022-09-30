#include "Score.h"

extern LPDIRECT3DDEVICE9		g_pd3dDevice;
extern LPD3DXSPRITE				g_pSprite;
extern LPD3DXFONT				g_Font32;
extern LPD3DXFONT				g_Font80;
extern PLAYER					player;
extern ENEMY					enemy[MAX_ENEMY_NUM];

extern DWORD					dwStartTime;
extern DWORD					dwCurTime;
extern DWORD					dwElapsedTime;

extern BOSS						stage1Boss;

extern INT						nEnemyIdx;
extern INT						g_Score;

FLOAT							shieldTextTimer;

extern BOOL						bStageEnd;

char							string[100];



VOID ScoreInit()
{
	D3DXCreateFont(	g_pd3dDevice, 32, 0, FW_BOLD, 1, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
					DEFAULT_PITCH, L"¸¼Àº °íµñ", &g_Font32);

	D3DXCreateFont(	g_pd3dDevice, 80, 0, FW_NORMAL, 1, FALSE,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
					DEFAULT_PITCH, L"¸¼Àº °íµñ", &g_Font80);

}

VOID ScoreRelease()
{
	if (g_Font32 != NULL)
		g_Font32->Release();
	if (g_Font80 != NULL)
		g_Font80->Release();
}

VOID ScoreUpdate()
{
}

VOID ScoreRender()
{
	RECT HPRt;
	RECT lvRt;
	RECT EXPRt;
	RECT timerRt;
	RECT scoreRt;
	RECT resultRt;
	RECT isSucceededRt;
	RECT skillRt;
	RECT exitRt;

	if (bStageEnd != TRUE)
	{
		SetRect(&lvRt, 0, 10, 20, 42);
		sprintf(string, "Lv : %d", (INT)player.prop.lv);
		g_Font32->DrawTextA(NULL, string, -1, &lvRt, DT_NOCLIP, D3DXCOLOR(1.0, 1.0, 1.0, 1.0));

		SetRect(&HPRt, 0, 42, 20, 74);
		sprintf(string, "HP : %d / %d", (INT)player.prop.HP, (INT)player.prop.maxEXP);
		g_Font32->DrawTextA(NULL, string, -1, &HPRt, DT_NOCLIP, D3DXCOLOR(1.0, 1.0, 1.0, 1.0));

		SetRect(&HPRt, 0, 74, 20, 106);
		sprintf(string, "EXP : %d / %d", (INT)player.prop.EXP, (INT)player.prop.maxEXP);
		g_Font32->DrawTextA(NULL, string, -1, &HPRt, DT_NOCLIP, D3DXCOLOR(1.0, 1.0, 1.0, 1.0));

		SetRect(&scoreRt, 0, 74 + 32, 20, 106 + 32);
		sprintf(string, "SCORE : %d", (INT)g_Score);
		g_Font32->DrawTextA(NULL, string, -1, &scoreRt, DT_NOCLIP, D3DXCOLOR(1.0, 1.0, 1.0, 1.0));


		SetRect(&timerRt, (SCREEN_WIDTH / 2) - 32, 10, (SCREEN_WIDTH / 2), 42);
		sprintf(string, "%d : %d", (INT)((INT)dwCurTime / 1000 - dwStartTime / 1000) / 60, (INT)((INT)dwCurTime / 1000 - dwStartTime / 1000) % 60);
		g_Font32->DrawTextA(NULL, string, -1, &timerRt, DT_NOCLIP, D3DXCOLOR(1.0, 1.0, 1.0, 1.0));

		SetRect(&skillRt, (SCREEN_WIDTH / 2) - 32 * 6, SCREEN_HEIGHT / 2 - 16, (SCREEN_WIDTH / 2), SCREEN_HEIGHT / 2 + 16);
		sprintf(string, "¾ÆÁ÷ ½ºÅ³À» »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù.");
		if (shieldTextTimer > 0 && player.prop.shieldCoolDown <= 0)
		{
			shieldTextTimer -= dwElapsedTime;
			g_Font32->DrawTextA(NULL, string, -1, &skillRt, DT_NOCLIP, D3DXCOLOR(1.0, 1.0, 1.0, 1.0));
		}
	}
	else
	{
		SetRect(&isSucceededRt, 61, 215, 61 + 80, 500);
		if (player.prop.HP <= 0)
		{
			sprintf(string, "FAILED");
			g_Font80->DrawTextA(NULL, string, -1, &isSucceededRt, DT_NOCLIP, D3DXCOLOR(1.0, 0.0, 0.0, 1.0));

		}
		else
		{
			sprintf(string, "SUCCESS!");
			g_Font80->DrawTextA(NULL, string, -1, &isSucceededRt, DT_NOCLIP, D3DXCOLOR(1.0, 1.0, 0.0, 1.0));
		}
		sprintf(string, "Score : %d", g_Score);
		SetRect(&scoreRt, 61, 215 + 80, 61 + 80, 500 + 160);
		g_Font80->DrawTextA(NULL, string, -1, &scoreRt, DT_NOCLIP, D3DXCOLOR(1.0, 1.0, 1.0, 1.0));

		sprintf(string, "Exit To Space");
		SetRect(&exitRt, 61, 780, 61 + 80, 780 + 80);
		g_Font80->DrawTextA(NULL, string, -1, &exitRt, DT_NOCLIP, D3DXCOLOR(1.0, 1.0, 1.0, 1.0));

	}
}
