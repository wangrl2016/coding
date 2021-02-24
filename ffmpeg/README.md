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

### 基础大纲

- [ ] PBM图像格式讲解

- [ ] FFmpeg打印多媒体文件元数据 `Metadata.cpp`

- [ ] FFmpeg打印音视频流信息 `Dump.cpp`

- [ ] FFmpeg解码视频流 `DecodeVideo.cpp`

- [ ] FFmpeg编码视频流 `EncodeVideo.cpp`

- [ ] RGBA图片构建视频 `Rgba2Video.cpp`

- [ ] RGB格式存储
