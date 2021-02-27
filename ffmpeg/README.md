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

## FFmpeg示例

### 基础大纲

- [x] PBM图像格式讲解

- [x] FFmpeg打印多媒体文件元数据 `Metadata.cpp`

- [x] FFmpeg打印音视频流信息 `Dump.cpp`

- [x] FFmpeg解码视频流 `DecodeVideo.cpp`

- [x] FFmpeg编码视频流 `EncodeVideo.cpp`

- [x] RGBA图片构建视频 `Rgba2Video.cpp`

- [x] 合成音频 `EncodeAudio.cpp`

- [x] 对视频进行缩放 `ScaleVideo.cpp`

- [x] 

