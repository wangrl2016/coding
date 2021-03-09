import os


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
