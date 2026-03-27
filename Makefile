SPRITES_DIR := ./aseprite/sprites
EXPORTS_DIR := ./aseprite/exports
ASSETS_DIR  := ./include/assets

SPRITES := $(shell find $(SPRITES_DIR) -name '*.aseprite')
EXPORTS := $(SPRITES:$(SPRITES_DIR)/%.aseprite=$(EXPORTS_DIR)/%.png)
ASSETS  := $(SPRITES:$(SPRITES_DIR)/%.aseprite=$(ASSETS_DIR)/%.xbm)

all: assets
	echo $(ASSETS)

assets: $(ASSETS)

exports: $(EXPORTS)

$(EXPORTS_DIR)/%.png: $(SPRITES_DIR)/%.aseprite
	mkdir -p $(EXPORTS_DIR)
	aseprite --batch $< --save-as $@

$(ASSETS_DIR)/%.xbm: $(EXPORTS_DIR)/%.png
	mkdir -p $(ASSETS_DIR)
	ffmpeg -i $< $@

.PHONY: all assets exports clean

clean:
	rm -rf $(EXPORTS_DIR)
	rm -rf $(ASSETS_DIR)
