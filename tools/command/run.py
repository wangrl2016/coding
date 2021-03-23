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

    exe = os.path.join(build_dir, 'CommandTest')

    # 对CommandLine库进行测试输出预期结果
    subprocess.run([exe, '-h'])
