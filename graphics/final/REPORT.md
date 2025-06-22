# 计算机图形学大作业报告
### 仲嘉暄 计35 2023010812
首先解释一下验收中的问题：MIS之后变暗，是因为没有删除漫反射采样光线击中光源返回零的设置；折射球没有显示出顶部光源，是因为普通蒙特卡洛光线追踪漏了菲涅尔项，只在NEE版本加入了。现在都已经修复。
## 已验收功能
### 基础项
#### Whitted-Style 光线追踪
我们让相机产生的光线遇到反射和折射材料的时候弹射，遇到漫反射材料的时候调用Phong模型计算色彩，阴影效果由物点向光源方向射线，如果中途相交就抛去光源即可。左侧是 Whitted-Style，右侧是没有实现NEE之前，64spp的路径追踪。（ws_vs_pt__ws.bmp和ws_vs_pt__pt.bmp）
#### Whitted-Style 和路径追踪对比
<div style="display: flex; justify-content: center;">
  <img src="results/ws_vs_pt__ws.bmp" width="400"/>
  <img src="results/ws_vs_pt__pt.bmp" width="400"/>
</div>
两张图片的不同之处：

- Whitted-Style算法的结果没有噪点，路径追踪的结果有噪点，原因是前者的扫描光线每次反射、折射方向固定，遇到了漫反射平面就直接用Phong模型计算，没有方差；但路径追踪的原理是采样统计，漫反射时方向随机，所以存在方差，产生噪点。
- Whitted-Style算法的质感比较虚假，路径追踪比较真实，原因是路径追踪是基于物理的，不像Whitted-Style那样简单地使用shadow ray，很方便递归计算光线，阴影、折射都可以用光线的效果来体现，所以阴影看起来很真实，折射球的底部有一个顶灯聚焦的绿色光斑，也很合理。Whitted-Style右上角反射球反射的绿球是黑的，这就是阴影不真实的体现。
- Whitted-Style算法天花板上是个点光源，但在路径追踪中无法使用点光源，因为需要击中光源返回光强，光源越小就越难击中，收敛更慢。
- 使用OpenMP之后，Whitted-Style只需要数秒，而64spp的蒙特卡洛光线追踪却需要好几分钟。
#### 蒙特卡洛光线追踪
在mcpt.hpp中实现，现在使用文件开头的bool变量调控NEE、MIS、菲涅尔等。  
我们对每个像素多次采样平均，每次采样光线漫反射、反射、折射都是递归调用，击中光源就返回光源光强。实现了动态轮盘赌，前5层递归drop概率为0，以后为0.1。现在，在漫反射情况不会返回着色函数结果，而是在法向量所在半球采样反射光线，直至击中光源。从上面的图可以看出来，经典的路径追踪噪点很多。
#### glossy BRDF
在ptutil.hpp中，按照课程PPT上的公式，我实现了基于Beckman分布的Cook Terrance模型，实现了glossy表面。基础项部分参见其中的均匀随机半球采样函数sample（漫反射也可用到）、beckmann_D、beckmann_G函数，在均匀半球采样的情况下概率密度函数为 $\frac{1}{2\pi}$。普通beckman的效果不是很好，也可能是采样次数不够，加上自相交问题，导致边界发黑。可以看到粗糙度不同的影响。
![](/results/glossybeckmann.bmp)
#### NEE
在NEE中我们对光源采样，在光源上随机均匀选一个点，只要和物体点中间没有遮挡，就计算来自于光源的直接光强，按照PPT中的伪代码进行计算。NEE没有改变渲染方程，而是提供了漫反射光线随机采样“必定击中光源部分”的“保底”，按概率计算这部分光强，从而省去很多无谓的采样。因此，在固定采样次数时，NEE的无用采样更少，收敛效率更高。为了美观，以后的图片都会带有NEE。我们比较实现NEE前后的同一张图，同为64spp，可以明显看到右侧使用NEE之后的收敛情况远远好于左侧基础路径追踪。（ws_vs_pt__pt.bmp和nee64.bmp）
<div style="display: flex; justify-content: center;">
  <img src="results/ws_vs_pt__pt.bmp" width="400"/>
  <img src="results/nee64.bmp" width="400"/>
</div>

