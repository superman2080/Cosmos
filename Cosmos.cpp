#include "Gamedata.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Background.h"
#include "Score.h"
#include "Boss.h"
#include "UI.h"
#include "Sound.h"

LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPD3DXSPRITE			g_pSprite = NULL;

GAME_STATE				g_GameState;

DWORD					dwCurTime;
DWORD					dwStartTime;
DWORD					dwElapsedTime;
DWORD					dwOldTime;
DWORD					loadTimer;

PLAYER					player;

PLAYER_BULLET			bullet[ADD_BULLET_NUM][MAX_BULLET_NUM];

ENEMY					enemy[MAX_ENEMY_NUM];
ENEMY_BULLET			enemyBullet[MAX_BULLET_NUM];

INT						g_nStage;
INT						g_Score;

IMAGE					loading;
IMAGE					title;
IMAGE					result;
IMAGE					background[2];
IMAGE					shield;
IMAGE					shieldCoolImage;

IMAGE					effect;
FLOAT					effectTimer;

IMAGE					HPBar;

IMAGE					enemy1;
IMAGE					enemy2;
IMAGE					enemy3;

BOSS					stage1Sub;
BOSS					stage1Boss;
BOSS_BULLET				bossBullet[BOSS_ADD_BULLET_NUM][MAX_BULLET_NUM];

BOOL					bStageEnd;

LPD3DXFONT				g_Font32;
LPD3DXFONT				g_Font80;


HRESULT InitD3D( HWND hWnd )
{
    // Create the D3D object.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = FALSE;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;

    // Create the D3DDevice
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }
	D3DXCreateSprite(g_pd3dDevice, &g_pSprite);
	g_GameState = LOADING;
	g_nStage = 0;
	dwCurTime = GetTickCount();
	srand(time(NULL));
	GameInit();
    // Device state would normally be set here

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	GameRelease();
	if (g_pSprite != NULL)
		g_pSprite->Release();

    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}

VOID Update()
{
	GameUpdate();
}

VOID Render()
{
    // Clear the backbuffer to a blue color
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

    // Begin the scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
		g_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		GameRender();
		g_pSprite->End();
		if (g_GameState == STAGE1)
			ScoreRender();
        g_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}
		break;
        case WM_DESTROY:
            Cleanup();
            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
    UNREFERENCED_PARAMETER( hInst );

    // Register the window class
    WNDCLASSEX wc =
    {
        sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
        L"D3D Tutorial", NULL
    };
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( L"D3D Tutorial", L"D3D Tutorial 02: Cosmos",
                              WS_EX_TOPMOST | WS_POPUP, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                              NULL, NULL, wc.hInstance, NULL );

    // Initialize Direct3D
    if( SUCCEEDED( InitD3D( hWnd ) ) )
    {
            // Show the window
            ShowWindow( hWnd, SW_SHOWDEFAULT );
            UpdateWindow( hWnd );

            // Enter the message loop
            MSG msg;
            ZeroMemory( &msg, sizeof( msg ) );
            while( msg.message != WM_QUIT )
            {
                if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
                {
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
				else
				{
					Update();
                    Render();
				}
            }
    }

    UnregisterClass( L"D3D Tutorial", wc.hInstance );
    return 0;
}

