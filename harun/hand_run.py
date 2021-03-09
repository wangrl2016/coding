import os
import shutil
import tkinter as tk

from src import phone


class Point(object):
    """
    Create a point on a coordinate with values
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

        # 鼠标开始点和释放点
        self.point0 = Point(0, 0)
        self.point1 = Point(0, 0)

        self.image_frame = tk.Frame(self, width=w * scale, height=h * scale, bg='white')
        self.operate_frame = tk.Frame(self, width=w * scale, height=h * scale, bg='beige')

        self.master = master
        self.pack()
        self.create_widgets()

    def create_widgets(self):
        self.image_frame.pack_propagate(0)  # 固定frame的大小
        self.image_frame.pack(side='left')
        self.operate_frame.pack_propagate(0)
        self.operate_frame.pack(side='left')


if __name__ == '__main__':
    run_threads = []

    root = tk.Tk()

    root.title('手动控制系统')

    devices = phone.get_devices()
    if not devices:
        print('没有发现设备')
        exit(0)

    out_dir = 'out/'
    scale = 0.5

    (w, h) = phone.get_size(devices[0])
    app = Application(master=root)

    app.mainloop()

    # 清理可能存在的文件
    if os.path.exists(out_dir):
        shutil.rmtree(out_dir)
    os.mkdir(out_dir)
