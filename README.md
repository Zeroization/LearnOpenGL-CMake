# LearnOpenGL-CMake
## 项目简介
这是使用CMake构建的LearnOpenGL项目，它对OpenGL中的常见API进行一定封装，也实现了一些简单的功能类和一些图形学（渲染方向）技术。
Ps：项目网站[在这里(TODO)]()
## 项目实现的技术点
### 工具方面
TODO
例：xxx类，怎么这么了

### 图形技术方面
TODO
例：xxxx技术，在Testxxxxx.cpp中有体现，最好配上图

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