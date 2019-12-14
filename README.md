# thunar-torrent-property

*A small thunar extension displaying the metadata in a torrent file.*

[![GitLabCI](https://img.shields.io/gitlab/pipeline/althonos/thunar-torrent-property?logo=GitLab&style=flat-square&maxAge=3600)](https://gitlab.com/althonos/thunar-torrent-property/commits/master)
[![License](https://img.shields.io/badge/license-GPLv3-blue.svg?logo=gnu&style=flat-square&maxAge=2678400)](https://choosealicense.com/licenses/gpl-3.0/)
[![Source](https://img.shields.io/badge/source-GitHub-303030.svg?logo=github&maxAge=2678400&style=flat-square)](https://github.com/althonos/thunar-torrent-property/)
[![Sanity](https://img.shields.io/codacy/grade/9172fcaad5724418b72ace8b31ba5874?logo=codacy&style=flat-square)](https://www.codacy.com/app/althonos/thunar-torrent-property)

## Features

* Works with both `thunarx-2.0` and `thunarx-3.0`
* Fetches the number of seeders / leechers currently available
* Display a list of tracker URLs
* Display the files described by the **torrent** file in a
  tree-based viewer

## Usage

1. Right Click on a torrent file and open *Properties...*
2. Go to the *Torrent* tab
3. ???
4. Profit.

![Torrent property page](/static/showcase.png)

## Installation

### Dependencies

#### Build

* `cmake`
* `make` or `Ninja`
* a `C` and `C++` compiler (at the moment, only `gcc` is actively supported)


#### Runtime dependencies

* `gtk+-2.0`
* `thunarx` (installed with [XFCE / Thunar](http://docs.xfce.org/xfce/thunar/start))
* `libtorrent` (the [rasterbar](http://www.libtorrent.org/) version), **>= 1.1.0**
* `Boost/system` (required to interface with `libtorrent`)


### Installing dependencies (tested distributions)

###### ArchLinux

```
pacman -S base-devel cmake libtorrent-rasterbar thunar gtk2 boost
```

###### Gentoo

```
eselect repository enable linxon && emaint sync --repo linxon
emerge -av xfce-extra/thunar-torrent-property
```

###### Ubuntu **>= 16.10 (yakkety)**

```
apt install cmake libthunarx-2-dev libgtk2.0-dev \
    libboost-filesystem-dev libboost-system-dev libglib2.0-dev \
    build-essential libssl-dev libtorrent-rasterbar-dev
```

###### Ubuntu **>= 19.04 (disco)**
```
apt install cmake libthunarx-3-dev libgtk2.0-dev \
    libboost-filesystem-dev libboost-system-dev libglib2.0-dev \
    build-essential libssl-dev libtorrent-rasterbar-dev
```

###### Fedora

```
dnf install gtk+-devel boost-devel cmake gcc Thunar-devel \
    rb_libtorrent-devel make gcc-c++
```


### Building from source

This project uses `CMake`. Start by creating a directory to do an
an out-of-source build:
```
mkdir build
cd build
```

By default, the target `thunarx` version is detected automatically
using `pkg-config`. Set the `THUNARX_TARGET_VERSION` variable to
2 or 3 to manually select the version to build against.


###### Ninja

```
cmake .. -GNinja
ninja
sudo ninja install
```

###### Make
```
cmake ..
make
sudo make install
```


Both commands will install a single file, `thunar-torrent-property.so`,
in the `extensions` directory of **Thunar** (you can check where that
directory is running `pkg-config --variable=extensionsdir thunarx-X`,
most of the time it will be `/usr/lib/thunarx-X` or `/lib/thunarx-X`)
where `X` is either `2` or `3` depending on your library version.
