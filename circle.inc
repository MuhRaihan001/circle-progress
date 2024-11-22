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
	#define DEFAULT_CIRCLE_POLYGONS 3.0
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

#if !defined MAX_PROGRESS_VALUE
	#define MAX_PROGRESS_VALUE 100
#endif

#if defined MAX_PROGRESS_VALUE
    #if MAX_PROGRESS_VALUE < 5
        #error Max value should be higher than 5
    #endif
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
    CIRCLE_MAX_VALUE,
    CIRCLE_MAX_COLOR,

    bool:CREATED_CIRCLE[MAX_POINTS + 1],
    bool:VALID_CIRCLE,
    bool:CIRCLE_UPDATE,
    bool: CIRCLE_GRADIENT,

    Float:CIRCLE_POS[2],
    Float:CIRCLE_THICKNESS,
    Float:CIRCLE_SIZE,
    Float:CIRCLE_VALUE,
    Float:CIRCLE_POLYGONS,

    PlayerText:CircleData_Points[MAX_POINTS + 1]
}

new CircleData[MAX_PLAYERS][MAX_CIRCLES_DRAW][E_CIRCLE_STRUCT];
new Iterator:circleIDS[MAX_PLAYERS]<MAX_CIRCLES_DRAW>;

forward SetPlayerCircleProgressValue(playerid, drawId, value);

stock CalculateTextDrawPosition(Float:angle, Float:distance, Float:sx, Float:sy, &Float:x, &Float:y)
{
    x = sx + (distance * floatsin(-angle, degrees));
    y = sy + (distance * floatcos(angle, degrees));
    return true;
}

stock InterpolateColor(colorStart, colorEnd, Float: progress)
{
    new r1 = (colorStart >> 24) & 0xFF, g1 = (colorStart >> 16) & 0xFF, b1 = (colorStart >> 8) & 0xFF, a1 = colorStart & 0xFF;
    new r2 = (colorEnd >> 24) & 0xFF, g2 = (colorEnd >> 16) & 0xFF, b2 = (colorEnd >> 8) & 0xFF, a2 = colorEnd & 0xFF;

    new r = floatround(r1 + (r2 - r1) * progress);
    new g = floatround(g1 + (g2 - g1) * progress);
    new b = floatround(b1 + (b2 - b1) * progress);
    new a = floatround(a1 + (a2 - a1) * progress);

    return (r << 24) | (g << 16) | (b << 8) | a;
}

stock GetCircleProgressValue(playerid, drawId)
{
    new Float: value = CircleData[playerid][drawId][CIRCLE_VALUE];
    return floatround(value);
}

stock DestroyAllPlayerCircleProgress(playerid)
{
    for (new i = 0; i < MAX_CIRCLES_DRAW; i++)
    {
        if (CircleData[playerid][i][VALID_CIRCLE])
        {
            DestroyPlayerCircleProgress(playerid, i);
            Iter_Clear(circleIDS[playerid]);
        }
    }
    return 1;
}

stock DestroyPlayerCircleProgress(playerid, drawId)
{
    if (drawId >= MAX_CIRCLES_DRAW) return 0;
    if (!CircleData[playerid][drawId][VALID_CIRCLE]) return 0;

    for (new i = 0; i < MAX_POINTS; i++)
    {
        if (CircleData[playerid][drawId][CircleData_Points][i] != PlayerText:INVALID_TEXT_DRAW)
        {
            PlayerTextDrawDestroy(playerid, CircleData[playerid][drawId][CircleData_Points][i]);
            CircleData[playerid][drawId][CircleData_Points][i] = PlayerText:INVALID_TEXT_DRAW;
            Iter_Remove(circleIDS[playerid], drawId);
        }
    }
    #if defined DEBUG_CIRCLE
        printf("[CIRCLE-PROGRESS] Destroyed player circle progress for player %d and circleid: %d.", playerid, drawId);
    #endif

    return true;
}

stock SetCircleProgressMaxValue(playerid, drawId, value)
{
    if(value == 0 || value < 0) return 0;
    CircleData[playerid][drawId][CIRCLE_MAX_VALUE] = value;
    #if defined DEBUG_CIRCLE
        printf("[CIRCLE-PROGRESS] Set circle progress max value for player %d to %d for circleid: %d.", playerid, value, drawId);
    #endif
    return 1; 
}

