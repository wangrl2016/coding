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


def write_print(fd, log, new_line=True):
    """
    将log在控制台输出，并且写入到README.md文件中。
    """
    if new_line:
        log = log + '\n'
    print(log)
    fd.write(log)


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

    readme = open('README.md', 'w')
    # 清空文件
    readme.truncate()
    # 写入文件头
    readme.write('参考书籍\n\n')
    readme.write('C++ Concurrency in Action\n\n')
    readme.write('代码实现\n\n')
    readme.write('https://github.com/anthonywilliams/ccia_code_samples\n\n')

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
        'AccumulateParallel',
        'ProtectMutex',
        'PassReference'
    ]

    for index, example in enumerate(reversed(examples)):
        exe = os.path.join(build_dir, example)
        args = [exe]
        readme.write('* ' + example + '\n\n')
        no = len(examples) - index
        if no == 1:
            write_print(readme, 'A simple Hello Concurrent World program')
        elif no == 2:
            write_print(readme, 'A function that returns while a thread still has access to local variables')
        elif no == 3:
            write_print(readme, 'Waiting for a thread to finish')
        elif no == 4:
            write_print(readme, 'Using RAII to wait for a thread to complete')
        elif no == 5:
            write_print(readme, 'Detaching a thread to handle other documents')
        elif no == 6:
            write_print(readme, 'Returning a std::thread from a function')
        elif no == 7:
            write_print(readme, 'ScopedThread and example usage')
        elif no == 8:
            write_print(readme, 'Spawns some threads and waits for them to finish')
        elif no == 9:
            write_print(readme, 'A naive version of std::accumulate')
        elif no == 10:
            write_print(readme, 'Protecting a list with a mutex')
        elif no == 11:
            write_print(readme, 'Accidentally passing out a reference to protected data')

        p = subprocess.run(args, stdout=subprocess.PIPE,
                           universal_newlines=True)
        for line in p.stdout.split('\n'):
            if line.isspace() or line.startswith(' '):
                continue
            if line.isprintable():
                print(line)
        write_print(readme, '')
    readme.close()
    exit(0)
