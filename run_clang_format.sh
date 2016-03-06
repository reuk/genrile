#!/bin/zsh
setopt extended_glob
ls src/*.(h|hpp|cpp) | xargs clang-format -i
