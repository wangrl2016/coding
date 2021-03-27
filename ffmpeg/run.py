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


def get_cmake_examples(path='CMakeLists.txt'):
    """
    获取CMakeLists.txt文件下面的EXAMPLES
    :return: 返回EXAMPLE链表
    """
    example_list = []
    if os.path.exists(path):
        with open(path) as file:
            get_flag = False
            while True:
                line = file.readline()

                for word in line.split():
                    if word.__eq__('EXAMPLES'):
                        get_flag = True
                        continue
                    if get_flag and word.__ne__(')'):
                        example_list.append(word)
                    if get_flag and word.__eq__(')'):
                        get_flag = False
                        break
                if not line:
                    break
    return example_list


if __name__ == '__main__':
    out_dir = 'out'
    build_dir = 'build'
    if not os.path.exists(out_dir):
        os.mkdir(out_dir)
    if not os.path.exists(build_dir):
        os.mkdir(build_dir)

    if not cmake_executable() or not ffmpeg_library():
        print('请手动安装CMake程序或者FFmpeg库')
        exit(0)

    subprocess.run([cmake_executable(), '-S', '.', '-B', build_dir])
    subprocess.run(['make', '-C', build_dir])

    examples = get_cmake_examples()

    for index, example in enumerate(reversed(examples)):
        exe = os.path.join(build_dir, example)
        args = [exe]
        no = len(examples) - index
        print(example)
        print(str(no) + '. ', end='')
        if no == 1:
            print('打印多媒体文件元数据')
            args.append('../res/big-buck-bunny.mp4')
        elif no == 2:
            print('打印音视频流信息')
            args.append('../res/big-buck-bunny.mp4')
        elif no == 3:
            print('解码视频为PPM图片')
            args.append('../res/big-buck-bunny.mp4')
        elif no == 4:
            print('构造YUV数据编码视频')
            args.append('out/yuv420p-encode.mp4')
        elif no == 5:
            print('构造RGBA数据编码视频')
            args.append('out/rgba-to-yuv420p.mp4')
        elif no == 6:
            print('编码MP3音频')
            args.append('out/16bit-dummy-audio.mp3')
        elif no == 7:
            print('解码MP3音频')
            args.append('../res/moon-night.mp3')
            args.append('out/mp3-samples.raw')
        elif no == 8:
            print('放大视频尺寸')
            args.append('out/scale-video.raw')
            args.append('svga')
        elif no == 9:
            print('解码音视频')
            args.append('../res/big-buck-bunny.mp4')
            args.append('out/demux-decode-video.raw')
            args.append('out/demux-decode-audio.raw')
        elif no == 10:
            print('视频filter处理')
            args.append('../res/big-buck-bunny.mp4')
        elif no == 11:
            print('音频filter处理')
            args.append('../res/big-buck-bunny.mp4')
        elif no == 12:
            print('MP3格式转码成为AAC格式')
            args.append('../res/moon-night.mp3')
            args.append('out/moon-night.mp4')
        elif no == 13:
            print('合成音视频')
            args.append('out/muxing-h264-aac.mp4')
        elif no == 14:
            print('容器转换(MP4->MOV)')
            args.append('../res/big-buck-bunny.mp4')
            args.append('out/big-buck-bunny.mov')
        elif no == 15:
            print('参考ffplay写的音视频播放器')
            args.append('--input')
            args.append('../res/big-buck-bunny.mp4')

        try:
            p = subprocess.run(args, check=True, stdout=subprocess.PIPE,
                               stderr=subprocess.STDOUT, universal_newlines=True)
            for ln in p.stdout.split('\n'):
                print(ln)
        except Exception as e:
            print(e)
            exit(1)
        # debug
        # break

    exit(0)
