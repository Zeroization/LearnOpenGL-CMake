# LearnOpenGL-CMake
## 项目简介

**项目描述**：学习应用 `OpenGL` 、图形学和其他游戏引擎相关知识编写的项目，使用 **C++** 编写，可以让用户编写测试代码进行简易图形渲染等功能。项目分为两部分，**核心部分**封装了项目的关键组件(如摄像机、光源、可渲染对象和 `OpenGL` 对象等)；**测试部分**则可视化展示项目特点(如多光源多物体渲染测试等)。

**项目特点**：

- **渲染模块**：简单封装 `OpenGL` ，封装渲染器核心部件（如Camera，GLObject，Material类等）；支持前向渲染管线（默认），延迟渲染管线；支持 **平行光, 点光源, 聚光灯** 三种光源，简单**Shadow Mapping**， **Blinn-Phong光照模型**和 **PBR**渲染模型；支持 **天空盒**，**法线&视差映射**，**HDR**，**SSAO**，**Bloom**等渲染技术。
- **动画模块**：使用Assimp库读取模型的蒙皮骨骼动画信息，使用SSBO存储动画信息，实现**动画播放**；实现**简单动画混合**（简单线性混合，CrossFading，Partial Skeleton混合，Additive混合）；实现**简单IK**（Two-Bone IK， CCD IK， FABRIK）；

## 图片展示

### 动画模块

![动画的读取与播放](https://cdn.keepfightinghxz.xyz/images/202410261807668.gif)

![简单线性混合 - 当前动画姿势与绑定姿势](https://cdn.keepfightinghxz.xyz/images/202411050000004.gif)

![CrossFading过渡混合](https://cdn.keepfightinghxz.xyz/images/202411091614213.gif)

![部分骨骼混合 - 上半身为动画A, 下半身为动画B](https://cdn.keepfightinghxz.xyz/images/202411220042211.gif)

![附加混合 - 蹲姿动画上附加装弹动画](https://cdn.keepfightinghxz.xyz/images/202411222317350.gif)

### 渲染模块

![多光源多物体渲染](https://cdn.keepfightinghxz.xyz/images/202403272350228.png)

![图片后处理](https://cdn.keepfightinghxz.xyz/images/202403272351031.png)

![简单PBR](https://cdn.keepfightinghxz.xyz/images/202409221926084.png)

![SSAO](https://cdn.keepfightinghxz.xyz/images/202409190038332.png)

![延迟渲染](https://cdn.keepfightinghxz.xyz/images/202409161350672.png)

![泛光](https://cdn.keepfightinghxz.xyz/images/202409141042146.png)

![视差映射](https://cdn.keepfightinghxz.xyz/images/202409061509643.png)

![Shadow Mapping](https://cdn.keepfightinghxz.xyz/images/202409301440126.png)

## 项目结构与构建
项目结构如下，包含第三方库`Dependencies`，项目核心组件`LearnOpenGL-Core`和编写测试用例的`LearnOpenGL-Tests`。
```
├─Dependencies
│  ├─glad
│  ├─glfw
│  ├─glm
│  ├─imgui
│  ├─spdlog
│  └─stb_image
├─LearnOpenGL-Core
│  ├─include
│  └─src
├─LearnOpenGL-Tests
│  ├─include
│  └─src
└─Res
    ├─Fonts
    ├─Shaders
    └─Textures
```
要想构建此项目，可以直接用VS2022打开此文件，然后在`LearnOpenGL-Tests`的`Application.cpp`中单击运行即可。

## 参考资料
- [TheCherno的OpenGL](https://www.bilibili.com/video/BV1Ni4y1o7Au)
- [LearnOpenGL(中文版)](https://learnopengl-cn.github.io/)