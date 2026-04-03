#!/usr/bin/env sh

SPRITES_DIR="./aseprite/sprites"
EXPORTS_DIR="$(mktemp -d)"
ASSETS_DIR="./include/assets"

# clean up current assets dir
rm -rf $ASSETS_DIR

# export aseprite files as pngs
mkdir -p "$EXPORTS_DIR"
for SPRITE in $(find "$SPRITES_DIR" -name '*.aseprite'); do
    EXPORT="$EXPORTS_DIR/$(basename "$SPRITE" .aseprite).png"
    aseprite --batch "$SPRITE" --save-as "$EXPORT"
done

# convert pngs to xbms
mkdir -p "$ASSETS_DIR"
for EXPORT in $(find "$EXPORTS_DIR" -name '*.png'); do
    ASSET="$ASSETS_DIR/$(basename $EXPORT .png).xbm"
	magick $EXPORT $ASSET
done

# Make sure the temp directory gets removed on script exit.
trap "exit 1"                 HUP INT PIPE QUIT TERM
trap 'rm -rf "$EXPORTS_DIR"'  EXIT

