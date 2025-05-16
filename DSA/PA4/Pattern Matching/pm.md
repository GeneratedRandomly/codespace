## 算法思路和实现要点
思路：  
我们算出大矩阵每一个$m\times n$子矩阵的 `hash`，然后和目标函数的 `hash` 匹配即可。  
要点：  
取一个大质数 `100000000000001219LL` 当模，然后取质基数 `base1=1319` 和 `base2=100493`，事先用费马小定理和快速幂算出来二者的数论倒数 `invbase1=64139499620925725LL` 和 `invbase2=34835262157563638LL`。由于非常大，所以哈希冲突可能性极小。  
一个矩阵`S`的哈希$hash(S)=\Sigma_{i,j} hash(S[i][j])$，其中$hash(S[i][j])=T[i][j]\times base1^i\times base2^j$。  
我们在 `T_hashed` 矩阵每一个元素保存$T\_hashed[i][j]=T[i][j]\times base1^i\times base2^j$，这里的乘法都是数论乘法（用 `int128` 再模）。然后维护它的前缀和，这样就能快速查到以`T[i][j]`为左上角的$m\times n$大小子矩阵的哈希和了。但是要注意的是，每次 `query` 时，要乘上$invbase1^i\times invbase2^j$，因为大矩阵元素的 `hash` 同它们到 `T[0][0]` 的距离有关，所以要消除 `T` 中位置信息。  
任何涉及到哈希的运算都要使用规定好的 `add` 和 `mul` 接口，以保证模了模数并且乘法不会溢出。
## 渐进时间复杂度的分析，包含过程
`init` 时，会计算 `base` 和 `invbase` 的各个幂，时间复杂度$O(M+N)$；计算 `T` 矩阵每个$(i,j)$元的 `hash` 并存到 `T_hashed`，以及计算 `T_hashed` 的前缀和存到 `T_hashed_prefix`，$O(MN)$；`query` 主矩阵中每个子矩阵的 `hash`，并且无序存下来到 `hash_bucket`，$O((M-m+1)(N-n+1)) \approx O(MN)$。  
`k` 次查询，每次先暴力计算目标矩阵 `hash`，$O(mn)$；然后遍历 `hash_bucket` 测试是否匹配，$O((M-m+1)(N-n+1)) \approx O(MN)$。
综上时间复杂度$O(kMN)$。
## 渐进空间复杂度的分析，包含过程
~~~cpp
char **T; // 主矩阵
char **P; // 目标矩阵
ll **T_hashed1;
ll **T_hashed_prefix1;
ll *hash_bucket;
ll *base1;    // 用于存储BASE^m的值
ll *base2;    // 用于存储BASE^m的值
ll *invbase1; // 用于存储BASE^m的值
ll *invbase2; // 用于存储BASE^m的值
~~~
其中`T T_hashed T_hashed_prefix`是$O(MN)$，`P`是$O(mn)$，`base invbase`是$O(M+N)$，`hash_bucket`是$O((M-m+1)(N-n+1))$。  
综上，空间复杂度$O(MN)$。
## 遇到的困难和解决方法
矩阵哈希多次TLE、WA。尝试了双模哈希、自然溢出，最终选用了大质数模哈希。
## 估计完成本题所用时间
10h
## 关于本题的更多感想
第14个点好神奇，是专门做出来卡自然溢出的吗（）