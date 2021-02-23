# FFmpeg示例

## 安装FFmpeg库

```
./configure --enable-shared // configure lib
make                        // build
sudo make install           // install
sudo vim /etc/ld.so.conf    // make ld to find
/usr/local/lib              // content
sudo ldconfig               // take effect
```

## FFmpeg示例

1. 打印多媒体文件元数据

2. 打印音视频流信息
