
# 🌀 Circle Progress

Welcome to the **Circle Progress** script! This feature lets you create smooth, customizable circle progress indicators for your players. Perfect for visualizing progress bars, loading indicators, and more! 🎮

![Preview](https://github.com/MuhRaihan001/circle-progress/blob/main/sa-mp-017.png)

## ✨ Features
- **Smooth Circles**: Render circular progress indicators with customizable size, color, and thickness. 🎨
- **Multiple Circles**: Support for up to 100 circles at once. 🔢
- **Customizable Polygons**: Define polygon density for smoothness. 🔺
- **Optimized**: Efficiently manages multiple textdraws with minimal lag. ⚡

## 🔧 Setup Instructions
1. **Define the maximum number of circles** in `MAX_CIRCLES_DRAW`.
2. **Customize the default size, thickness, and colors** to match your project. 🎨
3. Use `CreatePlayerCircleProgress` to create circles and set values with `SetPlayerCircleProgressValue`.

### ⚠️ Warnings and Errors
- **Too Many Circles**: Setting `MAX_CIRCLES_DRAW` above 100 will trigger a warning. ⚠️
- **Minimum Requirements**: Ensure `DEFAULT_CIRCLE_POLYGONS` is above 3.0, `MAX_POINTS` is at least 1, and `DEFAULT_SIZE` is greater than 0.0. 🛠️

## 📝 Code Example
```pawn
// Example to create a circle and set its progress
new circleId = CreatePlayerCircleProgress(playerid, pos_x, pos_y, color, bg_color, size, thickness);
SetPlayerCircleProgressValue(playerid, circleId, 75); // Set circle to 75%
```

## 🎉 Credits
- **Freesampscript** – Original Creator 🛠️
- **BlueN** – Added support for various circles 🔄
- **TsumuX** – Optimized flexibility and reduced lag 💨

---

### 🧰 Detailed Functions & Natives

#### 1. **`CreatePlayerCircleProgress`**
   - Creates a circular progress indicator for a player.
   - **Arguments**: Player ID, position (x, y), circle color, background color, size, thickness, polygon density.
   - **Returns**: Circle ID (a unique identifier for the circle).
   - **Explanation**: This function creates a circle for the player at a specific position, with defined attributes like size, thickness, color, and the number of polygons (smoothness). 

#### 2. **`SetPlayerCircleProgressValue`**
   - Sets the progress value of a player's circle.
   - **Arguments**: Player ID, Circle ID, progress value (between 0 and max progress).
   - **Returns**: Success (1) or failure (0).
   - **Explanation**: Updates the circle's progress value, adjusting the visual representation of the progress based on the percentage value provided.

#### 3. **`GetCircleProgressValue`**
   - Retrieves the current progress value of a player's circle.
   - **Arguments**: Player ID, Circle ID.
   - **Returns**: Progress value as a float.
   - **Explanation**: Fetches the current value of a specific player's circle progress to use in further calculations or for displaying purposes.

#### 4. **`DestroyPlayerCircleProgress`**
   - Destroys a player's circle progress indicator.
   - **Arguments**: Player ID, Circle ID.
   - **Returns**: Success (1) or failure (0).
   - **Explanation**: Removes the circle from the player's screen, cleaning up any textdraws associated with it.

#### 5. **`DestroyAllPlayerCircleProgress`**
   - Destroys all active circle progress indicators for a player.
   - **Arguments**: Player ID.
   - **Returns**: Success (1) or failure (0).
   - **Explanation**: This function will clean up and remove all circles associated with a player when they disconnect or no longer need the progress indicators.

#### 6. **`ShowPlayerCircleProgress`**
   - Displays a player's circle progress.
   - **Arguments**: Player ID, Circle ID.
   - **Returns**: Success (1).
   - **Explanation**: Makes the circle progress visible to the player. This is called after updating the circle's progress value.

#### 7. **`HideCircleProgressForPlayer`**
   - Hides a player's circle progress.
   - **Arguments**: Player ID, Circle ID.
   - **Returns**: Success (1).
   - **Explanation**: Hides the circle from the player's view. This can be used when you want to temporarily hide the circle without destroying it.

#### 8. **`CalculateTextDrawPosition`**
   - Calculates the position for textdraws on the circle.
   - **Arguments**: Angle, distance, start position (x, y), output (x, y).
   - **Returns**: Boolean `true`.
   - **Explanation**: Determines the coordinates where each point of the circle will be placed, ensuring that the circle’s segments are evenly distributed.

### 9. **`SetCircleProgressMaxValue`**
   - Sets the maximum progress value for a player's circle.
   - **Arguments**: Player ID, Circle ID, max progress value.
   - **Returns**: Success (1) or failure (0).
   - **Explanation**: Updates the maximum progress value for a specific player's circle.

### 10. **`SetCircleProgressColor`**
   - Sets the color of a player's circle progress.
   - **Arguments**: Player ID, Circle ID, color (RGB values).
   - **Returns**: Success (1) or failure (0).
   - **Explanation**: Changes the color of the circle's progress bar to the specified RGB values.

### 11. **`SetCircleBackgroundColor`**
   - Sets the background color of a player's circle progress.
   - **Arguments**: Player ID, Circle ID, color (RGB values).
   - **Returns**: Success (1) or failure (0).
   - **Explanation**: Changes the background color of the circle to the specified RGB values.

### 12. **`CircleColorChanging`**
   - Checks if the circle color is changing the higher the value.
   - **Arguments**: Player ID, Circle ID, allow (true/false).
   - **Returns**: Success (1) or failure (0).
   - **Explanation**: This function is used to check if the circle color is changing the higher the circle value

### 13. **`SetCircleMaxColor`**
   - Sets the maximum color of a player's circle progress.
   - **Arguments**: Player ID, Circle ID, color (RGB values).
   - **Returns**: Success (1) or failure (0).
   - **Explanation**: Sets the color of the circle's progress bar when it reaches its maximum value
---

## ⚡ Conclusion

This **Circle Progress** script is a powerful tool for adding circular progress indicators to your project. Whether you're building a game, an application, or a tool that requires visualizing progress, this system allows you to create smooth and customizable indicators for your users. With features like multiple circle support, optimized performance, and easy customization, it's a great addition to any SA-MP project! 🌟

Use it for visual effects like loading bars, health/XP tracking, or any other scenario where progress needs to be represented visually. With minimal lag and high flexibility, your players will enjoy a smooth and interactive experience. 🎮

---

Enjoy coding with Circle Progress! 🚀
