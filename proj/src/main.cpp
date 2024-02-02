#include "pch.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// 一些参数设置
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: 初始化与配置工作（告知OpenGL的版本和工作方式）
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 如果是MacOS，需要有下面这一行
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw: 创建GLFW窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // glfw: 注册实时调整视口大小的回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: 初始化GLAD，以便使用OpenGL函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // glfw: 交换颜色缓冲，检查IO等事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: 正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}

// glfw: 实时调整视口大小的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
