## 思考题

`2A`：依题意，对于所求$n$的最大值$n_m$，有$10n_m{^2}\times10^{-8}\times2=1$，故$n_m=\sqrt{5\times10^6}\approx2236$条。
`2B`：依题意，对于所求$n$的最大值$n_m$，有$20n_m\log_2{n_m}\times10^{-8}\times2=1$，化简为$n_m\ln{n_m}=2.5\times10^6 \times\ln2$，牛顿法得$n_m\approx145746$条。

## 调试和测试

`3A`：通过静态检查发现：①两个代码中二维数组`matrix`（和`rowsum`）初始化时`for(int i=1; i<=n; ++i)`可能导致数组越界和空间浪费，可以改成`matrix[2001][2001]`。②每次询问查询时没有注意边界检查，可能导致数组越界（当然，生成input时会断言a、b不足以使数组越界）。③在`solution_2.cpp`中，`sum`的计算有误。$r_{xy}=\sum_{j=1}^ym_{xj}$，所以$\sum_{i=x}^{x+a-1}\sum_{j=y}^{y+b-1}m_{ij}=\sum_{i=x}^{x+a-1}(r_{i,y+b-1}-r_{i,y-1})$而代码所求的是$\sum_{i=x}^{x+a-1}(r_{i,y+b}-r_{i,y})$，应把`sum += rowsum[x + j][y + b] - rowsum[x + j][y];`改成`sum += rowsum[x + j][y + b-1] - rowsum[x + j][y-1];`④每次询问没有重置sum，会导致子数组元素和过大。AIGC工具能够找到BUG①②④，但其对于数组索引从1开始没有做出修改，而对于没有注意边界检查的修改只包括i>n和j>m的检查，而没有包括它们小于0的检查。可见AIGC找出bug的能力及格，但修改bug的能力不足。

`3B`：对于`vscode`的图形化调试工具，点击快进可以到达下一个断点（快捷键`Fn+F5`，`gdb`命令`c`），逐过程可以完成当前高亮一行（如有函数跳过之），快捷键`Fn+F10`，`gdb`命令`n`，单步调试会进入高亮一行中的函数、运算符重载等，快捷键`Fn+F11`，`gdb`命令`s`，如果要从当前调用栈中退出可以使用单步跳出，快捷键`Fn+Shift+F11`，`gdb`命令`finish`，为了使用调试器在编译时要加入`-g`选项。

`3C`：`srand(time(0))`用于设置随机数生成器的种子。`time(0)`函数返回当前时间的秒数，每次运行程序时，这个值都会不同，所以可以保证生成的随机数随机。

`3D`：`system`函数会执行dos或shell命令。`system("g++ rand_input.cpp -o rand_input");`、`system("g++ check_input.cpp -o check_input");`、 `system("g++ solution_1.cpp -o solution_1")`、` system("g++ solution_2.cpp -o solution_2");`是将这四个源代码编译链接成可执行文件。`system("./rand_input > rand.in");`、`system("./check_input < rand.in")!=0)`则是将`rand_input`重定向到`rand.in`文件，也就是把它本来输出在屏幕上的内容输入到`rand.in`文件里去，以及把`rand.in`文件再重定向到`check_input`文件去，也就是把它用`check_input`按规则扫描检验一遍，如果非法就会触发其中的`assert`，从而返回0。重定义使得这两个代码既可以通过文件直接传输数据，也可以把数据打印到屏幕上/从屏幕上扫描。`system("./solution_1 < rand.in > 1.out");`、`system("./solution_2 < rand.in > 2.out");`则类似输入输出流，如第一句是将`rand.in`输入到`solution_1`可执行文件，然后原句还剩下`solution_1 > 1.out`即将`solution_1`输出到`1.out`，下一句同理。`system("diff 1.out 2.out")`则是判断`1.out``2.out`两个文件有无差异之处，实现对拍。

`3E`：观察到`gdb`调试迅速退出，并且终端显示`Program received signal SIGSEGV, Segmentation fault.\n fn () at recur.cpp:2 \n 2		fn();`意思为：程序收到了段错误信号（这是由于无限递归调用导致栈溢出），错误位置在函数`fn()`，其位于`recur.cpp`第二行，最后一句是该行的全部信息。

## 优化与比较

`4A`：运用递推思想，可以设置一个类似于`solution_2.cpp`中`rowsum`的矩阵`sum[2001][2001]`，使`sum[a][b]`储存$\sum_{i=1}^{a}\sum_{j=1}^{b}m_{ij}$。`sum`矩阵首行、首列也都为0，可以利用其每个元素周边元素推出它的值。由此我们有`sum[i][j] = sum[i-1][j] + sum[i][j-1] - sum[i-1][j-1] + matrix[i][j];`。可以用归纳法：如果这个算法对`sum[i-1][j]、sum[i][j-1]、sum[i-1][j-1]`成立（一开始，可以让`i=j=1`），$\sum_{x=1}^{i-1}\sum_{y=1}^{j}m_{xy}+(\sum_{x=1}^{i}\sum_{y=1}^{j-1}m_{xy}-\sum_{x=1}^{i-1}\sum_{y=1}^{j-1}m_{xy})+m_{ij}=\sum_{x=1}^{i-1}\sum_{y=1}^{j}m_{xy}+(\sum_{y=1}^{j-1}m_{iy}+m_{ij})$，其等于$\sum_{x=1}^{i-1}\sum_{y=1}^{j}m_{xy}+\sum_{y=1}^{j}m_{iy}=\sum_{x=1}^{i}\sum_{y=1}^{j}m_{xy}$，从而得证。然后每次取出$\sum_{i=x}^{x+a-1}\sum_{j=y}^{y+b-1}m_{ij}$，也就是`sum[x+a-1][y+b-1] - sum[x-1][y+b-1] - sum[x+a-1][y-1] + sum[x-1][y-1]`，其正确性是显然的。

`4B`：我们编写`compare.cpp`文件，对`n、m、q`不同情况下三种算法用时进行比较。

|                  | n = m = 2000, q = 10^5 | n = m = 100, q = 10^5 | n = m = 2000, q = 1000 | n = m = 100, q = 1000 | n = 2000, m = 100, q = 10^5 | n = 100, m = 2000, q = 10^5 |
| ---------------- | ---------------------- | --------------------- | ---------------------- | --------------------- | --------------------------- | --------------------------- |
| `solution_1.cpp` | `37.82s`               | `0.10s`               | `0.53s`                | `0.00s`               | `3.20s`                     | `3.02s`                     |
| `solution_2.cpp` | `0.43s`                | `0.00s`               | `0.15s`                | `0.00s`               | `0.30s`                     | `0.06s`                     |
| `solution_3.cpp` | `0.16s`                | `0.00s`               | `0.14s`                | `0.00s`               | `0.05s`                     | `0.04s`                     |

`4C`：结论：当数据规模大即`n、m、q`较大时，`solution_3`比其他两种算法有显著优势，尤其当`q`巨大时，``solution_1.cpp`每次问询都带来`O(n*m)`的平均复杂度，总时间复杂度在`O(n*m*q)`量级，`solution_2.cpp`时间复杂度在`O(n*m + n*q)`，而`solution_3.cp`的时间复杂度却只有`O(n*m + q)`的数量级，所以数据规模越大，第三种算法越有优势。
