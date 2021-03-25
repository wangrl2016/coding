from datetime import datetime

from src import checkin, app, phone, info


def jingdong(device, w, h):
    print()


def fanqie(device, w, h):
    print()


def fanchang(device, w, h):
    print()


def kuchang(device, w, h):
    print()


def shuqi(device, w, h):
    print()


def yingke(device, w, h):
    print()


def kugou(device, w, h):
    print()


def zhongqing(device, w, h):
    print()


def kuaiyin(device, w, h):
    print()


def kuge(device, w, h):
    print()


def momo(device, w, h):
    print()


def qingtuanshe(device, w, h):
    print()


def eleme(device, w, h):
    print()


def changdou(device, w, h):
    print()


def zhaoshang(device, w, h):
    print()


def toutiao(device, w, h):
    def open_treasure():
        print('今日头条开宝箱 ' + datetime.now().time().__str__())

    checkin.toutiao(device)
    # [x] 阅读头条文章
    app.read_article(device, w, h, num=1)
    phone.stop_app(device, info.packages['toutiao'])


def kuaishou(device, w, h):
    checkin.kuaishou(device)
    # [x] 看快手视频
    app.watch_video(device, w, h, num=10)
    phone.stop_app(device, info.packages['kuaishou'])


def douyin(device, w, h):
    checkin.douyin(device)
    # [x] 看抖音视频
    app.watch_video(device, w, h, num=10)
    phone.stop_app(device, info.packages['douyin'])


def qutoutiao(device, w, h):
    checkin.qutoutiao(device)
    # [x] 阅读趣头条文章
    app.read_article(device, w, h, num=1)
    phone.stop_app(device, info.packages['qutoutiao'])


def baidu(device, w, h):
    checkin.baidu(device)
    # [x] 阅读百度文章
    app.read_article(device, w, h, num=1)
    phone.stop_app(device, info.packages['baidu'])


def weishi(device, w, h):
    checkin.weishi(device)
    # [x] 看微视视频
    app.watch_video(device, w, h, num=10)
    phone.stop_app(device, info.packages['weishi'])


def douhuo(device, w, h):
    checkin.douhuo(device)
    app.watch_video(device, w, h, num=10)
    phone.stop_app(device, info.packages['douhuo'])


def chejia(device, w, h):
    print()


def uc(device, w, h):
    print()


def kuaikandian(device, w, h):
    checkin.kuaikandian(device)
    app.read_article(device, w, h, num=1)
    phone.stop_app(device, info.packages['kuaikandian'])
