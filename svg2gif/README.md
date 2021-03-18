## 将含有svg的gif转化为gif图片

#### 重要结构体介绍

借鉴FFmpeg中的AVFormatContext和AVCodecContext两个结构体思想，

构造SvgContext, RenderContext两个结构体。

* SvgContext保留和Svg文件相关的信息

* FrameContainer保留所有gif解码的帧

* RenderContext生成合成gif需要的帧

* EncodeContext将生成的帧合成gif

#### 有待提升的地方

* 提升渲染引擎兼容性

* Gif全部解码内存使用量太大

## 学习Rust编写

```
https://doc.rust-lang.org/book/
```

