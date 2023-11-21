# EP

Editor on patterns.

# Building from source

1. Get and build [vcpkg](https://github.com/microsoft/vcpkg)
1. Acquire transitive [dependencies to build Qt from source](#dependencies-to-build-qt-from-source)
1. The rest is done by `cmake` automatically:
`cmake -S . -B build/ -G Ninja && cmake --build build/`

## Dependencies to build Qt from source

For Debian based distros:
```
sudo apt install       \
  libx11-xcb-dev       \
  libglu1-mesa-dev     \
  libxrender-dev       \
  libxi-dev            \
  libxkbcommon-x11-dev \
  libwayland-dev
```

For Arch based distros:
```
sudo pacman -Su    \
  libx11           \
  mesa             \
  libxrender       \
  libxi            \
  libxkbcommon-x11 \
  wayland
```

_Errare humanum est_, latest Qt pulled by vcpkg fails to build with latest
`libxkbcommon`
([one](https://www.linuxquestions.org/questions/slackware-14/%5Btrivial%5D-regression-qt5-failed-to-build-with-new-libxkbcommon-1-6-0-a-4175729868/),
[two](https://bugreports.qt.io/browse/QTBUG-117950)), make sure to install a
version _older_ than 1.6.

For Debian based distros:
```
sudo apt install libxkbcommon0=1.5.0-1
```

For Arch based distros:
```
sudo pacman -U \
https://archive.archlinux.org/packages/l/libxkbcommon/libxkbcommon-1.5.0-1-x86_64.pkg.tar.zst
```

# Reference

1. "Design Patterns: Elements of Reusable Object-Oriented Software",
Erich Gamma, Richard Helm, Ralph Johnson, John Vlissides
1. [Qt Quick Controls - Text Editor](https://code.qt.io/cgit/qt/qtdeclarative.git/tree/examples/quickcontrols/texteditor)
1. [File System Explorer](https://code.qt.io/cgit/qt/qtdeclarative.git/tree/examples/quickcontrols/filesystemexplorer)
1. [Kate](https://github.com/KDE/kate)
