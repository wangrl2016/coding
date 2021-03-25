import os
from datetime import datetime

from src import info, schedule


def is_coordinate_checkin(a):
    """
    :return: 程序是否需要点击桌面图标启动
    """
    return info.activities[a].__contains__('#')


def get_photos(path):
    """
    获取path目录下的所有png图片集合
    :param path: 目录路径
    :return: 图片路径集合
    """
    photos = []
    for root, dirs, filenames in os.walk(path):
        for file in filenames:
            if os.path.splitext(file)[1].__eq__('.png'):
                photos.append(os.path.join(root, file))
    return photos


def get_packages_dict(activities_dict):
    """
    将activity对应的dict转化为package对应的dict

    :param activities_dict: 用户定义的程序和活动dict
    :return: package对应的dict
    """
    packages_dict = activities_dict.copy()
    for key in packages_dict:
        packages_dict[key] = packages_dict[key][1 if packages_dict[key].__contains__('#') else 0:
                                                packages_dict[key].index('/')]
    return packages_dict


def schedule_apps(device, w, h):
    """
    做两次程序的定时任务
    第1次半个小时，其余时间用来看视频
    第2次做重要的任务
    """
    if datetime.now().minute < info.SCHEDULE_TIME:
        print('第1次定时任务 ' + datetime.now().__str__())
        for a in info.apps:
            getattr(schedule, a)(device, w, h)
