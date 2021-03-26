import inspect

from src import phone
from src.info import activities

# 对于无法通过Activity进行启动的图标需要定位
# 不同手机位置不同
# 设置成为4x6的图标布局模式

ROWS = [340, 600, 900, 1170, 1420, 1690]
COLUMNS = [160, 420, 660, 910]


def jingdong(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, COLUMNS[0], ROWS[1], gap)


def fanqie(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, COLUMNS[1], ROWS[1], gap)


def fanchang(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, COLUMNS[2], ROWS[1], gap)


def kuchang(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, COLUMNS[3], ROWS[1], gap)


def shuqi(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, COLUMNS[0], ROWS[2], gap)


def yingke(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, COLUMNS[1], ROWS[2], gap)


def kugou(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, COLUMNS[2], ROWS[2], gap)


def zhongqing(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, COLUMNS[3], ROWS[2], gap)


def kuaiyin(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, COLUMNS[0], ROWS[3], gap)


def kuge(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, COLUMNS[1], ROWS[3], gap)


def momo(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, COLUMNS[2], ROWS[3], gap)


def qingtuanshe(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, COLUMNS[3], ROWS[3], gap)


def eleme(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, COLUMNS[0], ROWS[4], gap)


def changdou(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, COLUMNS[1], ROWS[4], gap)


def kuaikandian(device, w, h, gap=10):
    phone.go_home(device)
    phone.tap(device, COLUMNS[2], ROWS[4], gap)


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


def diantao(device, gap=10):
    phone.start_app(device, activities[
        inspect.getframeinfo(inspect.currentframe()).function.__str__()], gap)


def huitoutiao(device, gap=10):
    phone.start_app(device, activities[
        inspect.getframeinfo(inspect.currentframe()).function.__str__()], gap)
