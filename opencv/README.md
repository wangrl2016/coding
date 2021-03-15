
安装opencv

```
git clone https://github.com/opencv/opencv
cd opencv
mkdir -p build
cd build/
cmake ..
make -j4
sudo make install
```

- [x] 显示图片

- [x] 图像增强

```
I(i,j) = 5 * I(i, j) - [I(i-1, j) + I(i+1, j) + I(i, j-1) + I(i, j+1)]
```

- [x] 亮度和对比度(Brightness and contrast)

```
g(i, j) = a * f(i, j) + b
where i and j indicates that the pixel is located in the i-th row and j-th column.
```

- [x] 绘制等高线

![绘制等高线](../res/find-contours.png)

- [ ] 混合模式
