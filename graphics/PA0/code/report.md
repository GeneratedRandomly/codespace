# 图形学PA0实验报告
## 计35 仲嘉暄 2023010812
## 代码逻辑
### 直线绘制
对课本上的Bresenham算法进行改进。
- 首先判断斜率绝对值是否大于1（即`is_steep`），利用直线$y=x$对称，确保斜率绝对值小于等于1。
- 然后确保画线方向是从左到右，此时只存在直线函数增减性的区别。
- `dy`取绝对值，这样相当于把直线关于$y=y_A$对称，确保直线是斜率小于等于1、从左往右、单调递增的情况，即课本情况。
- 然后使用课本所给Bresenham逻辑。要注意还原斜率（见三目表达式）、还原增减（即`ystep`）。同时要注意，我们归约到课本情况之后，永远是`x++`驱动画线，因为我们在形式上要看成是斜率小于等于1、从左往右、单调递增，故`x`为自变量。
### 圆形绘制
在书上的基础小加修改即可。
- 画八对称点时候，要加上圆心坐标cx、cy。
- 将d变成书上的两倍，即将浮点数改成整数而加快了效率。
- 后来在AI帮助下发现了不用乘法的算法，但效率还没有原来的方法高。
~~~hpp
int x = 0, y = radius, d = 1 - radius;
int deltaE = 3, deltaSE = -2 * radius + 5;
circlepoints(img, x, y);
while (y > x)
{
    if (d < 0)
    {
        d += deltaE;
        deltaE += 2;
        deltaSE += 2;
    }
    else
    {
        d += deltaSE;
        deltaE += 2;
        deltaSE += 4;
        y--;
    }
    x++;
    circlepoints(img, x, y);
}
~~~
### 填色
我们用一个bool函数确保填色在图片边界截止，而且只填色和初始点颜色相同的。然后用队列实现BFS即可。四连通填色。
## 代码参考
教材\
[维基百科-Bresenham算法](https://zh.wikipedia.org/wiki/%E5%B8%83%E9%9B%B7%E6%A3%AE%E6%BC%A2%E5%A7%86%E7%9B%B4%E7%B7%9A%E6%BC%94%E7%AE%97%E6%B3%95)\
[前人代码](https://github.com/Guangxuan-Xiao/THU-Computer-Graphics-2020/blob/master/PA0/code/include/element.hpp)\
GitHub copilot
## 遇到问题
在直线绘制时曾经将`canvas01.bmp`左上角两条直线画成平的了，后来发现是`dy`没取绝对值，导致正负抵消。
## 未解决的困难
我的画圆算法有问题，但最终得到的答案是正确的：
~~~hpp
void draw(Image &img) override
{
    int x = 0, y = radius, d = 3 - (radius << 1);
    circlepoints(img, x, y);
    while (x <= y)
    {
        if (d < 0)
            d += (x << 2) + 6;
        else
        {
            d += ((x - y) << 2) + 10;
            y--;
        }
        x++;
        circlepoints(img, x, y);
    }
}
~~~
这里的`d = 3 - (radius << 1)`是有问题的。书上证明的算法是`d`为单精度浮点且初始值`1.25 - radius`，我误将1.25*2算成3，却正常画出了圆，减少了一次移位。但若把3改成2，则画出方形。对于为什么出现这个现象，以及如何证明这里用3的合理性，我没有解决，希望以后有机会改进一下。
## 建议
建议改进教材代码。