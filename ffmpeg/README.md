# FFmpeg示例

## 安装FFmpeg库

安装依赖

```
sudo apt install libmp3lame-dev
```

```
./configure --enable-shared // configure lib
make                        // build
sudo make install           // install
sudo vim /etc/ld.so.conf    // make ld to find
/usr/local/lib              // content
sudo ldconfig               // take effect
```

详细的编译选项
```
--enable-shared 编译动态库
--enable-libmp3lame 增加MP3编码
--enable-libx264 --enable-gpl x264编码
```

播放原始yuv数据

`ffplay -f rawvideo -video_size 1280x720 xxx.yuv`

播放16kHz单声道

`ffplay -ar 16000 -channels 1 -f s16le -i xxx.pcm`

## FFmpeg示例

- [x] 打印多媒体文件元数据

```
major_brand = isom
minor_version = 512
compatible_brands = isomiso2avc1mp41
encoder = Lavf58.47.100
```

- [x] 打印音视频流信息

```
Input #0, mov,mp4,m4a,3gp,3g2,mj2, from '../res/big-buck-bunny.mp4':
  Metadata:
    major_brand     : isom
    minor_version   : 512
    compatible_brands: isomiso2avc1mp41
    encoder         : Lavf58.47.100
  Duration: 00:00:30.02, start: 0.000000, bitrate: 1928 kb/s
  Stream #0:0(und): Video: h264 (High) (avc1 / 0x31637661), yuv420p, 1280x720 [SAR 1:1 DAR 16:9], 1794 kb/s, 24 fps, 24 tbr, 12288 tbn, 48 tbc (default)
    Metadata:
      handler_name    : (C) 2007 Google Inc. v08.13.2007.
      vendor_id       : [0][0][0][0]
  Stream #0:1(und): Audio: aac (LC) (mp4a / 0x6134706D), 44100 Hz, stereo, fltp, 129 kb/s (default)
    Metadata:
      handler_name    : (C) 2007 Google Inc. v08.13.2007.
      vendor_id       : [0][0][0][0]
```

- [x] 解码视频为PPM图片

![解码图片](../res/video-decode-ppm.png)

