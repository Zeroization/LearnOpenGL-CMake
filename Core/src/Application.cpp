#include "pch.hpp"

#include "Renderer.h"
#include "Shader.h"
#include "OpenGL/glTexture.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "Tests/Test.h"
#include "Tests/TestClearColor.h" 

#include <filesystem>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
GLFWwindow* appInit();
void imGuiOnAttach(GLFWwindow* window);
void imGuiOnDetach();
void imGuiNewFrame();
void imGuiRender();

// 一些参数设置
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const std::string RES_FILEPATH(PROJ_RES_PATH);

int main()
{
    GLFWwindow* window = appInit();

    // 提供一个作用域, 让VBO等类在OpenGL上下文关闭前被析构
    {
        // Data
        float vertexData[] = {
            100.0f, 100.0f, 0.0f, 0.0f, // 0
            200.0f, 100.0f, 1.0f, 0.0f,  // 1
            200.0f, 200.0f, 1.0f, 1.0f,    // 2
            100.0f, 200.0f, 0.0f, 1.0f   // 3
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
        };

        // 设置alpha通道如何混合
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        // 配置相关属性
        GLVertexArray va;
        GLVertexBuffer vb(vertexData, 4 * 4 * sizeof(float));

        GLVertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        va.addVBO(vb, layout);

        GLIndexBuffer ib(indices, 6);

        Shader shader(RES_FILEPATH + "/Shaders/test.shader");
        shader.bind();
        shader.setUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        GLTexture texture(RES_FILEPATH + "/Textures/awesomeface.png");
        texture.bind();
        shader.setUniform1i("u_Texture", 0);
         
        // 配置完后记得解绑
        va.unbind();
        vb.unbind();
        ib.unbind();
        shader.unbind();

        Renderer renderer;
        imGuiOnAttach(window);

        test::Test* p_currentTest = nullptr;
        test::TestMenu* test_menu = new test::TestMenu(p_currentTest);
        p_currentTest = test_menu;

        test_menu->registerTest<test::TestClearColor>("clear color");

        while (!glfwWindowShouldClose(window))
        {
            // 处理逻辑
            renderer.clear();
            imGuiNewFrame();

            // 处理渲染
            if (p_currentTest)
            {
                p_currentTest->onUpdate(0.0f);
                p_currentTest->onRender();
                ImGui::Begin("Tests");
                if (p_currentTest != test_menu && ImGui::Button("back"))
                {
                    delete p_currentTest;
                    p_currentTest = test_menu;
                }
                p_currentTest->onImGuiRender();
                ImGui::End();
            }

            imGuiRender();

            // glfw: 交换颜色缓冲，检查IO等事件
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        delete p_currentTest;
        if (p_currentTest != test_menu)
            delete test_menu;

    }

    imGuiOnDetach();

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

void imGuiOnAttach(GLFWwindow* window)
{
    // ImGui: 初始化本体
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // 允许键盘控制
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // 允许Docking
    io.Fonts->AddFontFromFileTTF((RES_FILEPATH + "/Fonts/OPPOSans M.ttf").c_str(), 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

    // ImGui: 初始化渲染后端
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void imGuiOnDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void imGuiNewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void imGuiRender()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