### 加分项
先是基础要求文档中提到的加分项。为了美观，我们使用重要性采样（余弦加权半球采样），等到对应加分点再做讲解。未经说明，全部使用64spp。
#### 折射带菲涅尔系数
我们在路径追踪的折射部分按照Schlick近似算一个菲涅尔系数，作为反射概率，然后取一个随机数决定是否反射。下图的两个球，左侧是反射球，右侧是折射球，左右两张图都是这样的。左图是未实现菲涅尔系数，右侧则实现了。为了实现一个较高的反射率，我把绝对折射率调成了10.可以看到，实现菲涅尔之后，折射球也反射出了左侧的兔子和上方的顶灯。（results文件夹的fnl_no.bmp和fnl_yes.bmp）
<div style="display: flex; justify-content: center;">
  <img src="results/fnl_no.bmp" width="400"/>
  <img src="results/fnl_yes.bmp" width="400"/>
</div>

水的折射率是1.33，玻璃的折射率是1.54，这两种球中玻璃更容易发生全反射。可以看到，下图中左侧的水球的“颜色描边”明显比右边的玻璃球更小。这个描边，是球心对侧墙壁的光线入射之后在球内全反射，传播90度造成的。根据厚度，明显玻璃球全反射情况更明显。同时，玻璃球下面的聚焦光斑也更小，是因为被折射了。
![](/results/waterglass.bmp)
入射角接近0度时，反射率就是 $R_0=\left(\frac{n_i-n_t}{n_i+n_t}\right)^2$，我们在y=-50处放一个玻璃（n=1.54），相机放在y=-47.5，z=150处，在z=50、25、0、-25处错落地放几个球。可以看到，近处的球在玻璃中很不明显，基本看不到，但远一点就清晰多了。
![](/results/0incident.bmp)

#### 多种glossy材质
我实现了GGX的glossy BRDF，其中各向同性GGX是ptutil.hpp中的ggx_D、ggx_G函数，而各向异性GGX则是ggx_aniso_D、smithG1_aniso、ggx_aniso_G三个函数。
以下是粗糙度0.5和0.2的各向同性ggx glossy。
![](/results/glossyggx.bmp)
以下是各向异性ggx glossy，相比右图验收的时候转了个角度，色差是伽马校正造成的。
<div style="display: flex; justify-content: center;">
  <img src="results/glossyaniso.bmp" width="400"/>
  <img src="results/glossyaniso_gamma.bmp" width="400"/>
</div>

