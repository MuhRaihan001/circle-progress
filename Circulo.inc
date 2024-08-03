/*

    SCRIPT UPDATED BY TsumuX
    Reduce CreateTextDraw to prevent lags
    Using Iterator to replace some useless loop
    Add ShowPlayerCircleProgress and HideCircleProgressForPlayer

*/
#define MAX_CIRCLES_DRAW                50
#define DEFAULT_CIRCLE_POLYGONS         15.0
#define INVALID_DRAW_ID                 -1

#define MAX_POINTS                      120
#define MAX_ANGLE                       360.0
#define MIN_PROGRESS_VALUE              0
#define MAX_PROGRESS_VALUE              100
#define DEFAULT_COLOR                   0xFF0000FF
#define DEFAULT_BACKGROUND_COLOR        0x000000FF
#define DEFAULT_SIZE                    10.0
#define DEFAULT_THICKNESS               0.2

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
new Iterator:circleIDS<MAX_CIRCLES_DRAW>;

forward UpdatePlayerCircleProgress(playerid, drawId, value);

stock CalculateTextDrawPosition(Float:angle, Float:distance, Float:sx, Float:sy, &Float:x, &Float:y)
{
    x = sx + (distance * floatsin(-angle, degrees));
    y = sy + (distance * floatcos(angle, degrees));
    return true;
}

stock DestroyAllPlayerCircleProgress(playerid)
{
    for (new i = 0; i < MAX_CIRCLES_DRAW; i++)
    {
        DestroyPlayerCircleProgress(playerid, i);
        Iter_Remove(circleIDS, i);
    }
    return true;
}

stock IsValidCircle(playerid, drawid)
{
    return CircleData[playerid][drawid][VALID_CIRCLE];
}

stock DestroyPlayerCircleProgress(playerid, drawId)
{
    if (drawId >= MAX_CIRCLES_DRAW) return 0;
    if (!IsValidCircle(playerid, drawId)) return 0;

    for (new i = 0; i < MAX_POINTS; i++)
    {
        if (CircleData[playerid][drawId][CircleData_Points][i] != PlayerText:INVALID_TEXT_DRAW)
        {
            PlayerTextDrawDestroy(playerid, CircleData[playerid][drawId][CircleData_Points][i]);
            CircleData[playerid][drawId][CircleData_Points][i] = PlayerText:INVALID_TEXT_DRAW;
            Iter_Remove(circleIDS, drawId);
        }
    }

    new EmptyCircleData[E_CIRCLE_STRUCT];
    CircleData[playerid][drawId] = EmptyCircleData;
    return true;
}

stock CreatePlayerCircleProgress(playerid, Float:pos_x, Float:pos_y, color = DEFAULT_COLOR, background_color = DEFAULT_BACKGROUND_COLOR, Float:size = DEFAULT_SIZE, Float:thickness = DEFAULT_THICKNESS, Float:polygons = DEFAULT_CIRCLE_POLYGONS)
{
    new drawId = Iter_Free(circleIDS);
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
        PlayerTextDrawColor(playerid, CircleData[playerid][drawId][CircleData_Points][count], background_color);

        CircleData[playerid][drawId][CREATED_CIRCLE][count] = true;
        count++;
    }
    Iter_Add(circleIDS, drawId);
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
            PlayerTextDrawHide(playerid, CircleData[playerid][drawId][CircleData_Points][i]);
            PlayerTextDrawShow(playerid, CircleData[playerid][drawId][CircleData_Points][i]);
        }
    }
    return 1;
}
