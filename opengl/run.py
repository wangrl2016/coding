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
        'HelloWindow',
        'HelloTriangle',
        'TwoTriangle',
        'HelloRectangle',
        'ColorTriangle',
        'DrawCircle',
        'HelloTransform',
        'HelloTexture',
        'DrawSphere',
        'CoordinateSystem',
        'CameraView'
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
        elif index == len(examples) - 11:
            print('相机视角')
        elif index == len(examples) - 12:
            print('镜面光')

        subprocess.run(args)
        print()

    exit(0)
