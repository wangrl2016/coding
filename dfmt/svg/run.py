#!/usr/bin/env python3
import os
import subprocess

if __name__ == '__main__':
    out_dir = 'out'
    if not os.path.exists(out_dir):
        os.mkdir(out_dir)

    subprocess.run(['cargo', 'build', '--release'])
    exe = 'target/release/svg'

    subprocess.run([exe, '-i', 'test/simple-text.svg', '-o', 'out/simple-text.png', '--perf',
                    '--dump-svg', 'out/simple-text.svg'])
