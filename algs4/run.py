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
    """
    下载程序需要的包依赖包
    """
    print('Download ' + path)
    dst = os.path.join(dst + path.rsplit('/')[-1])
    r = requests.get(url, stream=True)
    with open(dst, 'wb') as dst_file:
        for chunk in r.iter_content(chunk_size=1024):
            if chunk:
                dst_file.write(chunk)


if __name__ == '__main__':
    build_dir = 'build'
    out_dir = 'out'
    if not os.path.exists(build_dir):
        os.mkdir(build_dir)
    if not os.path.exists(out_dir):
        os.mkdir(out_dir)

    if not java_check():
        print('请手动安装java库')
        exit(0)

    third_jar_paths = [
        'https://algs4.cs.princeton.edu/code/algs4.jar'
    ]

    third_jars = []

    for path in third_jar_paths:
        filename = path.rsplit('/')[-1]
        third_jars.append(filename)
        if not os.path.exists(filename):
            download_third_jar(path)

    examples = [
        'InsertionSort',
        'MergeSort',
        'PriorityElementary',
        'Queue',
        'QueueArray',
        'QuickSort',
        'QuickWithCutoff',
        'QuickWithSentinels',
        'SelectionSort',
        'ShellSort',
        'Stack',
        'StackArray',
        'MaxPriorityQueue',
        'MinPriorityQueue'
    ]

    jar_paths = ''
    # 第一个路径前面不加:符号
    first = True
    for jar in third_jars:
        if first:
            first = False
            jar_paths += jar
        else:
            jar_paths += ':' + jar

    for index, example in enumerate(reversed(examples)):
        no = len(examples) - index
        if no == 1:
            subprocess.run(['javac', '-d', build_dir, '--class-path',
                            jar_paths, example + '.java'])
            class_path = build_dir + ':' + jar_paths
            subprocess.run(['java', '--class-path', class_path, example])
        else:
            print('Error')
