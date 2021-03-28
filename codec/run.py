#!/usr/bin/env python3
import os
import subprocess


if __name__ == '__main__':
    out_dir = 'out'
    build_dir = 'build'
    if not os.path.exists(out_dir):
        os.mkdir(out_dir)
    if not os.path.exists(build_dir):
        os.mkdir(build_dir)

    subprocess.run(['cmake', '-S', '.', '-B', build_dir])
    subprocess.run(['make', '-C', build_dir])

    examples = [
        'PBMFormat',
        'BitmapFormat',
        'WavCodec',
        'PcmCodec'
    ]

    for index, example in enumerate(reversed(examples)):
        exe = os.path.join(build_dir, example)
        args = [exe]

        no = len(examples) - index
        if no == 1:
            print('保存PBM图片格式')
            args.append('out/format_pgm_p5.pgm')
            args.append('out/format_ppm_p6.ppm')
        elif no == 2:
            print('BMP格式保存为PPM图片')
            args.append('../res/sample-640x426.bmp')
            args.append('out/bitmap2ppm.ppm')
        elif no == 3:
            print('解码WAV音频格式')
            args.append('../res/sample-wav-1m.wav')
        elif no == 4:
            print('分析PCM数据')

        subprocess.run(args)
        print()
