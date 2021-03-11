#!/usr/bin/env python3

import argparse
import os
import threading
from datetime import datetime

from src import utils, info, phone, checkin, sign

MAX_PHOTOS_STORE = 50


def cycle(device):
    # 需要手机保持亮屏状态
    # 获取手机像素大小
    (w, h) = phone.get_size(device)

    # 滑动手机打开屏幕
    phone.swipe_down_to_up(device, w / 2, h)

    phone.go_home(device)

    phone_packages = phone.list_packages(device)
    run_apps = []
    for p in info.packages_dict:
        if phone_packages.__contains__(info.packages_dict[p]):
            run_apps.append(p)


def run(device):
    # 获取手机的大小
    (w, h) = phone.get_size(device)
    # 滑动手机打开屏幕
    phone.swipe_down_to_up(device, w / 2, h)
    # 回到手机主页
    phone.go_home(device)

    properties = phone.get_device_properties(device)
    for p in properties:
        if p.__contains__('vivo'):
            for line in properties:
                if line.__contains__('ro.vivo.market.name'):
                    info.contexts[device]['phone_name'] = 'vivo'
                    print(line)
        if p.__contains__('Xiaomi'):
            for line in properties:
                if line.__contains__('ro.product.model'):
                    print(line)

    while True:
        while datetime.now().hour.__eq__(0):
            print('所有程序的签到工作 ' + datetime.now().__str__())
            for a in info.apps:
                if utils.is_coordinate_checkin(a):
                    getattr(checkin, a)(device, w, h)
                else:
                    getattr(checkin, a)(device)
                    # 所有程序的签到工作
                    getattr(sign, a)(device, w, h)
                    phone.stop_app(device, info.packages_dict[a])

        while datetime.now().hour == 1:
            print('今日头条的工作内容')


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
