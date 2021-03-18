#!/user/bin/env python3
import os
import subprocess
import sys


def main(argv):
    # 检查resvg是否存在
    resvg_dir = 'resvg'
    if not os.path.exists(resvg_dir):
        print('请执行git_syn_deps.py脚本下载resvg库')
        exit(0)

    # 对resvg文件进行修改

    # 编译运行
    subprocess.run(['cargo', 'build', '--release'])
    subprocess.run(['./target/release/svg2gif', '-i', '../res/base64-gif.svg',
                    '-o', 'out.gif', '-f', 'gif'])


if __name__ == '__main__':
    main(sys.argv[1:])
