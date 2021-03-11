#!/usr/bin/env python3
import os
import subprocess


def cmake_executable():
    """
    Find the cmake executable

    :return: A string suitable for passing to subprocess functions, or None.
    """

    with open(os.devnull, 'w') as devnull:
        try:
            subprocess.call(['cmake', '--version'], stdout=devnull)
        except (OSError,):
            print('Cannot found cmake executable')
            return None
        return 'cmake'


if __name__ == '__main__':
    out_dir = 'out'
    build_dir = 'build'
    if not os.path.exists(out_dir):
        os.mkdir(out_dir)
    if not os.path.exists(build_dir):
        os.mkdir(build_dir)

    if not cmake_executable():
        print('请手动安装CMake库')
        exit(0)

    subprocess.run([cmake_executable(), '-S', '.', '-B', build_dir])
    subprocess.run(['make', '-C', build_dir])

    examples = [
        'PBMFormat', 'BitmapFormat',
        'WavCodec'
    ]

    for index, example in enumerate(reversed(examples)):
        exe = os.path.join(build_dir, example)
        args = [exe]

        if index == len(examples) - 1:
            print('保存PBM图片格式')
            args.append('out/format_pgm_p5.pgm')
            args.append('out/format_ppm_p6.ppm')
        elif index == len(examples) - 2:
            print('BMP格式保存为PPM图片')
            args.append('../res/sample-640x426.bmp')
            args.append('out/bitmap2ppm.ppm')
        elif index == len(examples) - 3:
            print('解码WAV音频格式')
            args.append('../res/sample-wav-1m.wav')

        subprocess.run(args)
        print()
