{pkgs ? import <nixpkgs> {}}:
pkgs.mkShell {
  buildInputs = with pkgs; [
    platformio
    ccls
    clang-tools
    gnumake
  ];
}
