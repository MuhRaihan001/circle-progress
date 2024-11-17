#include <a_samp>
#include "circle.inc"


new speed_circle[MAX_PLAYERS];
new time_update[MAX_PLAYERS];

stock UpdateCircleValue(playerid)
{
    new Float: speed = GetPlayerSpeed(playerid),
        value = floatround((speed / 100.0) * 100.0);
    SetPlayerCircleProgressValue(playerid, speed_circle[playerid], value);
    return 1;
}

public OnPlayerConnect(playerid)
{
    speed_circle[playerid] = CreatePlayerCircleProgress(playerid, 138.00, 358.00, 17.00, 0.45);
    SetCircleProgressColor(playerid, speed_circle[playerid], 0x0388FCFF);
    SetCircleBackgroundColor(playerid, speed_circle[playerid], 774778623);
    SetCircleProgressMaxValue(playerid, speed_circle[playerid], 150);
    return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
    if(newstate == PLAYER_STATE_DRIVER)
    {
        ShowPlayerCircleProgress(playerid, speed_circle[playerid]);
        SetTimerEx("UpdateCircleValue", 200, true, "i", playerid);
    }
    if(oldstate == PLAYER_STATE_DRIVER)
    {
        HideCircleProgressForPlayer(playerid, speed_circle[playerid]);
        KillTimer(time_update);
    }
    return 1;
}