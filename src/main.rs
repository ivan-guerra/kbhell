use clap::Parser;

#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
struct Args {
    #[arg(help = "audio file to play on keypress")]
    audio_file: std::path::PathBuf,
}

fn main() {
    let args = Args::parse();
    if let Err(e) = kbhell::play_audio(&args.audio_file) {
        eprintln!("error: {}", e);
        std::process::exit(1);
    }
}
