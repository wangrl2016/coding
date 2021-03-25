# 手机参数


# 无法通过activity启动的app前使用#标记
activities = {
    # 1-4
    'zhaoshang': 'cmb.pb/.app.mainframe.container.PBMainActivity',
    'toutiao': 'com.ss.android.article.lite/.activity.SplashActivity',
    'kuaishou': 'com.kuaishou.nebula/com.yxcorp.gifshow.HomeActivity',
    'douyin': 'com.ss.android.ugc.aweme.lite/com.ss.android.ugc.aweme.splash.SplashActivity',

    # 5-8
    'jingdong': '#com.jd.jdlite/.MainFrameActivity',
    'fanqie': '#com.dragon.read/.pages.main.MainFragmentActivity',
    'fanchang': '#com.xs.fm/com.dragon.read.pages.main.MainFragmentActivity',
    'kuchang': '#com.kugou.android.douge/com.kugou.android.app.MediaActivity',

    # 9-12
    'shuqi': '#com.shuqi.controller/com.shuqi.activity.MainActivity',
    'yingke': '#com.ingkee.lite/com.meelive.ingkee.business.main.entry.legacy.MainActivity',
    'kugou': '#com.kugou.android.elder/com.kugou.android.app.MediaActivity',
    'zhongqing': '#cn.youth.news/.ui.splash.SplashAdActivity',

    # 13-16
    'kuaiyin': '#com.kuaiyin.player/.v2.ui.main.MainActivity',
    'kuge': '#com.kugou.android.child/com.kugou.android.app.MediaActivity',
    'momo': '#com.immomo.young/com.immomo.momo.maintab.MaintabActivity',
    'qingtuanshe': '#com.qts.customer/com.qts.point.DailyEarnMoneyActivity',

    # 17-20
    'eleme': '#me.ele/.application.ui.splash.SplashActivity',
    'changdou': '#com.zf.shuashua/.MainActivity',
    'kuaikandian': '#com.yuncheapp.android.pearl/com.kuaishou.athena.MainActivity',

    # 49-52
    'qutoutiao': 'com.jifen.qukan/com.jifen.qkbase.main.MainActivity',
    'baidu': 'com.baidu.searchbox.lite/com.baidu.searchbox.MainActivity',
    'weishi': 'com.tencent.weishi/com.tencent.oscar.module.splash.SplashActivity',
    'douhuo': 'com.ss.android.ugc.live/.main.MainActivity',

    # 53-56
    'chejia': 'com.autohome.speed/com.cubic.autohome.MainActivity',
    'uc': 'com.UCMobile/com.uc.browser.InnerUCMobile',
    'diantao': 'com.taobao.live/.home.activity.TaoLiveHomeActivity',
    'huitoutiao': 'com.cashtoutiao/.account.ui.main.MainTabActivity',

}

# 所有程序名
apps = []

# 所有程序对应的包名(app:package)
packages = {}

# 手机运行信息(线程号)
contexts = {}

# 高配置手机
high_serials = ['9598552235004UD']

# 程序的定时任务为30分钟
SCHEDULE_TIME = 30
