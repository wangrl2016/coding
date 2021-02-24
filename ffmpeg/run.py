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
    print('Running FFmpeg tutorials automatically')

    if not os.path.exists('out'):
        os.mkdir('out')

    cmake_executable()

    build_dir = 'build'

    subprocess.run([cmake_executable(), build_dir])
    subprocess.run(['make', '-C', build_dir])
    print()

    examples = ['Metadata', 'Dump', 'DecodeVideo',
                'EncodeVideo', 'RGBA2Video']

    print('0. 打印多媒体文件元数据')
    subprocess.run([os.path.join(build_dir, examples[0]),
                    '../res/big-buck-bunny.mp4'])
    print()

    print('1. 打印音视频流信息')
    subprocess.run([os.path.join(build_dir, examples[1]),
                    '../res/big-buck-bunny.mp4'])
    print()

    print('2. 解码视频为PPM图片')
    subprocess.run([os.path.join(build_dir, examples[2]),
                    '../res/big-buck-bunny.mp4'])
    print()

    print('3. 构造数据编码视频')
    subprocess.run([os.path.join(build_dir, examples[3]),
                    'out/yuv420p-to-yuv422p.mp4'])
    print()

    print('4. RGBA图片构建视频')
    subprocess.run([os.path.join(build_dir, examples[4]),
                    'out/rgba-to-yuv420p.mp4'])
    print()