stock SetCircleProgressColor(playerid, drawId, color)
{
    if(drawId == INVALID_DRAW_ID) return 0;
    CircleData[playerid][drawId][CIRCLE_COLOR] = color;
    #if defined DEBUG_CIRCLE
        printf("[CIRCLE-PROGRESS] Set circle progress color for player %d to %d for circleid: %d.", playerid, color, drawId);
    #endif
    return 1;
}

stock SetCircleBackgroundColor(playerid, drawId, color)
{
    if(drawId == INVALID_DRAW_ID) return 0;
    CircleData[playerid][drawId][CIRCLE_BACKGROUND] = color;
    #if defined DEBUG_CIRCLE
        printf("[CIRCLE-PROGRESS] Set circle progress background color for player %d to %d for circleid: %d", playerid, color, drawId);
    #endif
    return 1;
}

stock CircleGradient(playerid, drawId, bool: allow)
{
    if(drawId == INVALID_DRAW_ID) return 0;
    CircleData[playerid][drawId][CIRCLE_GRADIENT] = allow;
    #if defined DEBUG_CIRCLE
        printf("[CIRCLE-PROGRESS] Set circle progress color changing for player %d to %s for circleid: %d", playerid, allow ? "On" : "Off", drawId);
    #endif
    return 1;
}

stock SetCircleMaxColor(playerid, drawId, maxcolor)
{
    if(drawId == INVALID_DRAW_ID) return 0;
    CircleData[playerid][drawId][CIRCLE_MAX_COLOR] = maxcolor;
    #if defined DEBUG_CIRCLE
        printf("[CIRCLE-PROGRESS] Set circle progress max color for player %d to %d for circleid: %d", playerid, maxcolor, drawId);
    #endif
    return 1;
}

stock CreatePlayerCircleProgress(playerid, Float:pos_x, Float:pos_y, Float:size = DEFAULT_SIZE, Float:thickness = DEFAULT_THICKNESS, Float:polygons = DEFAULT_CIRCLE_POLYGONS)
{
    new drawId = Iter_Free(circleIDS[playerid]), Float:x, Float:y, count = 0;
    if (drawId < 0 || drawId >= MAX_CIRCLES_DRAW)
        return 0;

    CircleData[playerid][drawId][VALID_CIRCLE] = true;
    CircleData[playerid][drawId][CIRCLE_THICKNESS] = thickness;
    CircleData[playerid][drawId][CIRCLE_SIZE] = size;
    CircleData[playerid][drawId][CIRCLE_VALUE] = -1;
    CircleData[playerid][drawId][CIRCLE_POS][0] = pos_x;
    CircleData[playerid][drawId][CIRCLE_POS][1] = pos_y;
    CircleData[playerid][drawId][CIRCLE_POLYGONS] = polygons;

    for (new Float:angle = 0.0; angle < MAX_ANGLE; angle += CircleData[playerid][drawId][CIRCLE_POLYGONS])
    {
        CalculateTextDrawPosition(angle + 180.0, CircleData[playerid][drawId][CIRCLE_SIZE], CircleData[playerid][drawId][CIRCLE_POS][0], CircleData[playerid][drawId][CIRCLE_POS][1], x, y);

        CircleData[playerid][drawId][CircleData_Points][count] = CreatePlayerTextDraw(playerid, x, y, ".");
        PlayerTextDrawAlignment(playerid, CircleData[playerid][drawId][CircleData_Points][count], 2);
        PlayerTextDrawLetterSize(playerid, CircleData[playerid][drawId][CircleData_Points][count], thickness, thickness * 4);
        PlayerTextDrawSetShadow(playerid, CircleData[playerid][drawId][CircleData_Points][count], 0);
        PlayerTextDrawSetOutline(playerid, CircleData[playerid][drawId][CircleData_Points][count], 0);

        CircleData[playerid][drawId][CREATED_CIRCLE][count] = true;
        count++;
    }
    Iter_Add(circleIDS[playerid], drawId);
    #if defined DEBUG_CIRCLE
        printf("[CIRCLE-PROGRESS] Created circle progress for player %d", playerid);
    #endif
    return drawId;
}

