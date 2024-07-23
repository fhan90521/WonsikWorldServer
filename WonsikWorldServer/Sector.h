#pragma once
struct SECTOR_POS
{
	int x;
	int y;
};

struct SECTOR_AROUND
{
	int cnt = 0;
	SECTOR_POS around[9];
};