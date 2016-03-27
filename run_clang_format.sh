#!/bin/zsh
setopt extended_glob
ls src/*.(h|hpp|cpp) | xargs clang-format -i
ls tests/*.(h|hpp|cpp) | xargs clang-format -i
ls meta/*.(h|hpp|cpp) | xargs clang-format -i
