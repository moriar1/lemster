# Lemster

Desktop application for posting to [Lemmy](https://github.com/LemmyNet/lemmy) instances.

![](misc/gui.jpg)

## Features
- Simple scheduler
- GUI

# Usage

Click on `configuration` button on menu bar. Put your login credentials.
Click `Add post` to add new post in scheduler.
`Post` button will create post on lemmy by information in first record.

> [!NOTE]
> Your JWT stores in application directory in unencrypted plain text file (path to file: see [Uninstall](#Uninstall)).

# Build

## Linux

**Requirements:**

Install the following packages using your package manager:
- Qt6-base package
- CMake

Run:

```bash
git clone https://github.com/moriar1/lemster
cd lemster
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
```

if want to run `lemster` only in current directory:

```bash
make
./lemster
``` 

or if you want system-wide install:

```bash
make install # may need sudo
lemster
```

### Windows

Open QtCreator and build the project.

# Uninstall

## Delete config files:

### Linux


```bash
rm -r $HOME/.local/share/lemster
```

### Windows

Delete one of the following folder:
- `C:/Users/<USER>/AppData/Roaming/lemster`
- `C:/ProgramData/lemster`
- `lemster/data/lemster`

## Delete binary

If you used `make install` run from `lemster/build`:

```bash
xargs rm < install_manifest.txt # may need sudo
```

# TODO
- [x] Replace jwt with login+password credentials
- [ ] Replace community ID with community name + Completer.
- [ ] Solve qFatal() build error on Ubuntu
- [ ] Windows/Linux deploy
