# Wofi (Fork)

Wofi is a launcher/menu program for wlroots-based Wayland compositors such as sway.

This repository is a **fork** of [https://hg.sr.ht/~scoopta/wofi](https://hg.sr.ht/~scoopta/wofi) with additional features, including support for **external hooks and dynamic menu entries**. Changes can be seen in the [commit history](https://github.com/SimplyCEO/wofi/commits/master/?author=SimplyCEO).

---

## Features in This Fork

* Full support for standard Wofi functionality.
* **Dynamic menu entries:** Labels and commands for an "externally hooked" entry can now be generated and updated in real-time via external executables.
* **Custom executable execution:** Hooked entry can execute arbitrary external programs.
* **Example included:** `/externals` directory demonstrates a direct web search feature (e.g., typing `??red balloon images` opens your browser with a Google search for "red balloon images").

---

## Dependencies

* PkgConf
* Wayland
* GLib2
* GObject
* GTK3

`cmake` or `meson` is required to build. Use **CMake** if your C compiler is not **GCC** or **Clang**.

---

## Building

Clone the repository:

```bash
git clone --depth 1 https://github.com/PeterRiek/wofi.git
cd wofi
```

### Using CMake

```bash
cmake -S . -B build \
  -DCMAKE_INSTALL_PREFIX=/usr/local \
  -DENABLE_RUN=1 \
  -DENABLE_DRUN=1 \
  -DENABLE_DMENU=1
cmake --build build
```

Available options are listed in [CMakeLists.txt](/CMakeLists.txt):

```bash
cat CMakeLists.txt | head -n 87 | tail -n $((87-66)) | less
```

### Using Meson

```bash
meson setup build
ninja -C build
```

---

## Installing

* **CMake:**

```bash
su -c "cmake --install build"
```

* **Meson:**

```bash
su -c "ninja -C build install"
```

---

## Uninstalling

* **CMake:**

```bash
su -c "xargs -a build/install_manifest.txt rm"
```

* **Meson:**

```bash
su -c "ninja -C build uninstall"
```

---

## Bug Reports

This fork is **not** connected to mainstream Wofi. For bugs related to upstream behavior, please report them at [SourceHut](https://todo.sr.ht/~scoopta/wofi).

---

## Contributing

It is advised to submit patches to [https://lists.sr.ht/~scoopta/wofi](https://lists.sr.ht/~scoopta/wofi), as this fork is not upstreamed.

Documentation is available [here](https://man.sr.ht/hg.sr.ht/email.md).

---

## drun and dbus

Some desktop files declare themselves as being launched by dbus. On systems where a user session bus is not automatically started (such as systems using elogind), Wofi may experience issues.

To manually start a session bus:

```bash
dbus-daemon --session --address=unix:path=$XDG_RUNTIME_DIR/bus
```

---

## Packages

[![Packaging status](https://repology.org/badge/vertical-allrepos/wofi.svg)](https://repology.org/project/wofi/versions)

---

## Documentation

Official documentation is provided via man pages. Sample styling can be found [here](https://cloudninja.pw/docs/wofi.html).

---

## Donating

Support development at [https://ko-fi.com/scoopta](https://ko-fi.com/scoopta).

---

## Screenshots

[![example 4](https://f.cloudninja.pw/Scaled_4.png)](https://f.cloudninja.pw/Rootbar_Example_4.png)

