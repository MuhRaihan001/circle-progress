#if defined _inc_circle
	#undef _inc_circle
#endif

#if defined _circle_included
	#endinput
#endif

#if defined MAX_CIRCLES_DRAW
	#if MAX_CIRCLES_DRAW <= 0
		#error You are trying to allocate invalid number of circles. Set value of MAX_CIRCLES_DRAW greater than 0.
	#endif
	#if MAX_CIRCLES_DRAW > 100
		#warning You are trying to allocate too many circles. Set value of MAX_CIRCLES_DRAW below or equal to 100.
	#endif
#else
	#define MAX_CIRCLES_DRAW 50
#endif

#if defined DEFAULT_CIRCLE_POLYGONS
	#if DEFAULT_CIRCLE_POLYGONS < 3.0
		#error You are trying to allocate too few polygons for a circle. Set value of DEFAULT_CIRCLE_POLYGONS greater than or equal to 3.0.
	#endif
#else
	#define DEFAULT_CIRCLE_POLYGONS 15.0
#endif

#if !defined INVALID_DRAW_ID
	#define INVALID_DRAW_ID -1
#endif

#if defined MAX_POINTS
	#if MAX_POINTS <= 0
		#error You are trying to allocate invalid number of points. Set value of MAX_POINTS greater than 0.
	#endif
	#if MAX_POINTS > 120
		#error You are trying to allocate too many points. Set value of MAX_POINTS below or equal to 500.
	#endif
#else
	#define MAX_POINTS 120
#endif

#if !defined MAX_ANGLE
	#define MAX_ANGLE 360.0
#endif

#if !defined MIN_PROGRESS_VALUE
	#define MIN_PROGRESS_VALUE 0
#endif

#if defined MAX_PROGRESS_VALUE
	#if MAX_PROGRESS_VALUE <= MIN_PROGRESS_VALUE
		#error MAX_PROGRESS_VALUE should be greater than MIN_PROGRESS_VALUE.
	#endif
#else
	#define MAX_PROGRESS_VALUE 100
#endif

#if !defined DEFAULT_COLOR
	#define DEFAULT_COLOR 0xFF0000FF
#endif

#if !defined DEFAULT_BACKGROUND_COLOR
	#define DEFAULT_BACKGROUND_COLOR 0x000000FF
#endif

#if defined DEFAULT_SIZE
	#if DEFAULT_SIZE <= 0.0
		#error You are trying to allocate invalid size. Set value of DEFAULT_SIZE greater than 0.0.
	#endif
#else
	#define DEFAULT_SIZE 10.0
#endif

#if defined DEFAULT_THICKNESS
	#if DEFAULT_THICKNESS <= 0.0
		#error You are trying to allocate invalid thickness. Set value of DEFAULT_THICKNESS greater than 0.0.
	#endif
#else
	#define DEFAULT_THICKNESS 0.2
#endif


enum E_CIRCLE_STRUCT
{
    CIRCLE_COLOR,
    CIRCLE_BACKGROUND,

    bool:CREATED_CIRCLE[MAX_POINTS + 1],
    bool:VALID_CIRCLE,
    bool:CIRCLE_UPDATE,

    Float:CIRCLE_POS[2],
    Float:CIRCLE_THICKNESS,
    Float:CIRCLE_SIZE,
    Float:CIRCLE_VALUE,
    Float:CIRCLE_POLYGONS,

    PlayerText:CircleData_Points[MAX_POINTS + 1]
}

new CircleData[MAX_PLAYERS][MAX_CIRCLES_DRAW][E_CIRCLE_STRUCT];
new Iterator:circleIDS[MAX_PLAYERS]<MAX_CIRCLES_DRAW>;

forward UpdatePlayerCircleProgress(playerid, drawId, value);

public OnPlayerDisconnect(playerid, reason)
{

	DestroyAllPlayerCircleProgress(playerid);

	new ret = 1;
	#if defined MainCircle_OnPlayerDisconnect
		ret = MainCircle_OnPlayerDisconnect(playerid, reason);
	#endif
	return ret;
}