VOID GameInit()
{
	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"\Resource/Loading.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
								D3DX_FILTER_NONE, 0, 0, 0, 0, &loading.texture);
	loading.bVisible = TRUE;
	loading.center = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0 };
	loading.position = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0 };
	loading.rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	loading.color = { 1, 1, 1, 1 };

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"\Resource/Title.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
								D3DX_FILTER_NONE, 0, 0, 0, 0, &title.texture);
	title.bVisible = TRUE;
	title.center = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0 };
	title.position = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0 };
	title.rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	title.color = { 1, 1, 1, 1 };

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"\Resource/Result.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, 0, 0, 0, 0, &result.texture);

	result.bVisible = FALSE;
	result.center = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0 };
	result.position = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0 };
	result.rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	result.color = { 1, 1, 1, 1 };

	



	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"\Resource/Fighter 1.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, 0, 0, 0, 0, &player.image.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"\Resource/Alien 1.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, 0, 0, 0, 0, &enemy1.texture);
	enemy1.bVisible = FALSE;
	enemy1.rect = { 0, 0, 40, 70 };
	enemy1.color = { 1,1,1,1 };
	enemy1.center = { 40 / 2, 70 / 2, 0 };

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"\Resource/Alien 4.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, 0, 0, 0, 0, &enemy2.texture);
	enemy2.bVisible = FALSE;
	enemy2.rect = { 0, 0, 74, 45 };
	enemy2.color = { 1,1,1,1 };
	enemy2.center = { 74 / 2, 45 / 2, 0 };


	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"\Resource/M484BulletCollection1.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, 0, 0, 0, 0, &bullet[0][0].image.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"\Resource/Stage1Sub.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, 0, 0, 0, 0, &stage1Sub.image.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"\Resource/Stage1Boss.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, 0, 0, 0, 0, &stage1Boss.image.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"\Resource/AngelShieldEffect_10.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, 0, 0, 0, 0, &shield.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"\Resource/Shield Icon2.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, 0, 0, 0, 0, &shieldCoolImage.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"\Resource/HPBar.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, 0, 0, 0, 0, &HPBar.texture);

	D3DXCreateTextureFromFileEx(g_pd3dDevice, L"\Resource/Effect.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, 0, 0, 0, 0, &effect.texture);



	bossBullet[0][0].image.texture = bullet[0][0].image.texture;

	ScoreInit();
	SoundInit();
}
VOID GameRelease()
{
	if (loading.texture != NULL)
		loading.texture->Release();

	if (title.texture != NULL)
		title.texture->Release();

	if (result.texture != NULL)
		result.texture->Release();

	if (enemy1.texture != NULL)
		enemy1.texture->Release();

	if (enemy2.texture != NULL)
		enemy2.texture->Release();

	BackgroundRelease();
	PlayerRelease();
	BulletRelease();
	EnemyRelease();
	ScoreRelease();
	BossRelease();
	UIRelease();
	SoundRelease();
}
VOID GameUpdate()
{
	dwCurTime = GetTickCount();
	dwElapsedTime = dwCurTime - dwOldTime;
	dwOldTime = dwCurTime;

	switch (g_GameState)
	{
	case LOADING:
		if (g_nStage == 0)
		{
			if (dwCurTime - loadTimer > 3000)
			{
				g_nStage = 1;
				loadTimer = dwCurTime;
			}
		}
		else
		{
			if (g_nStage == 1)
			{
				if (dwCurTime - loadTimer > 2000)
				{
					loadTimer = dwCurTime;

					//РЬДж

					PlayerInit();
					BulletInit();
					EnemyInit();
					BackgroundInit();
					UIInit();
					BossInit();
					//

					g_GameState = READY;
				}
			}
			if (g_nStage == 2)
			{
				if (dwCurTime - loadTimer > 2000)
				{
					loadTimer = dwCurTime;

					//РЬДж
					PlayerInit();
					BulletInit();
					EnemyInit();
					BackgroundInit();
					UIInit();
					BossInit();

					g_GameState = STAGE2;
				}
			}
		}
		break;
	case READY:
		if (GetAsyncKeyState(VK_SPACE))
		{
			dwStartTime = GetTickCount();
			g_GameState = STAGE1;
		}
		break;
	case STAGE1:
		BackgroundUpdate();
		PlayerUpdate();
		EnemyUpdate();
		BulletUpdate();
		ScoreUpdate();
		BossUpdate();
		UIUpdate();
		SoundUpdate();
		break;
	case STAGE2:
		break;
	default:
		break;
	}
}
VOID GameRender()
{
	switch (g_GameState)
	{
	case LOADING:
		g_pSprite->Draw(loading.texture, &loading.rect, &loading.center, &loading.position, loading.color);
		break;
	case READY:
		g_pSprite->Draw(title.texture, &title.rect, &title.center, &title.position, title.color);
		break;
	case STAGE1:
		BackgroundRender();
		BulletRender();
		EnemyRender();
		PlayerRender();
		BossRender();
		UIRender();
		break;
	case STAGE2:
		break;
	default:
		break;
	}
}
