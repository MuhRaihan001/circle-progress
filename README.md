Here is an updated version of your `readme.md` to match the latest code updates, along with detailed explanations, especially regarding the new **Max Value** feature.

---

# 🚀 Circle Progress

This project is an enhanced version of the [freesampscripts circle-speedo](https://github.com/freesampscripts/circle-speedo) system, featuring optimized circle progress indicators, new functionalities, and reduced lag due to minimized textdraw resources.

# ⚙️ New Natives
```c++
native CreatePlayerCircleProgress(playerid, Float:pos_x, Float:pos_y, color = 0xFFFFFFFF, background_color = 0x181818FF, Float:size = 10.0, Float:thickness = 0.2, Float:polygons = DEFAULT_CIRCLE_POLYGONS, max = MAX_PROGRESS_VALUE);
native UpdatePlayerCircleProgress(playerid, drawId, value);
native DestroyPlayerCircleProgress(playerid, drawId);
native DestroyPlayerCircleProgressAll(playerid);
native ShowPlayerCircleProgress(playerid, drawId);
native HideCircleProgressForPlayer(playerid, drawId);
native SetCircleProgressMaxValue(playerid, drawId, value)
```

> [!IMPORTANT]
> About parameters:
> - **Thickness:** The line thickness of the circle.
> - **Polygons:** Number of points to form the circle. A higher number results in a smoother circle but requires more textdraw resources. The default is `DEFAULT_CIRCLE_POLYGONS` with a maximum of 256.
> - **Size:** The size of the circle, adjusted to match the number of polygons for smoothness.
> - **Max Value:** The maximum value of progress, used to determine the limit of progress updates. By default, it's set to `MAX_PROGRESS_VALUE` (e.g., 100), but this can be adjusted to represent different scales.
> - **drawId:** ID returned by `CreatePlayerCircleProgress`, representing the specific circle's unique ID.

# 🌐 What’s New?

- **Individual Circle Instances**: Now supports up to 50 individual progress indicators (modifiable through `MAX_CIRCLES_DRAW`), giving greater flexibility for complex UI setups.
- **Max Value**: Each circle can be assigned a unique maximum value to define its progress scale. This feature allows developers to adjust the scale without having to alter individual progress values directly.
- **Optimized Update Mechanism**: Reduced textdraw resource usage when updating circles by only refreshing the necessary parts, decreasing potential lag.
- **Enhanced Circle Control**: Additional functions to show or hide specific circles without destroying them, improving UI management.

# 📝 Example Usage: Circle Speedometer

This example demonstrates how to create a speedometer using `CreatePlayerCircleProgress` and continuously update it based on player speed.

```c++
#include "circle.inc"
forward UpdateCircleProgress(playerid);
new speed_circle[MAX_PLAYERS];
new UpdateCircle[MAX_PLAYERS];

public OnPlayerConnect(playerid)
{
  // Create a circle to display speed
  speed_circle[playerid] = CreatePlayerCircleProgress(playerid, 130.00, 373.30, 0x0388FCFF, 0x181818FF, 10.00, 0.30, 3.0, 100);
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
    new value = floatround((speed / 100.0) * 100.0); // Scale speed as percentage
    UpdatePlayerCircleProgress(playerid, drawId, value);
  }
  return 1;
}
```

### Explanation of the `max` parameter in `CreatePlayerCircleProgress`

The `max` parameter defines the upper limit for the circle's progress scale. For example, if `max` is set to `100`, then setting the progress value to `50` will fill the circle to 50%. This flexibility allows you to adjust the scale of progress indicators, such as setting `max` to `100` for percentage-based indicators or another value based on a specific metric (like fuel level, health points, etc.).

### Code Improvements and Definitions

The latest code introduces several new definitions and validation checks:

- **MAX_CIRCLES_DRAW**: Maximum circles each player can have. Set to 50 by default, adjustable up to 100.
- **DEFAULT_CIRCLE_POLYGONS**: Minimum value for polygons is set to `3.0` to ensure the circle shape is correctly rendered.
- **MAX_POINTS**: Maximum points in each circle, defined to 120 by default to prevent overuse of resources.
- **DEFAULT_SIZE and DEFAULT_THICKNESS**: Ensures a minimum size and thickness to avoid rendering issues.

# 📝 Credits
- **freesampscripts** - Created the original source code.
- **Diogo "blueN"** - Recreated the code with new natives and updated functions.
- **TsumuX** - Fixed bugs and reduced lag caused by high textdraw usage.

# Preview

![Preview](https://github.com/MuhRaihan001/circle-progress/blob/main/sa-mp-017.png)

--- 

This updated documentation should now match your latest code, including details on the `max` parameter and optimizations made to reduce lag. Let me know if there are additional aspects you'd like to clarify!