public OnGameModeInit()
{
	Iter_Init(circleIDS);
    printf("[TsumuX] Loaded all circle progress");

	new ret = 1;
	#if defined MainCircle_OnGameModeInit
		ret = MainCircle_OnGameModeInit();
	#endif
	return ret;
}

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit MainCircle_OnGameModeInit
#if defined MainCircle_OnGameModeInit
	forward MainCircle_OnGameModeInit();
#endif


#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect MainCircle_OnPlayerDisconnect
#if defined MainCircle_OnPlayerDisconnect
	forward MainCircle_OnPlayerDisconnect(playerid, reason);
#endif

stock CalculateTextDrawPosition(Float:angle, Float:distance, Float:sx, Float:sy, &Float:x, &Float:y)
{
    x = sx + (distance * floatsin(-angle, degrees));
    y = sy + (distance * floatcos(angle, degrees));
    return true;
}


stock IsValidCircle(playerid, drawid)
{
    return CircleData[playerid][drawid][VALID_CIRCLE];
}

stock DestroyAllPlayerCircleProgress(playerid)
{
    new i, validCircles = 0;
    for (i = 0; i < MAX_CIRCLES_DRAW; i++)
    {
        if (IsValidCircle(playerid, i))
        {
            DestroyPlayerCircleProgress(playerid, i);
            Iter_Remove(circleIDS[playerid], i);
            validCircles++;
        }
    }
    printf("[INFO] Destroyed %d valid circles for player %d", validCircles, playerid);
    return 1;
}

stock DestroyPlayerCircleProgress(playerid, drawId)
{
    if (drawId >= MAX_CIRCLES_DRAW) return 0;
    if (!IsValidCircle(playerid, drawId)) return 0;

    new i, validPoints = 0;
    for (i = 0; i < MAX_POINTS; i++)
    {
        if (CircleData[playerid][drawId][CircleData_Points][i] != PlayerText:INVALID_TEXT_DRAW)
        {
            PlayerTextDrawDestroy(playerid, CircleData[playerid][drawId][CircleData_Points][i]);
            CircleData[playerid][drawId][CircleData_Points][i] = PlayerText:INVALID_TEXT_DRAW;
            Iter_Remove(circleIDS[playerid], drawId);
            validPoints++;
        }
    }
    printf("[INFO] Destroyed %d valid points for circle %d of player %d", validPoints, drawId, playerid);

    new EmptyCircleData[E_CIRCLE_STRUCT];
    CircleData[playerid][drawId] = EmptyCircleData;
    return true;
}


