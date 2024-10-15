import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path
from typing import List

def compile() -> int:
    print("Starting compilation process...")
    shutil.copy('Arduino.h', 'src/Arduino.h')
    print("Copied Arduino.h to src/Arduino.h")
    max_attempts = 2
    
    for attempt in range(1, max_attempts + 1):
        try:
            print(f"Attempting compilation (attempt {attempt}/{max_attempts})...")
            subprocess.run(['pio', 'run'], check=True)
            print(f"Compilation successful on attempt {attempt}")
            return 0
        except subprocess.CalledProcessError:
            print(f"Compilation failed on attempt {attempt}")
            if attempt == max_attempts:
                print("Max attempts reached. Compilation failed.")
                return 1
            print("Retrying...")
    return 1

def insert_header(file: Path) -> None:
    print(f"Inserting header in file: {file}")
    with open(file, 'r') as f:
        content = f.read()
    
    content = content.replace('#include "platforms/stub/wasm/js.h"\n', '')
    content = '#include "platforms/stub/wasm/js.h"\n' + content
    
    with open(file, 'w') as f:
        f.write(content)
    print(f"Processed: {file}")

def include_deps() -> None:
    print("Including dependencies...")
    src_dir = Path('/js/src')
    ino_files = list(src_dir.glob('*.ino'))
    
    if ino_files:
        print(f"Found .ino file: {ino_files[0]}")
        main_cpp = src_dir / 'main.cpp'
        if main_cpp.exists():
            print("main.cpp already exists, renaming to main2.hpp")
            main_cpp.rename(src_dir / 'main2.hpp')
        
        print(f"Renaming {ino_files[0]} to generated_main.cpp")
        ino_files[0].rename(src_dir / 'generated_main.cpp')
        
        if (src_dir / 'main2.hpp').exists():
            print("Including main2.hpp in main.cpp")
            with open(src_dir / 'main.cpp', 'a') as f:
                f.write('#include "main2.hpp"\n')
    
    print("Processing all source files...")
    for file in src_dir.rglob('*'):
        if file.suffix in ('.ino', '.h', '.hpp', '.cpp'):
            insert_header(file)

def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Compile FastLED for WASM")
    parser.add_argument('--keep-files', action='store_true', help="Keep source files after compilation")
    return parser.parse_args()

def main() -> int:
    print("Starting FastLED WASM compilation script...")
    args = parse_args()
    print(f"Keep files flag: {args.keep_files}")

    print("Copying files from mapped directory to container...")
    js_src = Path('/js/src')
    js_src.mkdir(parents=True, exist_ok=True)
    mapped_dirs: List[Path] = list(Path('/mapped').iterdir())
    if len(mapped_dirs) > 1:
        print("Error: More than one directory found in /mapped")
        return 1
    
    src_dir: Path = mapped_dirs[0]
    for item in src_dir.iterdir():
        if item.is_dir():
            print(f"Copying directory: {item}")
            shutil.copytree(item, js_src / item.name, dirs_exist_ok=True)
        else:
            print(f"Copying file: {item}")
            shutil.copy2(item, js_src / item.name)

    print("Changing working directory to /js")
    os.chdir('/js')
    include_deps()
    
    print("Starting compilation...")
    if compile() != 0:
        print("Compilation failed. Exiting.")
        return 1

    print("Compilation successful. Copying output files...")
    fastled_js_dir: Path = src_dir / 'fastled_js'
    fastled_js_dir.mkdir(parents=True, exist_ok=True)

    build_dir: Path = next(Path('.pio/build').iterdir())
    
    for file in ['fastled.js', 'fastled.wasm']:
        print(f"Copying {file} to output directory")
        shutil.copy2(build_dir / file, fastled_js_dir / file)
    
    print("Copying index.html to output directory")
    shutil.copy2('index.html', fastled_js_dir / 'index.html')

    if not args.keep_files:
        print("Removing temporary source files")
        shutil.rmtree(js_src)
    else:
        print("Keeping temporary source files")

    print("Compilation process completed successfully")
    return 0

if __name__ == "__main__":
    sys.exit(main())
