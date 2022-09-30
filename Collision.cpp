#include "Collision.h"

BOOL Collision(D3DXVECTOR3 p1, FLOAT d1, D3DXVECTOR3 p2, FLOAT d2)
{
	FLOAT dist = sqrtf(((p2.x - p1.x) * (p2.x - p1.x)) + ((p2.y - p1.y) * (p2.y - p1.y)));

	if (dist <= d1 + d2)
		return TRUE;
	else
		return FALSE;
}