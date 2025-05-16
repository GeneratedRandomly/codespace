# 图形学 PA1 报告
### 仲嘉暄 计35 2023010812
## 代码逻辑
总体上，代码逻辑使用的是课件和指导文件的公式。
### Camera
`generateRay` 稍加优化，变成数字乘向量再相加，消掉了负号，减少了运算量。  
问题：不明白所给公式为什么是对的，后来在AI帮助下理解了小孔成像的物理正确性。
### group
遍历求交。
### material
直接照抄 $\text{Phong}$ 模型公式。  
问题：一开始使用 $\text{Blinn-Phong}$ 模型，造成了错误。
### plane
有所修改：公式分子从 `-(d+n*r0)` 改为 `d - Vector3f::dot(normal, r0)`  
问题：这里平面的定义方式为 `ax+by+cz=d` 和课件不同，开始时没有注意到。
### sphere
有所修改：先计算所需量，再根据光源位置修改 `t` 值。  
问题：由于方向向量单位化，不能使用 `t` 直接带入 `r.pointAtParameter(t)`，而要自己求 `(direction * t - OC).normalized()`。
### triangle
这里的问题是，取方向向量的时候不能单位化。实际上，取单位向量就不应该单位化，如果单位化要自己承担代价（如 `sphere` 那样）。
## Honor Code
参考了[学长代码](https://github.com/Guangxuan-Xiao/THU-Computer-Graphics-2020/tree/master/PA1)。但是，许多部分与其实现不同。
## Bugs
未发现bug。
## 建议
建议对透视相机加一点介绍，或者优化一下配图，按照小孔成像来，便于实现 `fx`、`fy`。