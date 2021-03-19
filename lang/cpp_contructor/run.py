#!/usr/bin/env python3
import os
import subprocess


def cmake_executable():
    with open(os.devnull, 'w') as devnull:
        try:
            subprocess.call(['cmake', '--version'], stdout=devnull)
        except (OSError,):
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
        print('请手动安装CMake程序')
        exit(0)

    subprocess.run([cmake_executable(), '-S', '.', '-B', build_dir])
    subprocess.run(['make', '-C', build_dir])

    examples = []
    # 读取CMakeLists.txt文件里面的EXAMPLES变量进行初始化
    cmake_filename = 'CMakeLists.txt'
    if os.path.exists(cmake_filename):
        with open(cmake_filename) as file:
            get_flag = False
            while True:
                line = file.readline()

                for word in line.split():
                    if word.__eq__('EXAMPLES'):
                        get_flag = True
                        continue
                    if get_flag and word.__ne__(')'):
                        examples.append(word)
                    if get_flag and word.__eq__(')'):
                        get_flag = False
                        break
                if not line:
                    break

    for index, example in enumerate(reversed(examples)):
        exe = os.path.join(build_dir, example)
        args = [exe]

        no = len(examples) - index
        if no == 1:
            print("Cpp constructor")

        subprocess.run(args)
        print()

    exit(0)
