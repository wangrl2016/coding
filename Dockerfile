FROM ubuntu:20.04

#
# sudo docker build -t coding-develop:0.1.0 .
# sudo docker run -it coding-develop:0.1.0 /bin/bash
#

MAINTAINER wangrl2016
COPY . /app
WORKDIR /app

# 下载依赖
RUN apt update \
    && apt install -y git vim gcc g++ make openjdk-14-jdk \
    			 nasm curl python libstdc++6 libasound \
                        libssl-dev libxrandr-dev libxinerama-dev libxcursor-dev \
			libxi-dev openjdk-11-jdk libopenal-dev libvorbis-dev \
			libflac-dev libmp3lame-dev libx264-dev libx11-dev \
			xorg-dev libglfw3-dev python3-tk python3-pil.imagetk 
    # 安装FFmpeg依赖库
    && git clone --depth 10 https://github.com/FFmpeg/FFmpeg \
    && cd FFmpeg \
    && ./configure --enable-ffplay --enable-shared --enable-libmp3lame --enable-libx264 --enable-gpl \
    && make \
    && sudo make install \
    && sudo ldconfig \
    && cd .. \
    # 安装OpenCV依赖库
    && git clone --depth 10 https://github.com/opencv/opencv \
    && cd opencv \
    && mkdir -p build \
    && cd build \
    && cmake .. \
    && make -j4 \
    && sudo make install \
    && cd ../.. \
    # 安装CMake构建工具
    && git clone --depth 10 https://github.com/Kitware/CMake/ \
    && cd CMake \
    && ./configure \
    && make -j4 \
    && sudo make install
    && cd .. \
    # 安装OpenGL库
    && apt install mesa-common-dev \
    # 安装GLFW图形显示




CMD python3 /app/git_sync_deps.py