public SetPlayerCircleProgressValue(playerid, drawId, value)
{
    if (GetCircleProgressValue(playerid, drawId) == value) return 0;
    
    value = clamp(value, MIN_PROGRESS_VALUE, CircleData[playerid][drawId][CIRCLE_MAX_VALUE]);
    new Float:preDraws = MAX_ANGLE / CircleData[playerid][drawId][CIRCLE_POLYGONS];
    new Float:percent = (preDraws * value) / CircleData[playerid][drawId][CIRCLE_MAX_VALUE];
    new interpolatedColor = InterpolateColor(CircleData[playerid][drawId][CIRCLE_COLOR], 0x00FF00FF, percent / preDraws);
    new color = CircleData[playerid][drawId][CIRCLE_GRADIENT] ? interpolatedColor : CircleData[playerid][drawId][CIRCLE_COLOR];

    for (new count = 0; count < preDraws; count++)
        PlayerTextDrawColor(playerid, CircleData[playerid][drawId][CircleData_Points][count], percent <= count ? CircleData[playerid][drawId][CIRCLE_BACKGROUND] : color);

    CircleData[playerid][drawId][CIRCLE_UPDATE] = true;
    CircleData[playerid][drawId][CIRCLE_VALUE] = value;

    ShowPlayerCircleProgress(playerid, drawId);
    return 1;
}

stock HideCircleProgressForPlayer(playerid, drawId)
{
    for (new i = 0; i < MAX_POINTS; i++)
    {
        if (CircleData[playerid][drawId][CREATED_CIRCLE][i])
        {
            PlayerTextDrawHide(playerid, CircleData[playerid][drawId][CircleData_Points][i]);
        }
    }
    #if defined DEBUG_CIRCLE
        printf("[CIRCLE-PROGRESS] Hiding circle progress for player %d", playerid);
    #endif
    return 1;
}

stock ShowPlayerCircleProgress(playerid, drawId)
{
    for (new i = 0; i < MAX_POINTS; i++)
    {
        if (CircleData[playerid][drawId][CREATED_CIRCLE][i])
        {
            PlayerTextDrawShow(playerid, CircleData[playerid][drawId][CircleData_Points][i]);
        }
    }
    #if defined DEBUG_CIRCLE
        printf("[CIRCLE-PROGRESS] Showing circle progress for player %d", playerid);
    #endif
    return 1;
}

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
    printf("  /$$$$$$  /$$                     /$$               /$$                           /$$                                 /$$                 /$$\n");
    printf(" /$$__  $$|__/                    | $$              |__/                          | $$                                | $$                | $$\n");
    printf("| $$  \\__/ /$$  /$$$$$$   /$$$$$$$| $$  /$$$$$$      /$$ /$$$$$$$   /$$$$$$$      | $$        /$$$$$$   /$$$$$$   /$$$$$$$  /$$$$$$   /$$$$$$$\n");
    printf("| $$      | $$ /$$__  $$ /$$_____/| $$ /$$__  $$    | $$| $$__  $$ /$$_____/      | $$       /$$__  $$ |____  $$ /$$__  $$ /$$__  $$ /$$__  $$\n");
    printf("| $$      | $$| $$  \\__/| $$      | $$| $$$$$$$$    | $$| $$  \\ $$| $$            | $$      | $$  \\ $$  /$$$$$$$| $$  | $$| $$$$$$$$| $$  | $$\n");
    printf("| $$    $$| $$| $$      | $$      | $$| $$_____/    | $$| $$  | $$| $$            | $$      | $$  | $$ /$$__  $$| $$  | $$| $$_____/| $$  | $$\n");
    printf("|  $$$$$$/| $$| $$      |  $$$$$$$| $$|  $$$$$$$ /$$| $$| $$  | $$|  $$$$$$$      | $$$$$$$$|  $$$$$$/|  $$$$$$$|  $$$$$$$|  $$$$$$$|  $$$$$$$\n");
    printf(" \\______/ |__/|__/       \\_______/|__/ \\_______/|__/|__/|__/  |__/ \\_______/      |________/ \\______/  \\_______/ \\_______/ \\_______/ \\_______/\n");
    printf("                                                                                                                                              \n");
    printf("                                                                                                                                              \n");
    printf("                                                                                                                                              \n");

    #if defined DEBUG_CIRCLE
        printf("[CIRCLE-PROGRESS] Debug Mode On");
    #endif

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