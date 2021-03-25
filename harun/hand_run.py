import ctypes
import inspect
import os
import subprocess
import threading
import tkinter as tk
from datetime import datetime
from random import randrange

from PIL import Image, ImageTk

import auto_run
from src import phone, info, utils
from src import input
import shutil

from src.info import activities


def _async_raise(tid, exc_type):
    if not inspect.isclass(exc_type):
        raise TypeError("Only types can be raised (not instances)")
    res = ctypes.pythonapi.PyThreadState_SetAsyncExc(ctypes.c_long(tid), ctypes.py_object(exc_type))
    if res == 0:
        raise ValueError("Invalid thread id")
    elif res != 1:
        ctypes.pythonapi.PyThreadState_SetAsyncExc(tid, None)
        raise SystemError("PyThreadState_SetAsyncExc failed")


def stop_thread(thread):
    try:
        _async_raise(thread.ident, SystemExit)
    except Exception as e:
        print(e)


def close_top_app():
    print('关闭当前程序 ' + datetime.now().time().__str__())
    for pid in devices:
        top_activities = phone.get_top_activities(pid)
        if top_activities is None:
            return
        for a in info.apps:
            if top_activities.__contains__(info.packages[a]):
                phone.stop_app(pid, info.packages[a], 0)


def start_auto_running():
    """
    开启自动运行系统
    """
    runner_threads.clear()
    for pid in devices:
        info.contexts.update({pid: {}})
        t = threading.Thread(target=auto_run.run, args=(pid,), daemon=True)
        runner_threads.append(t)
        t.start()


def stop_auto_running():
    """
    关闭自动运行系统
    """
    for t in runner_threads:
        if t.is_alive():
            stop_thread(t)


class Point(object):
    """
    Creates a point on a coordinate with values x and y.
    """

    def __init__(self, x, y):
        self.x = x
        self.y = y

    def get_x(self):
        return self.x

    def get_y(self):
        return self.y

    def set_x(self, x):
        self.x = x

    def set_y(self, y):
        self.y = y