#### 多重重要性采样MIS
多重重要性采样是在NEE基础上做的优化，具体来说，就是原本的NEE在漫反射击中光源之后drop掉，现在改为直接光强和间接光强的加权平均；漫反射没有击中光源就返回直接项和间接项的和，现在也加权平均。我阅读了[图形学｜Robust：Multiple Importance Sampling 多重重要性采样](https://zhuanlan.zhihu.com/p/379681777)MIS的本意是改善小光源-大粗糙度（反射打不中光源）和大光源-小粗糙度（采样点不够多则反射破碎），对普通情况来说，反而引入噪点。不过，在glossy情况下，MIS能够提供一些帮助。（并不是变暗了，而是噪点太多了）
<div style="display: flex; justify-content: center;">
  <img src="results/nee64cosine.bmp" width="400"/>
  <img src="results/whitdptmis.bmp" width="400"/>
</div>

#### 包围盒和层次结构
在bound.hpp和bvh.hpp。包围盒记录了三个轴上的最小最大值，实现了求交函数。在轴和射线近似平行时特判，否则slab，要求所有轴向上射线都与包围盒有重叠区间。BVH节点包括一个包围盒，标记自己和子节点的总包围盒；左右孩子指针；三角形面片指针的vector，但只有叶子节点才记录，具有懒惰功能。在建树时候，传入一个三角形指针vector，先计算包围盒，再选择包围盒长度最大的轴（Kd树思想），按照重心坐标分量排序，中位截断，给左右子树递归建树。intersect函数中，先看和包围盒是否相交，是则递归查看左右子节点，直到叶节点，再进行遍历。有了包围盒之后，才能正常渲染小兔子：
![](/results/mesh.bmp)
#### 多种采样方法
在漫反射部分，实现了余弦加权半球采样，大大加快了收敛。在glossy中，实现了beckmann采样[Beckmann-Spizzichino 微表面模型采样（上）](https://zhuanlan.zhihu.com/p/74432024)和GGX采样，也很好地提高了效率。这些都在ptutil.hpp。一个例子是（对漫反射的余弦加权平均）：
<div style="display: flex; justify-content: center;">
  <img src="results/nee64.bmp" width="400"/>
  <img src="results/nee64cosine.bmp" width="400"/>
</div>

#### 伽马校正
伽马校正很简单，对每个color施加 $\frac{1}{2.2}$ 的幂即可。
#### 基于 OpenMP 的 CPU 并行加速
由于相机的扫描光线并行，我使用了
```
#pragma omp parallel for schedule(dynamic, 1)。  
```
在蒙特卡洛的像素相加处，我使用了
```
#pragma omp declare reduction(+ : Vector3f : omp_out += omp_in) initializer(omp_priv = Vector3f(0, 0, 0))
#pragma omp parallel for reduction(+ : color)
```
## 验收后新增功能
#### 风格化渲染的后处理部分
在utils.hpp。
论文中说：
- color mapping：先要算光强，根据光强算亮度，clamp亮度，map到gradient value，并且亮度u是rgb平均。于是，我计算每个color的亮度，把它们映射到色带上面：u在0~1，乘以色带条数，再转换成int，使用对应index的色带染色。
![](/results/colormapping.bmp)
- celShading：极化色彩，根据论文所述，`float mu = u < 0.75f ? 0.4 : 0.95;`，颜色乘以mu/u。
![](/results/celshading.bmp)
- saturationMapping：论文是前 n hit，我试着用在1 hit上，Imin = u，cmin = color的最小值，cmax反之，Imax是(color-cmin)/cmax，up是Imax向量的平均，最终把color染色为w * Imin + (1.0f - w) * Imax * u / up。中间为了避免除以0，加了特判插值。
![](/results/saturated.bmp)
- unbiasedMapping：color = c * (color - p) + p，c和p自己定，这里的unbiased应该指的是收敛。
![](/results/unbiased.bmp)
- tieDyeMapping：由于chebyshev，先把color clamp到-1~4，然后调用函数 $r=-\frac{\cos(2\pi(r+1)+1)}{2}$，$g=-\frac{\cos(2.15\pi(g+1.13)+1)}{2}$，$b=-\frac{\cos(2.3\pi(r+1.29)+1)}{2}$。
![](/results/tiedye.bmpf)
#### 参数曲面解析法求交
我们有平面上的曲线$P(\mu),\mu=\frac{y-y_{min}}{y_{max}-y_{min}}$，绕着y轴转动，则有参数曲面$(x(t)\cosθ, y(t), x(t)\sinθ)$，而光线为$o+d*t$，其中o是起始点，d是方向。我从y轴负方向看，从x轴正方向开始，$\theta$ 是顺时针转动的角度，而 $\mu$ 要用来传给曲线让它找对应点。在牛顿法中，我们有一个函数getPoint计算当前$\theta$和$\mu$下二者的导向量，分别是$(-x(t)\sinθ, y(t), x(t)\cosθ)$（上面公式对$\theta$求导）以及曲线在bezier类里面对应点的导向量绕转$\theta$度。同时，返回一个曲线上对应点本身绕转$\theta$度。我们把$\theta$映射回0到$2\pi$，$\mu$clamp到0~1.法向量\(\mathbf{n} = \mathbf{d}_\mu \times \mathbf{d}_\theta\)，而由于误差函数 \(F(t, \theta, \mu) = \text{ray}(t) - \text{surface}(\theta, \mu)\)，对$t、\theta、\mu$求导为d、-dtheta、-dmu，用其组成雅可比矩阵，求逆，施加在牛顿法上一轮结果，即为减去量，这是牛顿法。以下分别为面片和参数法。
<div style="display: flex; justify-content: center;">
  <img src="results/norm.bmp" width="400"/>
  <img src="results/norm2.bmp" width="400"/>
</div>


#### 景深
景深是在照相机上工作的，取焦距focusDist和光圈大小aperture，在focusPoint = center + rayDir * focusDistance处是焦平面，然后焦平面上在光圈大小的圆上随机均匀抽样，在焦平面上有一个offset = dx * horizontal + dy * up在平面上引入随机性，然后把origin更改成center + offset，并且把光线方向改成origin到focusPoint。效果图：
<div style="display: flex; justify-content: center;">
  <img src="results/wlfl.bmp" width="400"/>
  <img src="results/wlfl100.bmp" width="400"/>
</div>