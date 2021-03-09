import subprocess
import time

"""
The commands and default sources are:
    text <string> (Default: touchscreen)
    keyevent [--longpress] <key code number or name> ... (Default: keyboard)
    tap <x> <y> (Default: touchscreen)
    swipe <x1> <y1> <x2> <y2> [duration(ms)] (Default: touchscreen)
    draganddrop <x1> <y1> <x2> <y2> [duration(ms)] (Default: touchscreen)
"""


def text(device, txt, gap=3):
    """
    输入文字
    :param device: 手机标识符
    :param txt: 想要输入的文本
    :param gap: 时间间隔
    """
    subprocess.run(['adb', '-s', device, 'shell', 'input', 'text', txt])
    time.sleep(gap)


def keyevent(device, event, gap=3):
    """
    输入按键事件
    https://developer.android.com/reference/android/view/KeyEvent
    :param device: 手机标识符
    :param event: 事件
    :param gap: 时间间隔
    """
    try:
        subprocess.run(['adb', '-s', device, 'shell', 'input', 'keyevent', event])
    except Exception as e:
        print(e)
        return None
    time.sleep(gap)


def tap(device, w, h, gap=5):
    """
    点击手机像素坐标点
    :param device: 手机标识符
    :param w: 水平方向像素坐标
    :param h: 竖直方向像素坐标
    :param gap: 时间间隔
    """
    subprocess.run(['adb', '-s', device, 'shell', 'input', 'tap', str(int(w)), str(int(h))])
    time.sleep(gap)


def swipe(device, w1, h1, w2, h2, internal=200, gap=3):
    """
    滑动屏幕
    :param device: 手机标识符
    :param w1: 第一个点水平方向像素坐标
    :param h1: 第一个点竖直防线像素坐标
    :param w2: 第二个点水平方向像素坐标
    :param h2: 第二个点竖直方向像素坐标
    :param internal: 两个点之间的滑动间隔时间
    :param gap: 时间间隔
    """
    subprocess.run(['adb', '-s', device, 'shell', 'input', 'swipe',
                    str(int(w1)), str(int(h1)),
                    str(int(w2)), str(int(h2)), str(internal)])
    time.sleep(gap)
