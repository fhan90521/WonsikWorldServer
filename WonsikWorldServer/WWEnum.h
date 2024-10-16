#pragma once
enum MapID : short
{
	MAP_ID_LOBBY=0,
	MAP_ID_FIELD1,
	MAP_ID_FIELD2
};
enum RoomID : int
{
	ROOM_ID_LOBBY=0,
	ROOM_ID_FIELD1,
	ROOM_ID_FIELD2
};
enum EnterGameResult : short
{
	ENTER_GAME_SUCCESS,
	NAME_DUPLICATION,
};

enum GameOption : int
{
	MAP_WIDTH = 10000,
	MAP_HEIGHT = 10000,
	GRID_CELL_SIZE = 100,
	SECTOR_SIZE = 500,
	MOVE_SPEED = 100,	
	CLOSE_DISTANCE = 10,
	DEFAULT_DIRX = -1,
	DEFAULT_DIRY = 0,
	STRANGE_DISTANCE_TO_DEST = 200000,
	LAST_RECV_TIME_OUT = 50000,
	SEND_HEARTBEAT_PERIOD =30000
};