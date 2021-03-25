#!/usr/bin/env python3

import argparse
import os
import signal
import sys
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
    for p in info.packages:
        if phone_packages.__contains__(info.packages[p]):
            run_apps.append(p)


def run(device):
    # 获取手机的大小
    (w, h) = phone.get_size(device)
    # 滑动手机打开屏幕
    phone.swipe_down_to_up(device, w / 2, h)
    # 回到手机主页
    phone.go_home(device)

    # 代码测试位置

    while True:
        if datetime.now().hour != 0:
            utils.schedule_apps(device, w, h)

        while datetime.now().hour.__eq__(0):
            print('所有程序的签到工作 ' + datetime.now().time().__str__())
            for a in info.apps:
                if utils.is_coordinate_checkin(a):
                    getattr(checkin, a)(device, w, h)
                else:
                    getattr(checkin, a)(device)
                    # 所有程序的签到工作
                    getattr(sign, a)(device, w, h)
                    phone.stop_app(device, info.packages[a])

        while datetime.now().hour == 1:
            print()
            utils.tail_work(device, w, h, hour=1)

        while datetime.now().hour == 2:
            print()
            utils.tail_work(device, w, h, hour=2)

        while datetime.now().hour == 3:
            print()
            utils.tail_work(device, w, h, hour=3)

        while datetime.now().hour == 4:
            print()
            utils.tail_work(device, w, h, hour=4)

        while datetime.now().hour == 5:
            print()
            utils.tail_work(device, w, h, hour=5)

        while datetime.now().hour == 6:
            print()
            utils.tail_work(device, w, h, hour=6)

        while datetime.now().hour == 7:
            print()
            utils.tail_work(device, w, h, hour=7)

        while datetime.now().hour == 8:
            print()
            utils.tail_work(device, w, h, hour=8)

        while datetime.now().hour == 9:
            print()
            utils.tail_work(device, w, h, hour=9)

        while datetime.now().hour == 10:
            print()
            utils.tail_work(device, w, h, hour=10)

        while datetime.now().hour == 11:
            print()
            utils.tail_work(device, w, h, hour=11)

        while datetime.now().hour == 12:
            print()
            utils.tail_work(device, w, h, hour=12)

        while datetime.now().hour == 13:
            print()
            utils.tail_work(device, w, h, hour=13)

        while datetime.now().hour == 14:
            print()
            utils.tail_work(device, w, h, hour=14)

        while datetime.now().hour == 15:
            print()
            utils.tail_work(device, w, h, hour=15)

        while datetime.now().hour == 16:
            print()
            utils.tail_work(device, w, h, hour=16)

        while datetime.now().hour == 17:
            print()
            utils.tail_work(device, w, h, hour=17)

        while datetime.now().hour == 18:
            print()
            utils.tail_work(device, w, h, hour=18)

        while datetime.now().hour == 19:
            print()
            utils.tail_work(device, w, h, hour=19)

        while datetime.now().hour == 20:
            print()
            utils.tail_work(device, w, h, hour=20)

        while datetime.now().hour == 21:
            print()
            utils.tail_work(device, w, h, hour=21)

        while datetime.now().hour == 22:
            print()
            utils.tail_work(device, w, h, hour=22)

        while datetime.now().hour == 23:
            print()
            utils.tail_work(device, w, h, hour=23)


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
    info.apps = list(info.activities.keys())
    info.packages = utils.get_packages_dict(info.activities)

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

    # noinspection PyUnusedLocal
    def signal_handler(sig, frame):
        # 结束前关闭所有程序
        for d in devices:
            ats = phone.get_top_activities(d)
            if ats is None:
                sys.exit(0)
            for a in info.apps:
                if ats.__contains__(info.packages[a]):
                    print('关闭运行程序 ' + info.packages[a])
                    phone.stop_app(d, info.packages[a], 0.1)
        sys.exit(0)

    # 处理中断情况
    signal.signal(signal.SIGINT, signal_handler)
    signal.pause()

    # 等待所有线程退出
    for t in threads:
        t.join()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog='PROG', conflict_handler='resolve')
    parser.add_argument('-s', '--serial', help='phone serial number')

    main(parser.parse_args())
