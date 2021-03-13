#!/usr/bin/env python3

import argparse
from random import randrange

from src import phone

"""
执行单一的刷短视频操作
"""


def main(args):
    # 获取连接电脑的设备
    devices = phone.get_devices()
    print(devices)
    goon = False
    for device in devices:
        if device.__eq__(args.serial):
            goon = True

    if not goon:
        print('Wrong serial number or not connect to computer')
        exit(0)

    (w, h) = phone.get_size(str(args.serial))

    while True:
        phone.swipe_down_to_up(str(args.serial), w / 2 + randrange(-15, 15),
                               h + randrange(-15, 15), randrange(5, 16))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog='PROG', conflict_handler='resolve')
    parser.add_argument('-s', '--serial', help='phone serial number')

    main(parser.parse_args())
