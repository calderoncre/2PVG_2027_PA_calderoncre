#!/bin/sh
# Generates build/Blackjack.xcworkspace for Xcode.
cd "$(dirname "$0")"
chmod +x tools/premake/macos/premake5
# Downloaded binaries are quarantined by macOS; allow this one to run.
xattr -d com.apple.quarantine tools/premake/macos/premake5 2>/dev/null
tools/premake/macos/premake5 xcode4
