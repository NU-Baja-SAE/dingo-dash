{pkgs ? import <nixpkgs> {}}:
pkgs.mkShell {
  buildInputs = with pkgs; [
    platformio
    ccls
    # clangd doesn't seem to work
  ];
}
