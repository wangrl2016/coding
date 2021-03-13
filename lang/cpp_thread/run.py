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

    examples = [
        'HelloConcurrent',
        'AccessLocalVariable',
        'WaitingThreadFinish',
        'RAII',
        'DetachThread',
        'ReturnThread',
        'ScopedThread',
        'VectorThread',
        'AccumulateParallel'
    ]

    for index, example in enumerate(reversed(examples)):
        exe = os.path.join(build_dir, example)
        args = [exe]

        no = len(examples) - index
        if no == 1:
            print('A simple Hello Concurrent World program')
        elif no == 2:
            print('A function that returns while a thread still has access to local variables')
        elif no == 3:
            print('Waiting for a thread to finish')
        elif no == 4:
            print('Using RAII to wait for a thread to complete')
        elif no == 5:
            print('Detaching a thread to handle other documents')
        elif no == 6:
            print('Returning a std::thread from a function')
        elif no == 7:
            print('ScopedThread and example usage')
        elif no == 8:
            print('Spawns some threads and waits for them to finish')
        elif no == 9:
            print('A naive version of std::accumulate')

        subprocess.run(args)
        print()

    exit(0)
