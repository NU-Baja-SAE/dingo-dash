# dingo-dash

LCD dashboard using u8g2 alongside an ESP32 and T6963C (Toshiba) 240x128 Graphic LCD

## Datasheets

- Display module [datasheet](https://www.buydisplay.com/download/manual/ERM240128-2_Series_Datasheet.pdf)
- Controller IC (RA6963) [datasheet](https://www.buydisplay.com/download/ic/RA6963.pdf)

## Assets
u8glib uses images in the `xbm` format, which can be included directly in C/C++ source code. 

### Prerequisites
To create an image that can be loaded with u8glib, you will need:

- [aseprite](https://github.com/aseprite/aseprite) for creating the image with the proper color mode and exporting as a `.png` (alternatives: gimp, photoshop)
- [imagemagick](https://github.com/ImageMagick/ImageMagick) for converting the aseprite exports to the `.xbm` format

### Adding new images
Once you have installed the prerequisites (i.e., `aseprite` and `imagemagick` are in your PATH):

1. Create a new sprite in aseprite with indexed color mode
2. Import the monochrome palette at `aseprite/resources/palette.aseprite` by clicking the burger menu > load palette
3. Design your sprite!
4. Save your sprite as a `.aseprite` file in `aseprite/sprites/`
4. Run `./generate_assets.sh`. This will use aseprite to export all of the sprites in `aseprite/sprites/` as `.png` images, and then imagemagick to export all of these `.png` images as `.xbm` images in `include/assets`. 
5. You can now #include the generated `.xbm` files directly in your C/C++ source code.
