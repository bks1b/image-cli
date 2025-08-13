# Examples
{examples}

# Commands

Use the `--color=<#fff|#ffffff|255,255,255>` flag to specify the result's color, or `--greyscale` to convert the result to greyscale. By default, the original image's colors are used.

All parameters are optional.

{commands}

# Build

Download [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) and [stb_image_write](https://github.com/nothings/stb/blob/master/stb_image_write.h) into `src`, then run `g++ src/*/*.cpp src/main.cpp -o build/imgcli.exe -I include -std=c++20 -Wno-narrowing` to build the project using g++.