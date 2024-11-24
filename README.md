# kbhell

https://github.com/ivan-guerra/kbhell/assets/13009513/5ec36209-92d0-4365-8710-2c1a3f22d772

kbhell (short for keyboard hell) is a Windows and Linux desktop application
which plays a audio soundbite everytime a user releases a key. The best part is
that the kbhell application does not need to be in the focused window (or in a
window at all) for it to play sound. You can launch kbhell in the background of
your unsuspecting friend's PC and watch them lose their mind.

### Program Usage

Program usage is as follows:

```text
Annoy everyone around you by playing noises everytime you press a key.

Usage: kbhell <AUDIO_FILE>

Arguments:
  <AUDIO_FILE>  audio file to play on keypress

Options:
  -h, --help     Print help
  -V, --version  Print version
```

You must give kbhell a audio file to play on key release. Shorter soundbites
work best. You can find a sample quacking duck WAV under
`kbhell/sounds/quack.wav`. There's plenty of [free use WAV files][1] online. Any
audio file format supported by the default configuration of the [rodio][2] crate
is supported.

To terminate kbhell, press the escape (`ESC`) key anywhere on the desktop.

[1]: https://pixabay.com/sound-effects/search/wav/
[2]: https://crates.io/crates/rodio
