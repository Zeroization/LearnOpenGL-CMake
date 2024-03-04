#include "pch.hpp"

#include "Renderer.h"
#include "Shader.h"

#include "Test/Test.h"
#include "Test/TestClearColor.h" 
#include "Test/TestTexture2D.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
GLFWwindow* appInit();

// 一些参数设置
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const std::string RES_FILEPATH(PROJ_RES_PATH);

int main()
{
    GLFWwindow* window = appInit();
    Renderer renderer(window);
    renderer.imGuiOnAttach();

    test::Test* p_currentTest = nullptr;
    test::TestMenu* test_menu = new test::TestMenu(p_currentTest);
    p_currentTest = test_menu;

    test_menu->registerTest<test::TestClearColor>("ClearColor");
    test_menu->registerTest<test::TestTexture2D>("Texture2D");

    while (!glfwWindowShouldClose(window))
    {
        // 处理逻辑
        renderer.clear();
        renderer.imGuiNewFrame();

        // 处理渲染
        if (p_currentTest)
        {
            p_currentTest->onUpdate(0.0f);
            p_currentTest->onRender();
            ImGui::Begin("Tests");
            if (p_currentTest != test_menu && ImGui::Button("返回"))
            {
                delete p_currentTest;
                p_currentTest = test_menu;
            }
            p_currentTest->onImGuiRender();
            ImGui::End();
        }

        renderer.imGuiRender();

        // glfw: 交换颜色缓冲，检查IO等事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete p_currentTest;
    if (p_currentTest != test_menu)
        delete test_menu;

    renderer.imGuiOnDetach();
    // glfw: 正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}

// glfw: 实时调整视口大小的回调函数
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLFWwindow* appInit()
{
    Logger::init();

    // glfw: 初始化与配置工作（告知OpenGL的版本和工作方式）
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw: 创建GLFW窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        LOG_CRITICAL(std::format("[{}]: {}", "GLFW", "Failed to create GLFW window"));
        glfwTerminate();
        GL_ASSERT(false);
    }
    glfwMakeContextCurrent(window);
    // glfw: 注册实时调整视口大小的回调函数
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    // glfw: 垂直同步
    glfwSwapInterval(1);

    // glad: 初始化GLAD，以便使用OpenGL函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_CRITICAL(std::format("[{0}]: {1}", "GLAD", "Failed to initialize GLAD"));
        GL_ASSERT(false);
    }
    else
    {
        const char* gl_version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
		LOG_INFO(std::format("[{0}]: {1}", "GL Application", gl_version));
    }

    GLDebug::GLEnableDebugging();

    return window;
}


