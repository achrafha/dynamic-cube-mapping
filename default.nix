with import <yarnpkgs>;
stdenv.mkDerivation {
  name = "cube-mapping";
  buildInputs = [ stdenv SDL2 mesa valgrind ];
  shellHook = ''
    zsh
  '';
}
