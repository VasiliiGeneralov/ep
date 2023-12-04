# DEPRECATED

1. Get and build [vcpkg](https://github.com/microsoft/vcpkg)
1. Use `scripts/prepare_deps/prepare_deps.py` to acquire transitive [dependencies to build Qt from source](#dependencies-to-build-qt-from-source) automatically

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
sudo apt install libxkbcommon-dev=1.5.0-1
```

For Arch based distros:
```
sudo pacman -U \
https://archive.archlinux.org/packages/l/libxkbcommon/libxkbcommon-1.5.0-1-x86_64.pkg.tar.zst
```

