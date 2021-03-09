import os
import subprocess
import time
from datetime import datetime
from random import randrange
from src import input


def go_home(pid, gap=1):
    """
    回到手机主页
    """
    input.keyevent(pid, 'KEYCODE_HOME', gap)


def go_back(pid, times=1, gap=2):
    """
    回退到上级页面
    """
    for i in range(0, times):
        input.keyevent(pid, 'KEYCODE_BACK', gap)


def tap(pid, w, h, gap=5):
    """
    点击屏幕
    """
    w = w + randrange(-15, 15)
    h = h + randrange(-5, 5)
    input.tap(pid, w, h, gap)


def swipe_down_to_up(pid, w, h, gap=3, internal=200):
    """
    从下往上滑动屏幕
    """
    w = w + randrange(-25, 25)
    h = h + randrange(-50, 50)
    internal = internal + randrange(-30, 30)
    subprocess.run(['adb', '-s', pid, 'shell', 'input', 'swipe',
                    str(int(w)), str(int(h * 3 / 4)),
                    str(int(w)), str(int(h * 1 / 4)),
                    str(internal)])
    time.sleep(gap)


def swipe_up_to_down(pid, w, h, gap=3, internal=200):
    """
    从上往下滑动屏幕
    """
    w = w + randrange(-25, 25)
    h = h + randrange(-50, 50)
    internal = internal + randrange(-30, 30)
    subprocess.call(['adb', '-s', pid, 'shell', 'input', 'swipe',
                     str(int(w)), str(int(h * 1 / 4)),
                     str(int(w)), str(int(h * 3 / 4)),
                     str(internal)])
    time.sleep(gap)


def swipe_right_to_left(pid, w, h, gap=3, internal=200):
    """
    从右往左滑动屏幕
    """
    w = w + randrange(-25, 25)
    h = h + randrange(-50, 50)
    internal = internal + randrange(-30, 30)
    subprocess.call(['adb', '-s', pid, 'shell', 'input', 'swipe',
                     str(int(w * 3 / 4)), str(int(h)),
                     str(int(w * 1 / 4)), str(int(h)),
                     str(internal)])
    time.sleep(gap)


def swipe_left_to_right(pid, w, h, gap=3, internal=200):
    """
    从右往左滑动屏幕
    """
    w = w + randrange(-25, 25)
    h = h + randrange(-50, 50)
    internal = internal + randrange(-30, 30)
    subprocess.call(['adb', '-s', pid, 'shell', 'input', 'swipe',
                     str(int(w * 1 / 4)), str(int(h)),
                     str(int(w * 3 / 4)), str(int(h)),
                     str(internal)])
    time.sleep(gap)


def screenshot(pid, gap=1):
    """
    截取手机屏幕
    :return:截取图片文件名称
    """
    filename = '/sdcard/' + datetime.now().__str__() + '.png'
    subprocess.run(['adb', '-s', pid, 'shell', 'screencap', '-p', filename])
    time.sleep(gap)
    return filename


def get_screenshot(pid, filename, output=".", gap=1):
    """
    获取手机上的截图
    """
    subprocess.run(['adb', '-s', pid, 'pull', filename, output])
    time.sleep(gap)


def remove_screenshot(pid, filename, gap=1):
    """
    删除手机上的截图
    """
    subprocess.run(['adb', '-s', pid, 'shell', 'rm', filename])
    time.sleep(gap)


def get_page_photo(pid, output):
    """
    获取手机的页面
    """
    filename = datetime.now().date().__str__() + '_' + datetime.now().time().__str__() + '.png'
    path = os.path.join('/sdcard', filename)
    subprocess.run(['adb', '-s', pid, 'shell', 'screencap', '-p', path])
    try:
        subprocess.run(['adb', '-s', pid, 'pull', path, output],
                       check=True, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, universal_newlines=True)
    except Exception as e:
        print('获取照片失败 ' + str(e))
        return None
    subprocess.run(['adb', '-s', pid, 'shell', 'rm', path])
    return filename


def stop_app(pid, package, gap=1):
    """
    关闭程序
    """
    subprocess.run(['adb', '-s', pid, 'shell', 'am', 'force-stop', package])
    time.sleep(gap)


def start_app(pid, activity, gap):
    """
    启动程序
    """
    try:
        subprocess.run(['adb', '-s', pid, 'shell', 'am', 'start', '-n', activity],
                       check=True, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, universal_newlines=True)
    except Exception as e:
        print('启动程序失败 ' + str(e))
        return None
    time.sleep(gap)


