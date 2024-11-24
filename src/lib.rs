use rdev::{listen, Event, EventType, Key};
use rodio::{Decoder, OutputStream, Sink};
use std::fs::File;
use std::io::BufReader;
use std::sync::{Arc, Mutex};

pub fn play_audio(audio_file: &std::path::PathBuf) -> Result<(), Box<dyn std::error::Error>> {
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

pub fn run(audio_file: &std::path::Path) {
    let audio_file = Arc::new(Mutex::new(audio_file.to_path_buf()));

    // Clone the Arc to share ownership with the callback
    let audio_file = Arc::clone(&audio_file);

    // Start listening for events
    if let Err(error) = listen(move |event| keypress_callback(event, &audio_file)) {
        eprintln!("error: {:?}", error);
        std::process::exit(1);
    }
}
