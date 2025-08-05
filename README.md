# Examples

`.\build\imgcli.exe sketch readme/balaton.jpg readme/sketch.png 6`
<img src="https://raw.githubusercontent.com/bks1b/image-cli/main/readme/sketch.png">

`.\build\imgcli.exe ascii readme/text.png readme/ascii.txt 2 2`
<img src="https://raw.githubusercontent.com/bks1b/image-cli/main/readme/ascii.png">

# Commands

All parameters are optional.

| Name   | Description                          | Parameters                                  | 
|--------|--------------------------------------|---------------------------------------------|
| sketch | Applies a sketch effect to an image. | color count (quantization), greyscale (0/1) | 
| ascii  | Generates ASCII art from an image.   | shrink X, shrink Y, brightness weight       | 

# Build

Download [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) and [stb_image_write](https://github.com/nothings/stb/blob/master/stb_image_write.h) into `src`, then run `g++ src/*/*.cpp src/main.cpp -o build/imgcli.exe -I include -std=c++20 -Wno-narrowing` to build the project using g++.
