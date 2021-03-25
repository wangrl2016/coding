from datetime import datetime

from src import checkin, app, phone, info


def toutiao(device, w, h):
    def open_treasure():
        print('今日头条开宝箱 ' + datetime.now().time().__str__())

    checkin.toutiao(device)

    # [x] 阅读头条文章
    app.read_toutiao_article(device, w, h, num=1)

    phone.stop_app(device, info.packages['toutiao'])
