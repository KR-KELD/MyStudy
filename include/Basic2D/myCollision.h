#pragma once
#include "myStd.h"
class myCollision
{
public:
	static bool		PtInRectWH(RECT rt, POINT pos);
	static bool		InterSectRectWH(RECT rt1, RECT rt2);
	static bool		PtInRect(RECT rt, POINT pos);
	static bool		PtInRectWH(RECT rt, myPoint pos);
	static bool		PtInRect(RECT rt, myPoint pos);
	static bool		InterSectRect(RECT rt1, RECT rt2);
	static bool		InterSectCircle(myPoint pt1, float r1, myPoint pt2, float r2);
	static bool		CollisionCircle(myPoint& ptMove, myPoint& ptFix, float fDist);
};


