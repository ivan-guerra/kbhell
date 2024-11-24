//! A keyboard sound effects program that plays audio files in response to keypresses.
//!
//! This program listens for keyboard events and plays a specified audio file when
//! any key is pressed. It uses the `rdev` crate for keyboard input handling and
//! the `rodio` crate for audio playback.
//!
//! The program can be terminated by pressing the Escape key.
//!
use clap::Parser;
use rdev::{listen, Event, EventType, Key};
use rodio::{Decoder, OutputStream, Sink};
use std::fs::File;
use std::io::BufReader;
use std::sync::{Arc, Mutex};

#[doc(hidden)]
#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
struct Args {
    #[arg(help = "audio file to play on keypress")]
    audio_file: std::path::PathBuf,
}

/// Plays an audio file using the default audio output device.
///
/// # Arguments
///
/// * `audio_file` - A path to the audio file to be played
///
/// # Returns
///
/// * `Ok(())` if the audio playback was successful
/// * `Err(Box<dyn Error>)` if there was an error during playback
///
/// # Errors
///
/// This function will return an error if:
/// * The default audio output stream cannot be created
/// * The audio sink cannot be created
/// * The audio file cannot be opened
/// * The audio file cannot be decoded
#[doc(hidden)]
fn play_audio(audio_file: &std::path::PathBuf) -> Result<(), Box<dyn std::error::Error>> {
    // Create an audio output stream
    let (_stream, stream_handle) = OutputStream::try_default()?;

    // Create a sink to control the playback
    let sink = Sink::try_new(&stream_handle)?;

    // Load the audio file
    let file = File::open(audio_file)?;
    let source = Decoder::new(BufReader::new(file))?;

    // Attach the source to the sink
    sink.append(source);

    // Block the thread until the audio finishes playing
    sink.sleep_until_end();

    Ok(())
}

/// Callback function that handles keyboard events and plays audio when triggered.
///
/// # Arguments
///
/// * `event` - The input event received from the event listener
/// * `filename` - A thread-safe reference to the audio file path
///
/// This function will:
/// * Exit the program if the Escape key is pressed
/// * Attempt to play the audio file for any other keypress
/// * Exit with an error code if audio playback fails
#[doc(hidden)]
fn keypress_callback(event: Event, filename: &Arc<Mutex<std::path::PathBuf>>) {
    if let EventType::KeyPress(_key) = event.event_type {
        if let EventType::KeyPress(Key::Escape) = event.event_type {
            std::process::exit(0);
        }

        let audio_file = filename.lock().unwrap();
        if let Err(error) = play_audio(&audio_file) {
            eprintln!("error: {}", error);
            std::process::exit(1);
        }
    }
}

#[doc(hidden)]
fn main() {
    let args = Args::parse();

    let audio_file = Arc::new(Mutex::new(args.audio_file.to_path_buf()));

    // Clone the Arc to share ownership with the callback
    let audio_file = Arc::clone(&audio_file);

    // Start listening for events
    if let Err(error) = listen(move |event| keypress_callback(event, &audio_file)) {
        eprintln!("error: {:?}", error);
        std::process::exit(1);
    }
}
