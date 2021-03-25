from src import checkin, app


def toutiao(device, w, h):
    checkin.toutiao(device)

    # [x] 阅读头条文章
    app.read_toutiao_article(device, w, h, num=1)
