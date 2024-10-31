#include "pch.hpp"

#include "Core/Renderer.h"
#include "OpenGL/glShader.h"
#include "Test/MyTests.h"

// N卡使用独显运行
// extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseMoveCallback(GLFWwindow* window, double x_pos, double y_pos);
void mouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset);
unsigned processKeyboardInput(GLFWwindow* window);
GLFWwindow* appInit();

const std::string RES_FILEPATH(PROJ_RES_PATH);

float g_deltaTime = 0.0f; // 当前帧与上一帧的时间差
float g_lastFrame = 0.0f; // 上一帧的时间

// 硬件输入相关
unsigned int g_scr_width = 1280;
unsigned int g_scr_height = 768;
bool g_firstMouse = true; // 绑定鼠标输入时必须为true
double g_mouse_mov_last_x = g_scr_width / 2.0f, g_mouse_mov_last_y = g_scr_height / 2.0f;
test::Input g_input;
GLFWcursorposfun gp_prev_mouseMovCallback = nullptr;
GLFWscrollfun gp_prev_mouseScrollCallback = nullptr;

test::Test* gp_currentTest = nullptr;

int main()
{
    GLFWwindow* window = appInit();
    GLCore::Renderer renderer(window);
    renderer.imGuiOnAttach();

    test::TestMenu* test_menu = new test::TestMenu(gp_currentTest, window);
    gp_currentTest = test_menu;

#ifdef APPTEST_2D
    test_menu->registerTest<test::TestClearColor>("ClearColor");
    test_menu->registerTest<test::TestTexture2D>("Texture2D");
    test_menu->registerTest<test::TestImgBlend>("ImgBlend");
#endif

#ifdef APPTEST_3D
    test_menu->registerTest<test::TestCamera>("Camera");
    test_menu->registerTest<test::TestBlinnPhong>("Blinn-Phong Model");
    test_menu->registerTest<test::TestMultipleLights>("Multiple Lights");
    test_menu->registerTest<test::TestFrameBuffer>("FrameBuffer (Post-Processing)");
    test_menu->registerTest<test::TestEnvMapping>("Environment Mapping");
    test_menu->registerTest<test::TestShadowMap>("ShadowMap");
    test_menu->registerTest<test::TestNormalMapping>("Normal Mapping");
    test_menu->registerTest<test::TestHDR>("HDR");
    test_menu->registerTest<test::TestBloom>("Bloom");
    test_menu->registerTest<test::TestDeferredRendering>("Deferred Rendering");
    test_menu->registerTest<test::TestSSAO>("SSAO");
    test_menu->registerTest<test::TestPBR>("PBR");
    test_menu->registerTest<test::TestAnimation>("Animation");
#endif

    while (!glfwWindowShouldClose(window))
    {
        // 处理逻辑
        renderer.clear();
        renderer.imGuiNewFrame();

        // 更新DeltaTime
        float currentFrame = glfwGetTime();
        g_deltaTime = currentFrame - g_lastFrame;
        g_lastFrame = currentFrame;

        if (gp_currentTest)
        {
            g_input.keyboardInput = processKeyboardInput(window);
            gp_currentTest->onUpdate(g_deltaTime, g_input);
            g_input.clear();
        }

        // 处理渲染
        if (gp_currentTest)
        {
            gp_currentTest->onRender();
            ImGui::Begin("Main##0001");
            ImGui::SeparatorText("Tests##0001");
            if (gp_currentTest != test_menu && ImGui::Button("<< Back##0001"))
            {
                delete gp_currentTest;
                gp_currentTest = test_menu;
            }
            gp_currentTest->onImGuiRender();
            ImGui::SeparatorText("Frame Rate##0001");
            ImGui::Text("Avg %.3f ms/f (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        renderer.imGuiRender();

        // glfw: 交换颜色缓冲，检查IO等事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    if (gp_currentTest)
    {
        delete gp_currentTest;
        if (gp_currentTest != test_menu)
            delete test_menu;
    }
    
    renderer.imGuiOnDetach();
    // glfw: 正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}

// glfw: 实时调整视口大小的回调函数
void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    g_input.screenWidth = width;
    g_input.screenHeight = height;
}

void mouseMoveCallback(GLFWwindow* window, double x_pos, double y_pos)
{
    if (g_firstMouse) // 这个bool变量初始时是设定为true的
    {
        g_mouse_mov_last_x = x_pos;
        g_mouse_mov_last_y = y_pos;
        g_firstMouse = false;
    }

    // 1. 计算偏移量
    double xOffset = x_pos - g_mouse_mov_last_x;
    double yOffset = g_mouse_mov_last_y - y_pos; // 注意y坐标是从底部往顶部依次增大的
    g_mouse_mov_last_x = x_pos;
    g_mouse_mov_last_y = y_pos;

    g_input.mouseMovXOffset = xOffset;
    g_input.mouseMovYOffset = yOffset;
}

void mouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
    g_input.mouseScrollYOffset = y_offset;
}

unsigned processKeyboardInput(GLFWwindow* window)
{
    // ESC - 退出/进入鼠标移动模式
    static bool is_esc_pressed = false;
    static bool is_mouse_enabled = false;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
        is_esc_pressed = true;
	}
    if (is_esc_pressed && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
    {
        is_esc_pressed = false;
        if (!is_mouse_enabled)
        {
            is_mouse_enabled = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            gp_prev_mouseMovCallback = glfwSetCursorPosCallback(window, mouseMoveCallback);
            gp_prev_mouseScrollCallback = glfwSetScrollCallback(window, mouseScrollCallback);
        }
        else
        {
            is_mouse_enabled = false;
            g_firstMouse = true;

        	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, gp_prev_mouseMovCallback);
            glfwSetScrollCallback(window, gp_prev_mouseScrollCallback);
        }
    }

    // 切换线框模式
    static int toggle_wireframe_mode = 1;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        toggle_wireframe_mode = -toggle_wireframe_mode;
        if (toggle_wireframe_mode == -1)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Others
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        return GLFW_KEY_W;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        return GLFW_KEY_A;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        return GLFW_KEY_S;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        return GLFW_KEY_D;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        return GLFW_KEY_SPACE;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        return GLFW_KEY_LEFT_CONTROL;

    return 0;
}

GLFWwindow* appInit()
{
	GLCore::Logger::init();

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
    GLFWwindow* window = glfwCreateWindow(g_scr_width, g_scr_height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        LOG_CRITICAL(std::format("[{}]: {}", "GLFW", "Failed to create GLFW window"));
        glfwTerminate();
        GL_ASSERT(false);
    }
    glfwMakeContextCurrent(window);
    // glfw: 注册实时调整视口大小的回调函数
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
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

        int maxUniformBlockSize = 0, maxVertexUniformBlocks = 0;
        // 查询最大 UBO 大小
        glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
        // 查询最大可用 UBO 数量
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxVertexUniformBlocks);

        LOG_INFO(std::format("[{0}]: Max gl uniform block size {1} bytes, max vertex uniform block count: {2}",
                             "GL Application", maxUniformBlockSize, maxVertexUniformBlocks));
    }

#ifdef NEW_GL_DEBUG
    GLCore::GLEnableDebugging();
#endif
    return window;
}