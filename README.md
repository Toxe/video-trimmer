# Video Trimmer

## Build

Default build instructions for CMake and Vcpkg. These examples assume that Vcpkg is installed in your home directory. Adjust the paths if necessary.

#### Vcpkg toolchain

Pass your Vcpkg toolchain file via `CMAKE_TOOLCHAIN_FILE`, for example on Windows:  
`-DCMAKE_TOOLCHAIN_FILE=%HOMEPATH%\vcpkg\scripts\buildsystems\vcpkg.cmake`

Or on Unix systems:  
`-DCMAKE_TOOLCHAIN_FILE=$HOME/vcpkg/scripts/buildsystems/vcpkg.cmake`

### Linux + Mac

```
$ mkdir build
$ cd build
$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$HOME/vcpkg/scripts/buildsystems/vcpkg.cmake ..
$ ninja
```

### Windows

```
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=%HOMEPATH%\vcpkg\scripts\buildsystems\vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows ..
$ cmake --build . --config Release
```

## Usage

```
Video Trimmer
Usage: video_trimmer [OPTIONS] [directory]

Positionals:
  directory TEXT              video directory (default: current directory)

Options:
  -h,--help                   Print this help message and exit
  -d                          dump first decoded video frame to file (default: false)
  -v [0]                      log level (-v: INFO, -vv: DEBUG, -vvv: TRACE)
  --no-vsync                  disable vsync (vsync is enabled by default)
  --font-size INT             UI font size in pixels
  --width INT:POSITIVE Needs: --height
                              window width
  --height INT:POSITIVE Needs: --width
                              window height
```
