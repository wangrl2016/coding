import subprocess


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
