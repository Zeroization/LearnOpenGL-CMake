# LearnOpenGL-CMake
## ��Ŀ���

**��Ŀ����**��ѧϰӦ�� `OpenGL` ��ͼ��ѧ��������Ϸ�������֪ʶ��д����Ŀ��ʹ�� **C++** ��д���������û���д���Դ�����м���ͼ����Ⱦ�ȹ��ܡ���Ŀ��Ϊ�����֣�**���Ĳ���**��װ����Ŀ�Ĺؼ����(�����������Դ������Ⱦ����� `OpenGL` �����)��**���Բ���**����ӻ�չʾ��Ŀ�ص�(����Դ��������Ⱦ���Ե�)��

**��Ŀ�ص�**��

- **��Ⱦģ��**���򵥷�װ `OpenGL` ����װ��Ⱦ�����Ĳ�������Camera��GLObject��Material��ȣ���֧��ǰ����Ⱦ���ߣ�Ĭ�ϣ����ӳ���Ⱦ���ߣ�֧�� **ƽ�й�, ���Դ, �۹��** ���ֹ�Դ����**Shadow Mapping**�� **Blinn-Phong����ģ��**�� **PBR**��Ⱦģ�ͣ�֧�� **��պ�**��**����&�Ӳ�ӳ��**��**HDR**��**SSAO**��**Bloom**����Ⱦ������
- **����ģ��**��ʹ��Assimp���ȡģ�͵���Ƥ����������Ϣ��ʹ��SSBO�洢������Ϣ��ʵ��**��������**��ʵ��**�򵥶������**�������Ի�ϣ�CrossFading��Partial Skeleton��ϣ�Additive��ϣ���ʵ��**��IK**��Two-Bone IK�� CCD IK�� FABRIK����

## ͼƬչʾ

### ����ģ��

![�����Ķ�ȡ�벥��](https://cdn.keepfightinghxz.xyz/images/202410261807668.gif)

![�����Ի�� - ��ǰ���������������](https://cdn.keepfightinghxz.xyz/images/202411050000004.gif)

![CrossFading���ɻ��](https://cdn.keepfightinghxz.xyz/images/202411091614213.gif)

![���ֹ������ - �ϰ���Ϊ����A, �°���Ϊ����B](https://cdn.keepfightinghxz.xyz/images/202411220042211.gif)

![���ӻ�� - ���˶����ϸ���װ������](https://cdn.keepfightinghxz.xyz/images/202411222317350.gif)

### ��Ⱦģ��

![���Դ��������Ⱦ](https://cdn.keepfightinghxz.xyz/images/202403272350228.png)

![ͼƬ����](https://cdn.keepfightinghxz.xyz/images/202403272351031.png)

![��PBR](https://cdn.keepfightinghxz.xyz/images/202409221926084.png)

![SSAO](https://cdn.keepfightinghxz.xyz/images/202409190038332.png)

![�ӳ���Ⱦ](https://cdn.keepfightinghxz.xyz/images/202409161350672.png)

![����](https://cdn.keepfightinghxz.xyz/images/202409141042146.png)

![�Ӳ�ӳ��](https://cdn.keepfightinghxz.xyz/images/202409061509643.png)

![Shadow Mapping](https://cdn.keepfightinghxz.xyz/images/202409301440126.png)

## ��Ŀ�ṹ�빹��
��Ŀ�ṹ���£�������������`Dependencies`����Ŀ�������`LearnOpenGL-Core`�ͱ�д����������`LearnOpenGL-Tests`��
```
����Dependencies
��  ����glad
��  ����glfw
��  ����glm
��  ����imgui
��  ����spdlog
��  ����stb_image
����LearnOpenGL-Core
��  ����include
��  ����src
����LearnOpenGL-Tests
��  ����include
��  ����src
����Res
    ����Fonts
    ����Shaders
    ����Textures
```
Ҫ�빹������Ŀ������ֱ����VS2022�򿪴��ļ���Ȼ����`LearnOpenGL-Tests`��`Application.cpp`�е������м��ɡ�

## �ο�����
- [TheCherno��OpenGL](https://www.bilibili.com/video/BV1Ni4y1o7Au)
- [LearnOpenGL(���İ�)](https://learnopengl-cn.github.io/)