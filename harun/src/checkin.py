# 对于无法通过Activity进行启动的图标需要定位
import inspect

from src import phone
from src.info import activities_dict


def toutiao(device, gap=10):
    phone.start_app(device, activities_dict[
        inspect.getframeinfo(inspect.currentframe()).function.__str__()], gap)