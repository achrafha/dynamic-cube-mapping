with import <yarnpkgs>;
stdenv.mkDerivation {
  name = "cube-mapping";
  buildInputs = [ stdenv SDL2 mesa valgrind ];
  shellHook = ''
    export CPATH=$CPATH:${SDL2}/include/SDL2
    zsh
  '';
}
