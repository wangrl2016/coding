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

- [x] 构造YUV数据编码视频

![编码视频](../res/yuv420p-encode.png)

- [x] 构造RGBA数据编码视频

![RGBA数据构造视频](../res/rgba-to-yuv420p.png)

- [x] 编码MP3音频

```
Output #0, mp3, to 'out/16bit-dummy-audio.mp3':
    Stream #0:0: Audio: mp3, 44100 Hz, stereo, s32p, 64 kb/s
```

- [x] 解码MP3音频

```
Play the output audio file with the command:
ffplay -f f32le -ac 2 -ar 44100 out/mp3-samples.raw
```
      
- [x] 放大视频尺寸

```
Scaling succeed. Play the output file with the command:
ffplay -f rawvideo -pix_fmt rgb24 -video_size 800x600 out/scale-video.raw
```

- [x] 解码音视频

- [x] 视频filter处理

- [x] 音频filter处理

- [ ] MP3格式转码成为AAC格式

- [x] 合成音视频

```
pts:60928 pts_time:1.14523e+10 dts:58880 dts_time:1.14523e+10 duration:0 duration_time:0 stream_index:0
pts:259072 pts_time:3.19107e+09 dts:259072 dts_time:3.19107e+09 duration:1024 duration_time:0.02322 stream_index:1
pts:260096 pts_time:3.19107e+09 dts:260096 dts_time:3.19107e+09 duration:1024 duration_time:0.02322 stream_index:1
pts:59904 pts_time:1.14523e+10 dts:59392 dts_time:1.14523e+10 duration:0 duration_time:0 stream_index:0
pts:261120 pts_time:3.19107e+09 dts:261120 dts_time:3.19107e+09 duration:1024 duration_time:0.02322 stream_index:1
pts:262144 pts_time:3.19107e+09 dts:262144 dts_time:3.19107e+09 duration:1024 duration_time:0.02322 stream_index:1
```

- [x] 容器转换(MP4->MOV)

- [ ] 两个声音混合