stock CreatePlayerCircleProgress(playerid, Float:pos_x, Float:pos_y, color = DEFAULT_COLOR, background_color = DEFAULT_BACKGROUND_COLOR, Float:size = DEFAULT_SIZE, Float:thickness = DEFAULT_THICKNESS, Float:polygons = DEFAULT_CIRCLE_POLYGONS)
{
    new drawId = Iter_Free(circleIDS[playerid]);
    if (IsValidCircle(playerid, drawId))
    {
        SendClientMessage(playerid, -1, "Error: Circle reached the limit");
        return INVALID_DRAW_ID;
    }

    CircleData[playerid][drawId][VALID_CIRCLE] = true;
    CircleData[playerid][drawId][CIRCLE_THICKNESS] = thickness;
    CircleData[playerid][drawId][CIRCLE_COLOR] = color;
    CircleData[playerid][drawId][CIRCLE_BACKGROUND] = background_color;
    CircleData[playerid][drawId][CIRCLE_SIZE] = size;
    CircleData[playerid][drawId][CIRCLE_POS][0] = pos_x;
    CircleData[playerid][drawId][CIRCLE_POS][1] = pos_y;
    CircleData[playerid][drawId][CIRCLE_VALUE] = -1.0;
    CircleData[playerid][drawId][CIRCLE_POLYGONS] = polygons;

    new Float:x, Float:y, count = 0, Float:angleStep = CircleData[playerid][drawId][CIRCLE_POLYGONS];
    for (new Float:angle = 0.0; angle < MAX_ANGLE; angle += angleStep)
    {
        CalculateTextDrawPosition(angle + 180.0, CircleData[playerid][drawId][CIRCLE_SIZE], CircleData[playerid][drawId][CIRCLE_POS][0], CircleData[playerid][drawId][CIRCLE_POS][1], x, y);

        CircleData[playerid][drawId][CircleData_Points][count] = CreatePlayerTextDraw(playerid, x, y, ".");
        PlayerTextDrawAlignment(playerid, CircleData[playerid][drawId][CircleData_Points][count], 2);
        PlayerTextDrawLetterSize(playerid, CircleData[playerid][drawId][CircleData_Points][count], thickness, thickness * 2);
        PlayerTextDrawSetShadow(playerid, CircleData[playerid][drawId][CircleData_Points][count], 0);
        PlayerTextDrawSetOutline(playerid, CircleData[playerid][drawId][CircleData_Points][count], 0);
        PlayerTextDrawColor(playerid, CircleData[playerid][drawId][CircleData_Points][count], background_color);

        CircleData[playerid][drawId][CREATED_CIRCLE][count] = true;
        count++;
    }
    Iter_Add(circleIDS[playerid], drawId);
    return drawId;
}

public UpdatePlayerCircleProgress(playerid, drawId, value)
{
    if (drawId < 0 || drawId >= MAX_CIRCLES_DRAW) return 0;
    if (CircleData[playerid][drawId][CIRCLE_VALUE] == value) return 0;

    value = clamp(value, MIN_PROGRESS_VALUE, MAX_PROGRESS_VALUE);

    new Float:x, Float:y, percent, count = 0, Float:angleStep = CircleData[playerid][drawId][CIRCLE_POLYGONS], preDraws = 0;
    for (new Float:angle = 0.0; angle < MAX_ANGLE; angle += angleStep) preDraws++;

    percent = (preDraws * value) / 100;

    for (new Float:angle = 0.0; angle < MAX_ANGLE; angle += angleStep)
    {
        CalculateTextDrawPosition(angle + 180.0, CircleData[playerid][drawId][CIRCLE_SIZE], CircleData[playerid][drawId][CIRCLE_POS][0], CircleData[playerid][drawId][CIRCLE_POS][1], x, y);

        PlayerTextDrawColor(playerid, CircleData[playerid][drawId][CircleData_Points][count], percent <= count ? CircleData[playerid][drawId][CIRCLE_BACKGROUND] : CircleData[playerid][drawId][CIRCLE_COLOR]);
        count++;
    }

    CircleData[playerid][drawId][CIRCLE_UPDATE] = true;
    CircleData[playerid][drawId][CIRCLE_VALUE] = value;

    ShowPlayerCircleProgress(playerid, drawId);

    return 1;
}

stock HideCircleProgressForPlayer(playerid, drawId)
{
    if (!IsValidCircle(playerid, drawId)) return 0;
    for (new i = 0; i < MAX_POINTS; i++)
    {
        if (CircleData[playerid][drawId][CREATED_CIRCLE][i])
        {
            PlayerTextDrawHide(playerid, CircleData[playerid][drawId][CircleData_Points][i]);
        }
    }
    return 1;
}

stock ShowPlayerCircleProgress(playerid, drawId)
{
    if (!IsValidCircle(playerid, drawId)) return 0;
    for (new i = 0; i < MAX_POINTS; i++)
    {
        if (CircleData[playerid][drawId][CREATED_CIRCLE][i])
        {
            PlayerTextDrawShow(playerid, CircleData[playerid][drawId][CircleData_Points][i]);
        }
    }
    return 1;
}
