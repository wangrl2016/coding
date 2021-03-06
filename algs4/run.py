#!/usr/bin/env python3
import os
import subprocess
import requests


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
        'BinarySearch',
        'Vector',
        'Date',
        'Point2D',
        'ResizingArrayStack',
        'LinkedListStack',
        'ResizingArrayQueue',
        'LinkedListQueue',
        'ResizingArrayBag',
        'LinkedListBag',
        'SeqSearchSymbolTable',
        'BinarySearchTree',
        'MaxPriorityQueue',
        'MinPriorityQueue',
        'BubbleSort',
        'HeapSort',
        'InsertionSort',
        'MergeSort',
        'QuickSort',
        'SelectionSort',
        'ShellSort',
        'RedBlackTree',
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

    class_path = build_dir + ':' + jar_paths

    for index, example in enumerate(reversed(examples)):
        no = len(examples) - index
        args = ['java', '--class-path', class_path, example]
        print(example)
        print(str(no) + '. ', end='')
        if no == 1:
            print('使用二分法在数组中查找')
            args.append('../res/int-allow-list.txt')
        elif no == 2:
            print('构建不可变的向量')
        elif no == 3:
            print('日期类')
        elif no == 4:
            print('二维点坐标')
            args += [str(54.0), str(154.0), str(122.0), str(64.0), str(160.0), str(198.0)]
        elif no == 5:
            print('使用数组实现可变大小的栈')
            args.append("to be or not to - be - - that - - - is")
        elif no == 6:
            print('使用单链表表示栈')
            args.append("to be or not to - be - - that - - - is")
        elif no == 7:
            print('使用数组实现可变大小的队列')
            args.append("to be or not to - be - - th at - - - is a qu es ti on "
                        "wh et - her - - it is")
        elif no == 8:
            print('使用单链表实现队列')
            args.append("to be or not to - be - - th at - - - is a qu es ti on "
                        "wh et - her - - it is")
        elif no == 9:
            print('使用数组实现背包')
            args.append("Hello world how are you")
        elif no == 10:
            print('使用链表实现背包')
            args.append("Hello world how are you")
        elif no == 11:
            print('使用单链表实现符号表')
            args.append("to be or not to - be - - that - - - is")
        elif no == 12:
            print('二分查找树')
            args.append('S E A R C H E X A M P L E')
        elif no == 13:
            print('使用堆实现最大优先队列')
            args.append('P Q E - X A M - P L E -')
        elif no == 14:
            print('使用堆实现最小优先队列')
            args.append('P Q E - X A M - P L E -')
        elif no == 15:
            print('冒泡排序')
            args.append('S O R T E X A M P L E')
        elif no == 16:
            print('堆排序')
            args.append('S O R T E X A M P L E')
        elif no == 17:
            print('插入排序')
            args.append('S O R T E X A M P L E')
        elif no == 18:
            print('归并排序')
            args.append('S O R T E X A M P L E')
        elif no == 19:
            print('快速排序')
            args.append('S O R T E X A M P L E')
        elif no == 20:
            print('选择排序')
            args.append('S O R T E X A M P L E')
        elif no == 21:
            print('希尔排序')
            args.append('S O R T E X A M P L E')
        elif no == 22:
            print('红黑树')

        subprocess.run(['javac', '-Xlint:unchecked', '-d', build_dir, '--class-path',
                        jar_paths, example + '.java'])
        subprocess.run(args)
        print()
