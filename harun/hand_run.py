import os
import shutil
import threading
import tkinter as tk

from PIL import Image, ImageTk

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

        self.continue_update_image = False
        self.auto_system_start = True

        # 鼠标开始点和释放点
        self.point0 = Point(0, 0)
        self.point1 = Point(0, 0)

        self.image_frame = tk.Frame(self, width=w * scale, height=h * scale, bg='white')
        self.operate_frame = tk.Frame(self, width=w * scale, height=h * scale, bg='beige')
        self.button_frame = tk.Frame(self.operate_frame)

        img = Image.new(mode='RGB', size=(int(w * scale), int(h * scale)), color='white')
        img = ImageTk.PhotoImage(image=img)

        self.image_label = tk.Label(self.image_frame)
        self.image_label.config(image=img)
        self.image_label.image = img

        self.hand_system = tk.Button(self.button_frame, text='手动系统已开启', bg='green',
                                     command=self.hand_system)

        self.master = master
        self.pack()
        self.create_widgets()

        # 指向之前显示的图片
        self.prev_img = None

    def hand_system(self):
        self.continue_update_image = not self.continue_update_image
        if self.continue_update_image:
            self.update_page()

    def create_widgets(self):
        self.image_frame.pack_propagate(0)  # 固定frame的大小
        self.image_frame.pack(side='left')
        self.operate_frame.pack_propagate(0)
        self.operate_frame.pack(side='left')

        self.button_frame.pack(side='top')

    def update_page(self):
        update_page_thread = UpdatePageThread(devices[0], out_dir)
        update_page_thread.start()
        update_page_thread.join()

        file_path = update_page_thread.get_file_path()

        try:
            # 中途可能拔掉手机
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
            root.after(10, self.update_page())


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
    devices = phone.get_devices()
    if not devices:
        print('没有发现设备')
        exit(0)

    out_dir = 'out/'
    # 清理可能存在的文件
    if os.path.exists(out_dir):
        shutil.rmtree(out_dir)
    os.mkdir(out_dir)

    run_threads = []

    root = tk.Tk()
    root.title('手动控制系统')

    scale = 0.5

    (w, h) = phone.get_size(devices[0])
    app = Application(master=root)

    hand_thread = threading.Thread(target=root.after, args=(50, app.update_page), daemon=True)
    hand_thread.start()

    app.mainloop()
