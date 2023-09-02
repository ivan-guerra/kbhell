# kbhell

https://github.com/ivan-guerra/kbhell/assets/13009513/5ec36209-92d0-4365-8710-2c1a3f22d772

kbhell (short for keyboard hell) is a Windows and Linux desktop application
which plays a WAV audio soundbite everytime a user releases a key. The best part
is that the kbhell application does not need to be in the focused window (or in
a window at all) for it to play sound. You can launch kbhell in the background
on your unsuspecting friend's PC and watch them lose their mind.

### Program Usage

Program usage is as follows:

```text
usage: kbhell WAV_FILE
play WAV_FILE everytime a key is released (press ESC to stop)
```

You must give kbhell a WAV file to play on key release. Shorter WAV soundbites
work best. You can find a sample quacking duck WAV under
`kbhell/sounds/quack.wav`. There's plenty of [free use WAV files][1] online.

To terminate kbhell, press the escape (`ESC`) key anywhere on the desktop.

### Building

To build the project locally, you will need the following libraries and tools
installed:

* CMake3.27+
* C++ compiler supporting C++20 features
* [Doxygen][2]
* Visual Studio 2022 (Windows Only)
* X11 Developer Libs (Linux Only)
* ALSA and PulseAudio Developer Libs (Linux Only)

> **Note**
> Older versions of Visual Studio may work as well. I only tested with VS2022
> because that is what I had available. To try out an older version, update
> the generator arg in the first `cmake` command located in the
> `kbhell/scripts/windows/build.bat` script.

To build the project on either Windows or Linux, change directory to the
`scripts/[windows|linux]` directory and run `build.[bat|sh]`.

After a successful build, you will find the binary installed to `kbhell/bin/`.

### Running the Tests

`kbhell` has been unit tested using the GoogleTest framework in conjunction with
`ctest`. To run the test, build the project and change directory to
`kbhell/build/`.  Run `ctest` to execute all unit tests.

### Doxygen Docs

This project is documented using Doxygen. Doxygen docs are built automatically
by both the Windows and Linux build script. Docs can be found under
`kbhell/docs/kbhell/`.

[1]: https://pixabay.com/sound-effects/search/wav/
[2]: https://www.doxygen.nl/
