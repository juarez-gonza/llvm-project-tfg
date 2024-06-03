#!/usr/bin/sh

"$(git rev-parse --show-toplevel)/build/bin/clang" -std=c++23 -lstdc++ -Xclang -ast-dump "${1}"
