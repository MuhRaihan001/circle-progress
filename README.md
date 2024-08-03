# 🚀 Circle Progress
The project is an improved "model" of [freesampscripts](https://github.com/freesampscripts/circle-speedo)

# ⚙️ New Natives
```c++
native CreatePlayerCircleProgress(playerid, Float:pos_x, Float:pos_y, color = 0xFFFFFFFF, background_color = 0x181818FF, Float:size = 10.0, Float:thickness = 0.2, Float:polygons = DEFAULT_CIRCLE_POLYGONS);
native UpdatePlayerCircleProgress(playerid, drawId, value);
native DestroyPlayerCircleProgress(playerid, drawId);
native DestroyPlayerCircleProgressAll(playerid);
native ShowPlayerCircleProgress(playerid, drawId);
native HideCircleProgressForPlayer(playerid, drawId);
```

> [!IMPORTANT]
> About params:
> - **Thickness:** Circle line size
> - **Polygons:** Number of points to form a perfect circle, the larger it is, the more defined it will be, but it will use more textdraw resources, the limit is 256
> - **Size:** Circle size (match Polygons)
> - **drawId:** ID returned by ***CreatePlayerCircleProgress*** with the circle ID

# 📝 Example use to make circle speedometer
```c++
#include "circle.inc"
forward UpdateCircleProgress(playerid);
new speed_circlep[MAX_PLAYERS];
new UpdateCircle[MAX_PLAYERS];

public OnPlayerConnect(playerid)
{
  speed_circle[playerid] = CreatePlayerCircleProgress(playerid, 130.00, 373.30, 0x0388FCFF, 774778623, 10.00, 0.30, 3.0);

  return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
  if(newstate == PLAYER_STATE_DRIVER)
  {
    ShowPlayerCircleProgress(playerid, speed_circle[playerid]);
    UpdateCircle[playerid] = SetTimerEx("UpdateCircleProgress", 200, true, "i", playerid);
  }

  if(oldstate == PLAYER_STATE_DRIVER)
  {
    HideCircleProgressForPlayer(playerid, speed_circle[playerid]);
    KillTimer(UpdateCircle[playerid]);
  }
  return 1;
}

public UpdateCircleProgress(playerid)
{
  new vehicleid = GetPlayerVehicleID(playerid);
  if (vehicleid != INVALID_VEHICLE_ID)
  {
			new Float:speed;
			GetVehicleVelocity(vehicleid, speed, speed, speed);
			speed = GetPlayerSpeed(playerid);

			new drawId = speed_circle[playerid];
			new value = floatround((speed / 100.0) * 100.0);
			UpdatePlayerCircleProgress(playerid, drawId, value);
			UpdatePlayerCircleProgress(playerid, fuel_circle[playerid], floatround(GetVehicleData(vehicleid, V_FUEL)));
  }
  return 1;
}

```

# 🌐 What are the changes?
- Circles are created individually, allowing up to 40 progress indicators
- Added new native functions
- Reduce textdraw ussage in UpdatePlaterCircleProgress
- Add  ShowPlayerCircleProgress and HideCircleProgressForPlayer

# 📝 Credits
- freesampscripts - Create source code
- Diogo "blueN" - Recreate code with new natives and update functions
- TsumuX  -  Fix some bug and reduce the lags due to high amount of textdraw created

# Preview
![](https://github.com/MuhRaihan001/circle-progress/blob/main/sa-mp-017.png)
