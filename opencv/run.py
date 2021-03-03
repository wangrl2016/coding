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
    if not os.path.exists('out'):
        os.mkdir('out')

    build_dir = 'build'

    subprocess.run([cmake_executable(), build_dir])
    subprocess.run(['make', '-C', build_dir])
    print()

    examples = [
        'DisplayImage',
        'ImageEnhance',
        'BlendImage'
    ]

    print('1. 显示图片')
    subprocess.run([os.path.join(build_dir, examples[0]),
                    '../res/sunflower-girl.png'])
    print()

    print('2. 图像增强')
    subprocess.run([os.path.join(build_dir, examples[1]),
                    '../res/sunflower-girl.png'])
    print()

    print('3. 两张图片混合')
    subprocess.run([os.path.join(build_dir, examples[2]),
                    '../res/foreign-girl.jpeg'])
