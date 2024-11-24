use clap::Parser;

#[derive(Parser, Debug)]
#[command(version, about, long_about = None)]
struct Args {
    #[arg(help = "audio file to play on keypress")]
    audio_file: std::path::PathBuf,
}

fn main() {
    let args = Args::parse();
    kbhell::run(&args.audio_file);
}
