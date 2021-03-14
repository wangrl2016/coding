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
        'HelloWindow',
        'HelloTriangle',
        'TwoTriangle',
        'HelloRectangle',
        'ColorTriangle',
        'DrawCircle',
        'HelloTransform',
        'HelloTexture',
        'DrawSphere',
        'CoordinateSystem'
    ]

    for index, example in enumerate(reversed(examples)):
        exe = os.path.join(build_dir, example)
        args = [exe]

        # 按ESC键进入下一个项目
        if index == len(examples) - 1:
            print('窗口输出')
        elif index == len(examples) - 2:
            print('绘制三角形')
        elif index == len(examples) - 3:
            print('绘制两个三角形')
        elif index == len(examples) - 4:
            print('绘制长方形')
        elif index == len(examples) - 5:
            print('绘制彩色三角形')
        elif index == len(examples) - 6:
            print('绘制圆形')
        elif index == len(examples) - 7:
            print('变换')
            args.append('../res/foreign-girl.jpeg')
            args.append('../res/cow-toy.png')
        elif index == len(examples) - 8:
            print('绘制纹理')
            args.append('../res/foreign-girl.jpeg')
            args.append('../res/cow-toy.png')
        elif index == len(examples) - 9:
            print('绘制圆球')
            args.append('../res/foreign-girl.jpeg')
            args.append('../res/cow-toy.png')
        elif index == len(examples) - 10:
            print('坐标变换')
            args.append('../res/foreign-girl.jpeg')
            args.append('../res/cow-toy.png')

        subprocess.run(args)
        print()

    exit(0)
