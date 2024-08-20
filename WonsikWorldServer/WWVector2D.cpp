#include "WWVector2D.h"
#include <utility>
void WWVector2D::Normalize()
{
	if (_x == 0 && _y == 0)
	{
		return;
	}
	float divideVal = sqrt(pow(_x, 2) + pow(_y, 2));
	_x = _x / divideVal;
	_y = _y / divideVal;	
}

float WWVector2D::Length()
{
	return  sqrt(pow(_x, 2) + pow(_y, 2));
}
std::pair<float, float> WWVector2D::TransferToPair()
{
	return std::pair<float, float>(_x, _y);
}


CSendBuffer& operator<<(CSendBuffer& buf, const WWVector2D& vec)
{
	buf << vec._x << vec._y;
	return buf;
}

CRecvBuffer& operator>>(CRecvBuffer& buf, WWVector2D& vec)
{
	buf >> vec._x >> vec._y;
	return buf;
}

CSendBuffer& operator<<(CSendBuffer& buf, const Vector<WWVector2D>& vectors)
{
	USHORT vecCnt = vectors.size();
	buf << vecCnt;
	for (int i = 0; i < vecCnt; i++)
	{
		buf << vectors[i];
	}
	return buf;
}

CRecvBuffer& operator>>(CRecvBuffer& buf, Vector<WWVector2D>& vectors)
{
	USHORT vecCnt;
	buf >> vecCnt;
	vectors.resize(vecCnt);
	for (int i = 0; i < vecCnt; i++)
	{
		buf >> vectors[i];
	}
	return buf;
}

WWVector2D operator-(const WWVector2D& vec1, const WWVector2D& vec2)
{
	return WWVector2D(vec1._x - vec2._x, vec1._y - vec2._y);
}