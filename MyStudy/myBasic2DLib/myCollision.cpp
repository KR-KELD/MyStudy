#include "myCollision.h"

bool myCollision::PtInRectWH(RECT rt, POINT pos)
{
	if (pos.x <= rt.left + rt.right && pos.x >= rt.left&&
		pos.y <= rt.top + rt.bottom && pos.y >= rt.top)
		return true;
	else
		return false;
}

bool myCollision::InterSectRectWH(RECT rt1, RECT rt2)
{
	if ((rt1.left <= (rt2.right + rt2.left) &&
		rt2.left <= (rt1.right + rt1.left)) &&
		(rt1.top <= (rt2.top + rt2.bottom) &&
		rt2.top <= (rt1.top + rt1.bottom)))
		return true;
	else
		return false;
}

bool myCollision::PtInRect(RECT rt, POINT pos)
{
	if (pos.x <= rt.right && pos.x >= rt.left&&
		pos.y <= rt.bottom && pos.y >= rt.top)
		return true;
	else
		return false;
}

bool myCollision::PtInRectWH(RECT rt, myPoint pos)
{
	if (pos.x <= rt.left + rt.right && pos.x >= rt.left&&
		pos.y <= rt.top + rt.bottom && pos.y >= rt.top)
		return true;
	else
		return false;
}

bool myCollision::PtInRect(RECT rt, myPoint pos)
{
	if (pos.x <= rt.right && pos.x >= rt.left&&
		pos.y <= rt.bottom && pos.y >= rt.top)
		return true;
	else
		return false;
}

bool myCollision::InterSectRect(RECT rt1, RECT rt2)
{
	if (rt1.left <= rt2.right && rt2.left <= rt1.right &&
		rt1.bottom <= rt2.top && rt2.bottom <= rt1.top)
		return true;
	else
		return false;
}

bool myCollision::InterSectCircle(myPoint pt1, float r1, myPoint pt2, float r2)
{
	myPoint pt = pt2 - pt1;
	float dist = pt.Length();
	if (dist < (r1 + r2))
		return true;
	else
		return false;
}

bool myCollision::CollisionCircle(myPoint& ptMove, myPoint& ptFix, float fDist)
{
	myPoint pt = ptMove - ptFix;
	ptMove = ptFix + pt.Normalize() * fDist;
	return true;
}

