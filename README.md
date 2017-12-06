# thunar-torrent-property

*A small thunar extension displaying the metadata in a torrent file.*

[![build status](https://gitlab.com/althonos/thunar-torrent-property/badges/master/build.svg)](https://gitlab.com/althonos/thunar-torrent-property/commits/master)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/9172fcaad5724418b72ace8b31ba5874)](https://www.codacy.com/app/althonos/thunar-torrent-property?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=althonos/thunar-torrent-property&amp;utm_campaign=Badge_Grade)

## Features

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

###### Ubuntu **>= 16.10 (yakkety)**

```
apt install cmake libthunarx-2-dev libgtk2.0-dev \
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
directory is running `pkg-config --variable=extensionsdir thunarx-2`,
most of the time it will be `/usr/lib/thunarx-2` or `/lib/thunarx-2`).
