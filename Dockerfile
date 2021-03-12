FROM ubuntu:18.04

MAINTAINER wangrl2016
COPY . /app
WORKDIR /app

# 下载依赖
RUN apt update \
    && apt install -y git vim openjdk-11-jdk nasm curl python libstdc++6 libasound \
                        libssl-dev \
    # 安装FFmpeg依赖库
    && git clone --depth 10 https://github.com/FFmpeg/FFmpeg \
    && cd FFmpeg \
    && ./configure --enable-shared --enable-libmp3lame --enable-libx264 --enable-gpl \
    && make \
    && sudo make install \
    && cd .. \
    # 安装OpenCV依赖库
    && git clone --depth 10 https://github.com/opencv/opencv \
    && cd opencv \
    && mkdir -p build \
    && cd build \
    && make -j4 \
    && sudo make install \
    && cd ..\
    # 安装CMake构建工具
    && git clone --depth 10 https://github.com/Kitware/CMake/ \
    && cd CMake \
    && ./configure \
    && make -j4 \
    && sudo make install
    && cd ..




CMD python3 /app/git_sync_deps.py
