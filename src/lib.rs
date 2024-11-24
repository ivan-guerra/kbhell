use rodio::{Decoder, OutputStream, Sink};
use std::fs::File;
use std::io::BufReader;

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
