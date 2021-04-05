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

    for index, example in enumerate(reversed(examples)):
        exe = os.path.join(build_dir, example)
        args = [exe]
        no = len(examples) - index
        print(example)
        print(str(no) + '. ', end='')
        if no == 1:
            print('解码视频为PPM图片')
            args.append('../res/big-buck-bunny.mp4')
        elif no == 2:
            print('构造YUV数据编码视频')
            args.append('out/yuv420p-encode.mp4')
        elif no == 3:
            print('编码MP3音频')
            args.append('out/16bit-dummy-audio.mp3')
        elif no == 4:
            print('解码MP3音频')
            args.append('../res/moon-night.mp3')
            args.append('out/mp3-samples.raw')
        elif no == 5:
            print('放大视频尺寸')
            args.append('out/scale-video.raw')
            args.append('svga')
        elif no == 6:
            print('解码音视频')
            args.append('../res/big-buck-bunny.mp4')
            args.append('out/demux-decode-video.raw')
            args.append('out/demux-decode-audio.raw')
        elif no == 7:
            print('视频filter处理')
            args.append('../res/big-buck-bunny.mp4')
        elif no == 8:
            print('音频filter处理')
            args.append('../res/big-buck-bunny.mp4')
            args.append('out/mono-s16le-8000.pcm')
        elif no == 9:
            print('MP3格式转码成为AAC格式')
            args.append('../res/moon-night.mp3')
            args.append('out/moon-night.mp4')
        elif no == 10:
            print('合成音视频')
            args.append('out/muxing-h264-aac.mp4')
        elif no == 11:
            print('容器转换(MP4->MOV)')
            args.append('../res/big-buck-bunny.mp4')
            args.append('out/big-buck-bunny.mov')
        elif no == 12:
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

    exit(0)
