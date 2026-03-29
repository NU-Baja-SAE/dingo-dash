SPRITES_DIR := aseprite/sprites
EXPORTS_DIR := aseprite/exports
ASSETS_DIR  := include/assets

# list the outputs produced when saving the aseprite file $(1) as $(2)
# TODO do both Output and Overwrite work?
define ase_outputs_of
  $(shell aseprite --batch "$(1)"                                              \
      --save-as "$(1:$(SPRITES_DIR)/%.aseprite=$(EXPORTS_DIR)/%.png)"          \
      --preview                                                                \
    | grep -e Output -e Overwrite                                              \
    | cut -d\' -f2)
endef

# just strip numeric chars (assume originals do not have any numbers)
# TODO enforce this with an error
define ase_input_of
$(shell printf '%s\n' "$(1:$(EXPORTS_DIR)/%.png=$(SPRITES_DIR)/%.aseprite)" | tr -d '0123456789')
endef

SPRITES := $(shell find $(SPRITES_DIR) -name '*.aseprite')
EXPORTS := $(foreach SPRITE,                                                   \
  $(SPRITES),                                                                  \
  $(call ase_outputs_of,$(SPRITE)))
ASSETS  := $(EXPORTS:$(EXPORTS_DIR)/%.png=$(ASSETS_DIR)/%.xbm)

all: assets
	pio run

assets: $(ASSETS)

exports: $(EXPORTS)

.SECONDEXPANSION:

$(EXPORTS_DIR)/%.png: $$(call ase_input_of,$@)
	mkdir -p $(EXPORTS_DIR)
	aseprite --batch $< --save-as $@

$(ASSETS_DIR)/%.xbm: $(EXPORTS_DIR)/%.png
	mkdir -p $(ASSETS_DIR)
	magick $< $@

.PHONY: all assets exports clean upload

clean:
	rm -rf $(EXPORTS_DIR)
	rm -rf $(ASSETS_DIR)

upload: assets
	pio run --target upload
