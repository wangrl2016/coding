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
                'EncodeVideo', 'RGBA2Video',
                'EncodeAudio', 'DecodeAudio',
                'ScaleVideo', 'DemuxDecode',
                'FilteringVideo', 'FilteringAudio',
                'TranscodeAAC'
                ]

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

    print('5. 编码MP3音频')
    subprocess.run([os.path.join(build_dir, examples[5]),
                    'out/16bit-dummy-audio.mp3'])
    print()

    print('6. 解码MP3音频')
    subprocess.run([os.path.join(build_dir, examples[6]),
                    '../res/moon-night.mp3', 'out/mp3-samples.raw'])
    print()

    print('7. 放大视频尺寸')
    subprocess.run([os.path.join(build_dir, examples[7]),
                    'out/scale-video.raw', 'svga'])
    print()

    print('8. 解码音视频')
    subprocess.run([os.path.join(build_dir, examples[8]),
                    '../res/big-buck-bunny.mp4', 'out/demux-decode-video.raw',
                    'out/demux-decode-audio.raw'])
    print()

    print('9. 视频filter处理')
    subprocess.run([os.path.join(build_dir, examples[9]),
                    '../res/big-buck-bunny.mp4'])
    print()

    print('10. 音频filter处理')
    subprocess.run([os.path.join(build_dir, examples[10]),
                    '../res/big-buck-bunny.mp4'])
    print()

    print('11. MP3格式转化为AAC格式')
    subprocess.run([os.path.join(build_dir, examples[11]),
                    '../res/moon-night.mp3'])
    print()
