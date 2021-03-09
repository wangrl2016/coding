#!/usr/bin/env python3

import argparse
import os
import threading

from src import utils, info, phone

MAX_PHOTOS_STORE = 50


def cycle(device):
    (w, h) = phone.get_size(device)


def run(device):
    (w, h) = phone.get_size(device)


def main(args):
    # 初始化out目录
    out_dir = 'out'
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)

    photos = utils.get_photos(out_dir)
    if len(photos) > MAX_PHOTOS_STORE:
        for p in photos:
            os.remove(p)

    # 初始化全局变量
    info.apps = list(info.activities_dict.keys())
    info.packages_dict = utils.get_packages_dict(info.activities_dict)

    # 获取设备号
    devices = []
    if args.serial:
        devices.append(args.serial)
    else:
        devices = phone.get_devices()

    print(devices)

    # 为每部手机设备创建单独的线程
    threads = []
    # 设备号对应的线程号(device:thread_id)
    pts_dict = {}
    for device in devices:
        info.contexts.update({device: {}})
        if info.high_serials.__contains__(device):
            t = threading.Thread(target=run, args=(device,), daemon=True)
            threads.append(t)
            pts_dict[device] = t.ident
            t.start()
        else:
            # 跑低配置手机
            t = threading.Thread(target=cycle, args=(device,), daemon=True)
            threads.append(t)
            pts_dict[device] = t.ident
            t.start()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog='PROG', conflict_handler='resolve')
    parser.add_argument('-s', '--serial', help='phone serial number')

    main(parser.parse_args())
