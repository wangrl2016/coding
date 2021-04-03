from datetime import datetime
from random import randrange

from src import phone


def watch_video(device, w, h, num):
    print('看视频' + str(num) + '次 ' + datetime.now().time().__str__())
    for i in range(0, num):
        phone.swipe_down_to_up(device, w / 2, h, randrange(5, 16))


def read_article(device, w, h, num):
    print('看文章' + str(num) + '次 ' + datetime.now().time().__str__())
    for i in range(0, num):
        # 获取文章目录
        phone.swipe_down_to_up(device, w / 2, h, internal=300)
        # 点击文章
        phone.tap(device, w / 2, h / 2)
        # 滑动阅读
        for j in range(0, 10):
            phone.swipe_down_to_up(device, w / 2, h, randrange(2, 5), internal=300)
        # 返回上级目录
        phone.go_back(device)


def read_novel(device, w, h, num):
    print('看小说' + str(num) + '次 ' + datetime.now().time().__str__())
    for i in range(0, num):
        for j in range(0, 10):
            phone.swipe_right_to_left(device, w, h / 2, randrange(2, 5), internal=300)
    phone.go_back(device)


# ~~~~~~~~~~今日头条极速版~~~~~~~~~~

def toutiao_benefit_page(device, w, h, gap=3):
    # 点击福利
    phone.tap(device, w / 2, h - 80, gap)  # modify


# ~~~~~~~~~~快手极速版~~~~~~~~~~

def watch_kuaishou_video(device, w, h, hour):
    print('看快手视频 ' + datetime.now().time().__str__())
    while datetime.now().hour == hour:
        phone.swipe_down_to_up(device, w / 2, h, randrange(5, 16))


# ~~~~~~~~~~抖音极速版~~~~~~~~~~

def watch_douyin_video(device, w, h, hour):
    print('看抖音视频 ' + datetime.now().time().__str__())
    while datetime.now().hour == hour:
        phone.swipe_down_to_up(device, w / 2, h, randrange(5, 16))
