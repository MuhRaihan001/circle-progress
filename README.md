
# 🌀 Circle Progress

Welcome to the **Circle Progress** script! This feature lets you create smooth, customizable circle progress indicators for your players. Perfect for visualizing progress bars, loading indicators, and more!

![Preview](https://github.com/MuhRaihan001/circle-progress/blob/main/sa-mp-017.png)

## ✨ Features
- **Smooth Circles**: Render circular progress indicators with customizable size, color, and thickness.
- **Multiple Circles**: Support for up to 100 circles at once.
- **Customizable Polygons**: Define polygon density for smoothness.
- **Optimized**: Efficiently manages multiple textdraws with minimal lag.

## 🔧 Setup Instructions
1. Define the maximum number of circles in `MAX_CIRCLES_DRAW`.
2. Customize the default size, thickness, and colors to match your project.
3. Use `CreatePlayerCircleProgress` to create circles and set values with `SetPlayerCircleProgressValue`.

### ⚠️ Warnings and Errors
- **Too Many Circles**: Setting `MAX_CIRCLES_DRAW` above 100 will trigger a warning.
- **Minimum Requirements**: Ensure `DEFAULT_CIRCLE_POLYGONS` is above 3.0, `MAX_POINTS` is at least 1, and `DEFAULT_SIZE` is greater than 0.0.

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


**Note**: Be sure to set a valid number of circles and polygons to avoid issues.

Happy coding! 🎨
