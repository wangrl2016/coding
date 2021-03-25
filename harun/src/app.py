from datetime import datetime
from random import randrange

from src import phone


def read_toutiao_article(device, w, h, num):
    print('阅读今日头条文章 ' + datetime.now.__str__())
    for i in range(0, num):
        # 获取文章目录
        phone.swipe_up_to_down(device, w / 2, h)
        # 点击文章
        phone.tap(device, w / 2, h / 2)
        # 滑动阅读
        for j in range(0, 10):
            phone.swipe_down_to_up(device, w / 2, h, randrange(2, 5))
        # 返回上级目录
        phone.go_back(device)
