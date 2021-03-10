#!/usr/bin/env python3

import os
import subprocess


def cmake_executable():
    """
    查找CMake可执行程序是否安装

    :return: A string suitable for passing to subprocess functions or None
    """

    with open(os.devnull, 'w') as devnull:
        try:
            subprocess.call(['cmake', '--version'], stdout=devnull)
        except (OSError,):
            print('CMake没有安装')
            return None
        return 'cmake'


def ffmpeg_library():
    """
    检查ffmpeg是否安装

    :return: A string suitable for passing to subprocess functions or None
    """
    with open(os.devnull, 'w') as devnull:
        try:
            subprocess.call(['ffmpeg', '-version'], stdout=devnull)
        except (OSError,):
            print('FFmpeg没有安装')
            return None
        return 'ffmpeg'


if __name__ == '__main__':
    out_dir = 'out'
    build_dir = 'build'
    if not os.path.exists(out_dir):
        os.mkdir(out_dir)
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    if not cmake_executable() or not ffmpeg_library():
        print('请手动安装CMake程序或者FFmpeg库')
        exit(0)

    subprocess.run([cmake_executable(), build_dir])
    subprocess.run(['make', '-C', build_dir])

    # examples = ['Metadata', 'Dump', 'DecodeVideo',
    #             'EncodeVideo', 'RGBA2Video',
    #             'EncodeAudio', 'DecodeAudio',
    #             'ScaleVideo', 'DemuxDecode',
    #             'FilteringVideo', 'FilteringAudio',
    #             'TranscodeAAC', 'Muxing',
    #             'Remuxing'
    #             ]

    examples = ['Metadata',
                'Dump',
                'DecodeVideo']

    for index, example in enumerate(reversed(examples)):
        exe = os.path.join(build_dir, example)
        args = [exe]

        if index == len(examples) - 1:
            print('打印多媒体文件元数据')
            args.append('../res/big-buck-bunny.mp4')
        elif index == len(examples) - 2:
            print('打印音视频流信息')
            args.append('../res/big-buck-bunny.mp4')
        elif index == len(examples) - 3:
            print('解码视频为PPM图片')
            args.append('../res/big-buck-bunny.mp4')

        subprocess.run(args)
        print()

    exit(0)

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

    print('12. 合成视频')
    subprocess.run([os.path.join(build_dir, examples[12]),
                    'out/muxing-audio-video.mp4'])
    print()

    print('13. 转换容器')
    subprocess.run([os.path.join(build_dir, examples[13]),
                    '../res/big-buck-bunny.mp4', '../out/big-buck-bunny.mov'])
