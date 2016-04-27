with import <yarnpkgs>;
stdenv.mkDerivation {
  name = "cube-mapping";
  buildInputs = [ stdenv SDL2 mesa valgrind glew ];
  shellHook = ''
    export CPATH=$CPATH:${SDL2}/include/SDL2:${glew}/include/GL
    zsh
  '';
}
