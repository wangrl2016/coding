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

    if cmake_executable() is None:
        exit(0)

    build_dir = 'build'

    subprocess.run([cmake_executable(), build_dir])
    subprocess.run(['make', '-C', build_dir])
    print()

