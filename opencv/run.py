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

    subprocess.run([cmake_executable(), build_dir])
    subprocess.run(['make', '-C', build_dir])

    examples = [
        'DisplayImage',
        'ImageEnhance',
        'DrawContours',
        'BlendMode'
    ]

    for index, example in enumerate(reversed(examples)):
        exe = os.path.join(build_dir, example)
        args = [exe]

        if index == len(examples) - 1:
            print('显示图片')
            args.append('../res/sunflower-girl.png')
        elif index == len(examples) - 2:
            print('图像增强')
            args.append('../res/sunflower-girl.png')
            args.append('out/image-enhance0.png')
            args.append('out/image-enhance1.png')
        elif index == len(examples) - 3:
            print('绘制等高线')
            args.append('../res/sunflower-girl.png')
            args.append('out/alpha-binary.png')
            args.append('out/draw-contours.png')
        elif index == len(examples) - 4:
            print('混合模式')
            args.append('../res/model-girl-warm.jpeg')
            args.append('../res/model-girl-cold.jpeg')

        subprocess.run(args)
        print()

    exit(0)
