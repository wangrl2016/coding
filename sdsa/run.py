#!/usr/bin/env python3
import os
import subprocess
import requests


def write_print(fd, log, new_line=True):
    """
    将log在控制台输出，并且写入到README.md文件中。
    """
    if new_line:
        log = log + '\n'
    print(log)
    fd.write(log)


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

    readme = open('README.md', 'w')
    # 清空文件
    readme.truncate()

    third_jar_paths = [
    ]

    third_jars = []

    for path in third_jar_paths:
        filename = path.rsplit('/')[-1]
        third_jars.append(filename)
        if not os.path.exists(filename):
            download_third_jar(path)

    examples = [
        'array/HelloWorld',
        'array/ArrayIntroduction',
        'array/ArrayInJava',
        'array/Student',
        'array/ArrayCopy',
        'array/GCD',
        'array/ArrayRotate',
        'array/ArrayReverse',
        'array/ArrayRightRotate',
        'array/ArraySearch',
        'linkedlist/SinglyLinkedList',
        'linkedlist/SinglyLinkedListDeleted',
        'linkedlist/SinglyLinkedListInserted',
        'linkedlist/SinglyListLength',
        'linkedlist/DoubleLinkedList',
        'linkedlist/DoubleListDeleted',
        'linkedlist/CircularLinkedList',
        'linkedlist/IsCircularList',
        'stack/ArrayStack',
        'stack/LinkedListArray',
        'queue/ArrayQueue',
        'queue/LinkedListQueue',
        'binarytree/BinaryTree',
        'binarytree/BinaryTreeIntro',
        'binarytree/BinarySearchTree',
        'binarytree/BinaryTreeInsert',
        'binarytree/BinaryTreeTraversal',
        'heap/MinHeap',
        'hashing/HashMap',
        'graph/GraphRepresent',
        'matrix/MatrixRotate',
        'greedy/HuffmanCoding'
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

        readme.write('* ' + example + '\n\n')

        if no == 1:
            write_print(readme, '进入Java编程环境')
        elif no == 2:
            write_print(readme, '数组介绍')
        elif no == 3:
            write_print(readme, 'Java语言中数组的使用')
        elif no == 4:
            write_print(readme, '创建学生数组')
        elif no == 5:
            write_print(readme, '数组复制')
        elif no == 6:
            write_print(readme, '求最大公约数')
        elif no == 7:
            write_print(readme, '数组旋转')
        elif no == 8:
            write_print(readme, '数组倒置')
        elif no == 9:
            write_print(readme, '数组向右旋转')
        elif no == 10:
            write_print(readme, '查找数组')

        subprocess.run(['javac', '-d', build_dir, '--class-path',
                        jar_paths, example + '.java'])
        class_path = build_dir + ':' + jar_paths
        p = subprocess.run(['java', '--class-path', class_path, example], stdout=subprocess.PIPE,
                           universal_newlines=True)
        for line in p.stdout.split('\n'):
            if line.isspace() or line.startswith(' '):
                continue
            if line.isprintable():
                print(line)

        write_print(readme, '')

    readme.close()