def list_packages(pid):
    """
    获取程序包名
    :return: 包含包名的字符串
    """
    p = subprocess.run(['adb', '-s', pid, 'shell', 'pm', 'list', 'package'],
                       check=True, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, universal_newlines=True)
    return p.stdout


def display_on(pid):
    """
    屏幕是否处于唤醒状态
    默认处于唤醒状态
    """
    try:
        p = subprocess.run(['adb', '-s', pid, 'shell', 'dumpsys', 'power'],
                           check=True, stdout=subprocess.PIPE,
                           stderr=subprocess.STDOUT, universal_newlines=True)
    except Exception as e:
        print(e)
        return True
    for line in p.stdout.split('\n'):
        if line.__contains__('Display Power'):
            if line.__contains__('OFF'):
                return False
    return True


def wakeup(pid, gap=3):
    """
    和KEYCODE_POWER表现一致
    但是如果屏幕是唤醒状态该操作没有效果
    """
    print('唤醒屏幕 ' + pid + ' ' + datetime.now().__str__())
    subprocess.run(['adb', '-s', pid, 'shell', 'input', 'keyevent', 'KEYCODE_WAKEUP'])
    time.sleep(gap)


def sleep(pid):
    """
    和KEYCODE_POWER表现一致
    但是如果屏幕是熄灭状态该操作没有效果
    """
    print("关闭屏幕 " + datetime.now().__str__())
    subprocess.run(['adb', '-s', pid, 'shell', 'input', 'keyevent', 'KEYCODE_SLEEP'])


def power(pid, gap=3):
    """
    关闭或者点亮屏幕
    """
    subprocess.run(['adb', '-s', pid, 'shell', 'input', 'keyevent', 'KEYCODE_POWER'])
    time.sleep(gap)


def get_top_activities(pid):
    """
    获取最上面的activity名称
    :return: activity名称
    """
    try:
        p = subprocess.run(['adb', '-s', pid, 'shell', 'dumpsys', 'activity', 'top'],
                           check=True, stdout=subprocess.PIPE,
                           stderr=subprocess.STDOUT, universal_newlines=True)
    except Exception as e:
        print(e)
        return None
    return p.stdout


def sleep_to_weak(pid, w, h, gap=300):
    """
    手机休息到唤醒
    """
    sleep(pid)
    time.sleep(gap)
    wakeup(pid, 3)
    if not display_on(pid):
        print("尝试再次唤醒手机 " + datetime.now().hour.__str__())
        wakeup(pid, 3)
    swipe_down_to_up(pid, w / 2, h, internal=100)


def get_dpi(pid):
    """
    获取屏幕密度
    """
    p = subprocess.run(['adb', '-s', pid, 'shell', 'wm', 'density'], check=True, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, universal_newlines=True)
    dpi_str = p.stdout.strip('\n')
    return int(dpi_str.split(' ')[-1])


def reboot(devices):
    """
    重启设备
    """
    for pid in devices:
        subprocess.run(['adb', '-s', pid, 'reboot'])
        time.sleep(60)


def get_device_properties(pid):
    """
    获取手机的属性
    """
    properties = []
    p = subprocess.run(['adb', '-s', pid, 'shell', 'getprop'], check=True, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, universal_newlines=True)
    for line in p.stdout.split('\n'):
        line.strip('\n')
        properties.append(line)
    return properties


def get_size(device):
    """
    获取手机的屏幕尺寸

    :param device: 手机唯一标识
    :return: 手机屏幕尺寸
    """
    p = subprocess.run(['adb', '-s', device, 'shell', 'wm', 'size'], check=True,
                       stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, universal_newlines=True)
    size_str = p.stdout.strip('\n')
    for s in size_str.split(' '):
        if s.rfind('x') > 0:
            return int(s.split('x')[0]), int(s.split('x')[1])


def get_devices():
    """
    获取连接电脑的手机设备号

    可能的输出信息
    List of devices attached
    * daemon not running; starting now at tcp:5037
    * daemon started successfully
    ce7f96a00307	device

    :return: 手机设备号
    """
    p = subprocess.run(['adb', 'devices'], check=True, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, universal_newlines=True)
    devices = []
    for line in p.stdout.split('\n'):
        line = line.strip('\n')
        if len(line) == 0 or line.__contains__('List') or line.__contains__('*'):
            continue
        # 以whitespace分开
        devices.append(line.split()[0].strip('\\s+'))
    return devices
