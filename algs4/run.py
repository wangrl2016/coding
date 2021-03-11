#!/usr/bin/env python3
import os
import subprocess
import requests


def java_check():
    """
    检查计算机中是否安装java
    """
    with open(os.devnull, 'w') as devnull:
        try:
            subprocess.call(['java', '--version'], stdout=devnull)
        except (OSError,):
            return None
    return 'java'


def download_third_jar(url, dst='./'):
    print('Download ' + path)
    dst = os.path.join(dst + path.rsplit('/')[-1])
    r = requests.get(url, stream=True)
    with open(dst, 'wb') as dst_file:
        for chunk in r.iter_content(chunk_size=1024):
            if chunk:
                dst_file.write(chunk)


if __name__ == '__main__':
    out_dir = 'out'
    if not os.path.exists(out_dir):
        os.mkdir(out_dir)

    if not java_check():
        print('请手动安装java库')
        exit(0)

    third_jar_paths = [
        'https://algs4.cs.princeton.edu/code/algs4.jar'
    ]

    for path in third_jar_paths:
        filename = path.rsplit('/')[-1]
        if not os.path.exists(filename):
            download_third_jar(path)
