[![Build Status](https://travis-ci.org/bariscelik/logcolor.svg?branch=main)](https://travis-ci.org/bariscelik/logcolor)

# logcolor
logcolor, or lc in short, is a very basic log viewer with colors. It works in command line and supplies elementary features to view log files. It is easy to setup and customize.

## Features
- Colorize matched texts in the log input
- Works with pipe operator |
- Line numbering
- Easy usage
- No prerequisites

## Installation
Download or clone repo and enter into root folder:
```bash
mkdir build
cd build
cmake ..
make # or make install to system-wide using
```

## Usage

### Log file with default colors
```console
foo@bar:~$ lc -f file.log
```

### Pipe operator as input
```console
foo@bar:~$ cat /var/log/syslog | lc
```

### Adding line number
```console
foo@bar:~$ lc -l -f file.log
```

### Filter results into last ``n`` lines
```console
foo@bar:~$ lc -n 300 -f file.log # last 300 lines
```

## License
logcolor is an open source software licensed under MIT. See the LICENSE file for more

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