class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)

        self.continue_update_image = True
        self.auto_system_start = True

        # 鼠标的开始点和释放点
        self.point0 = Point(0, 0)
        self.point1 = Point(0, 0)

        self.image_frame = tk.Frame(self, width=w * scale, height=h * scale, bg='white')
        self.operate_frame = tk.Frame(self, width=w * scale, height=h * scale, bg='beige')

        self.button_frame = tk.Frame(self.operate_frame)

        self.phone_list = tk.Listbox(self.operate_frame, width=int(w * scale), selectmode=tk.SINGLE)
        self.phone_list.bind('<<ListboxSelect>>', self.phone_list_click)

        for pid in devices:
            self.phone_list.insert(tk.END, pid)

        img = Image.new(mode='RGB', size=(int(w * scale), int(h * scale)), color='white')
        img = ImageTk.PhotoImage(image=img)

        self.image_label = tk.Label(self.image_frame)

        self.image_label.config(image=img)
        self.image_label.image = img

        self.close_top_app = tk.Button(self.button_frame, text='关闭当前程序', command=close_top_app)
        self.hand_system = tk.Button(self.button_frame, text='手动系统已开启', bg='green', command=self.hand_system)
        self.auto_system = tk.Button(self.button_frame, text='自动系统已开启', bg='green', command=self.auto_system)

        self.home = tk.Button(self.button_frame, text='回到主页', command=self.go_home)
        self.reboot = tk.Button(self.button_frame, text='重启手机', command=self.reboot)
        self.update = tk.Button(self.button_frame, text='更新代码', command=self.update_code)

        self.master = master
        self.pack()

        self.create_widgets()

        # 指向之前显示的图片
        self.prev_img = None

    def phone_list_click(*args):
        # TODO: 完善函数功能
        print(*args)

    def hand_system(self):
        print(('关闭 ' if self.continue_update_image else '开启') + '手动系统 ' + datetime.now().__str__())
        self.continue_update_image = not self.continue_update_image
        if self.continue_update_image:
            self.hand_system['text'] = '手动系统已开启'
            self.hand_system['bg'] = 'green'
            self.update_page()
        else:
            self.hand_system['text'] = '开启手动已关闭'
            self.hand_system['bg'] = 'red'

    def auto_system(self):
        print(('关闭' if self.auto_system_start else '开启') + '自动系统 ' + datetime.now().__str__())
        if self.auto_system_start:
            stop_auto_running()
            self.auto_system['text'] = '自动系统已关闭'
            self.auto_system['bg'] = 'red'
        else:
            start_auto_running()
            self.auto_system['text'] = '自动系统已开启'
            self.auto_system['bg'] = 'green'
        self.auto_system_start = not self.auto_system_start

    def create_widgets(self):
        self.image_frame.pack_propagate(0)  # 固定frame的大小
        self.image_frame.pack(side='left')
        self.operate_frame.pack_propagate(0)
        self.operate_frame.pack(side='left')

        self.image_label.bind('<Button-1>', self.mouse_left_click)  # 鼠标左键单击
        self.image_label.bind('<Button-2>', self.mouse_center_click)  # 鼠标中键单击
        self.image_label.bind('<Button-3>', self.mouse_right_click)  # 鼠标右键单击
        self.image_label.bind('<ButtonRelease-2>', self.mouse_center_release)  # 鼠标中键释放

        self.image_label.bind('<KeyPress>', self.keyboard_press)

        self.image_label.focus_set()
        self.image_label.pack()

        self.button_frame.pack(side='top')
        self.phone_list.pack(side='top')

        # 第0排按钮
        self.close_top_app.grid(row=0, column=0)
        self.hand_system.grid(row=0, column=1)
        self.auto_system.grid(row=0, column=2)

        # 第1排按钮
        self.home.grid(row=1, column=0)
        self.update.grid(row=1, column=1)
        self.reboot.grid(row=1, column=2)

    @staticmethod
    def mouse_left_click(event):
        print('点击鼠标左键 (' + str(event.x) + ', ' + str(event.y) + ')')
        for pid in devices:
            tid = threading.Thread(target=phone.tap, args=(pid, int(event.x / scale), int(event.y / scale)))
            tid.start()

    def mouse_center_click(self, event):
        self.point0.set_x(event.x)
        self.point0.set_y(event.y)

    @staticmethod
    def mouse_right_click(event):
        print('点击鼠标右键 (' + str(event.x) + ', ' + str(event.y) + ')')
        for pid in devices:
            tid = threading.Thread(target=phone.go_back, args=(pid,))
            tid.start()

    def mouse_center_release(self, event):
        self.point1.set_x(event.x)
        self.point1.set_y(event.y)
        self.hand_swipe()

    def hand_swipe(self):
        print('滑动屏幕 ' + datetime.now().time().__str__())
        for pid in devices:
            tid = threading.Thread(target=input.swipe,
                                   args=(pid,
                                         int(self.point0.get_x() / scale),
                                         int(self.point0.get_y() / scale),
                                         int(self.point1.get_x() / scale),
                                         int(self.point1.get_y() / scale),
                                         randrange(190, 230)))
            tid.start()

    @staticmethod
    def keyboard_press(event):
        print('按键事件 ' + event.keysym)

    @staticmethod
    def reboot():
        print('手机重启 ' + datetime.now().time().__str__())
        for pid in devices:
            tid = threading.Thread(target=phone.reboot, args=(pid,))
            tid.start()

    @staticmethod
    def go_home():
        print('回到主页 ' + datetime.now().time().__str__())
        for pid in devices:
            tid = threading.Thread(target=phone.go_home, args=(pid,))
            tid.start()

    def update_page(self):
        update_page_thread = UpdatePageThread(devices[0], out_dir)
        update_page_thread.start()
        update_page_thread.join()

        file_path = update_page_thread.get_file_path()
        try:
            # 可能中途拔掉手机
            img = Image.open(out_dir + file_path).resize((int(w * scale), int(h * scale)))
        except Exception as e:
            print(e)
            return None

        img = ImageTk.PhotoImage(image=img)
        self.image_label.config(image=img)
        self.image_label.image = img

        if self.prev_img:
            os.remove(out_dir + file_path)

        self.prev_img = file_path

        if self.continue_update_image:
            root.after(10, self.update_page)

    @staticmethod
    def update_code():
        print('更新代码 ' + datetime.now().time().__str__())
        subprocess.run(['git', 'checkout', 'https://username:password@github.com/wangrl2016/runner'])


class UpdatePageThread(threading.Thread):
    def __init__(self, pid, od):
        threading.Thread.__init__(self)
        self.pid = pid
        self.out_dir = od
        self.file_path = ''

    def run(self):
        self.file_path = phone.get_page_photo(devices[0], out_dir)

    def get_file_path(self):
        return self.file_path


if __name__ == '__main__':
    runner_threads = []

    # 初始化全局变量
    info.apps = list(activities.keys())
    info.packages = utils.get_packages_dict(activities)

    root = tk.Tk()

    root.title('手机手动控制系统')

    devices = phone.get_devices()
    if devices.__contains__('13bfd6e6'):
        devices.remove('13bfd6e6')
    if devices.__contains__('8aa89ae87d94'):
        devices.remove('8aa89ae87d94')
    if not devices:
        print('没有发现手机设备')
        exit(0)

    # 开启自动运行系统
    start_auto_running()

    out_dir = 'out/'

    (w, h) = phone.get_size(devices[0])

    scale = 0.5

    app = Application(master=root)

    hand_thread = threading.Thread(target=root.after, args=(50, app.update_page), daemon=True)
    hand_thread.start()

    app.mainloop()

    # 清理可能存在的文件
    shutil.rmtree(out_dir)
    os.mkdir(out_dir)
