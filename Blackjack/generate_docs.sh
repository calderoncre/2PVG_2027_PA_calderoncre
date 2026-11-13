#!/bin/sh
# Generates the API documentation in doc/html (open doc/html/index.html).
# The first time it downloads a portable doxygen into tools/doxygen/macos.
set -e
cd "$(dirname "$0")"
VERSION=1.18.0
DIR=tools/doxygen/macos
if [ ! -x "$DIR/doxygen" ]; then
  case "$(uname -m)" in
    arm64) ARCH=arm ;;
    *) ARCH=intel ;;
  esac
  TAG="Release_$(echo "$VERSION" | tr . _)"
  echo "Downloading doxygen $VERSION..."
  mkdir -p "$DIR"
  curl -fL -o "$DIR/doxygen.zip" \
    "https://github.com/doxygen/doxygen/releases/download/$TAG/doxygen-$VERSION-mac-$ARCH.zip"
  unzip -j -o "$DIR/doxygen.zip" "doxygen-$VERSION/doxygen" -d "$DIR"
  rm "$DIR/doxygen.zip"
  chmod +x "$DIR/doxygen"
fi
"$DIR/doxygen" Doxyfile
echo "Done: open doc/html/index.html"
