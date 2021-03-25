import inspect

from src import phone
from src.info import activities


# 对于无法通过Activity进行启动的图标需要定位
# 不同手机位置不同
# 设置成为4x6的图标布局模式

def jingdong(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, w, h, gap)


def fanqie(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, w, h, gap)


def fanchang(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, w, h, gap)


def kuchang(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, w, h, gap)


def shuqi(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, w, h, gap)


def yingke(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, w, h, gap)


def kugou(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, w, h, gap)


def zhongqing(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, w, h, gap)


def kuaiyin(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, w, h, gap)


def kuge(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, w, h, gap)


def momo(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, w, h, gap)


def qingtuanshe(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, w, h, gap)


def eleme(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, w, h, gap)


def changdou(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, w, h, gap)


def zhaoshang(device, gap=10):
    phone.start_app(device, activities[
        inspect.getframeinfo(inspect.currentframe()).function.__str__()], gap)


def toutiao(device, gap=10):
    phone.start_app(device, activities[
        inspect.getframeinfo(inspect.currentframe()).function.__str__()], gap)


def kuaishou(device, gap=10):
    phone.start_app(device, activities[
        inspect.getframeinfo(inspect.currentframe()).function.__str__()], gap)


def douyin(device, gap=10):
    phone.start_app(device, activities[
        inspect.getframeinfo(inspect.currentframe()).function.__str__()], gap)


def qutoutiao(device, gap=10):
    phone.start_app(device, activities[
        inspect.getframeinfo(inspect.currentframe()).function.__str__()], gap)


def baidu(device, gap=10):
    phone.start_app(device, activities[
        inspect.getframeinfo(inspect.currentframe()).function.__str__()], gap)


def weishi(device, gap=10):
    phone.start_app(device, activities[
        inspect.getframeinfo(inspect.currentframe()).function.__str__()], gap)


def douhuo(device, gap=10):
    phone.start_app(device, activities[
        inspect.getframeinfo(inspect.currentframe()).function.__str__()], gap)


def chejia(device, gap=10):
    phone.start_app(device, activities[
        inspect.getframeinfo(inspect.currentframe()).function.__str__()], gap)


def uc(device, gap=10):
    phone.start_app(device, activities[
        inspect.getframeinfo(inspect.currentframe()).function.__str__()], gap)


def kuaikandian(device, gap=10):
    phone.start_app(device, activities[
        inspect.getframeinfo(inspect.currentframe()).function.__str__()], gap)
