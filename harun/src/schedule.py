from datetime import datetime

from src import checkin, app, phone, info


def jingdong(device, w, h):
    return None


def fanqie(device, w, h):
    return None


def fanchang(device, w, h):
    return None


def kuchang(device, w, h):
    return None


def shuqi(device, w, h):
    return None


def yingke(device, w, h):
    return None


def kugou(device, w, h):
    return None


def zhongqing(device, w, h):
    return None


def kuaiyin(device, w, h):
    return None


def kuge(device, w, h):
    return None


def momo(device, w, h):
    return None


def qingtuanshe(device, w, h):
    return None


def eleme(device, w, h):
    return None


def changdou(device, w, h):
    return None


def kuaikandian(device, w, h):
    checkin.kuaikandian(device, w, h)
    app.read_article(device, w, h, num=1)
    phone.stop_app(device, info.packages['kuaikandian'])


def zhaoshang(device, w, h):
    return None


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
    def open_treasure():
        print('抖音极速版开宝箱 ' + datetime.now().time().__str__())
        phone.tap(device, w / 2, h - 100)  # modify
        phone.tap(device, w - 160, h - 200)  # modify

    checkin.douyin(device)
    # [x] 看抖音视频
    app.watch_video(device, w, h, num=10)
    # [x] 开宝箱
    open_treasure()
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
    return None


def uc(device, w, h):
    return None


def diantao(device, w, h):
    checkin.diantao(device)

    # [x] 点击进入直播页面
    phone.tap(device, w / 3, h / 3, gap=5)
    app.watch_video(device, w, h, num=10)

    phone.stop_app(device, info.packages['diantao'])


def huitoutiao(device, w, h):
    return None
