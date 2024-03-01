#include "pch.hpp"

#include "Renderer.h"
#include "Shader.h"
#include "OpenGL/glTexture.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include <filesystem>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// 一些参数设置
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const std::string RES_FILEPATH(PROJ_RES_PATH);

int main()
{
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
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // glfw: 注册实时调整视口大小的回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfw: 垂直同步
    glfwSwapInterval(1);

    // glad: 初始化GLAD，以便使用OpenGL函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
	else
	{
        std::cout << glGetString(GL_VERSION) << std::endl;
	}

    // 启用自定义debug
    int flags;
    GLCall(glGetIntegerv(GL_CONTEXT_FLAGS, &flags));
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(GLDebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

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

        glm::vec3 translate(0.0f, 0.0f, 0.0f);
        while (!glfwWindowShouldClose(window))
        {
            // 处理逻辑
            renderer.clear();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 处理渲染
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translate);
            glm::mat4 view(1.0f);
            glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH),
                                        0.0f, static_cast<float>(SCR_HEIGHT),
                                        -1.0f, 1.0f);

            // ImGui: 渲染组件
            {
                bool test_alive = false;
                ImGui::Begin("Test", &test_alive, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);
                if (ImGui::BeginMenuBar())
                {
                    if (ImGui::BeginMenu("File"))
                    {
                        if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
                        if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenuBar();
                }
                ImGui::SliderFloat3("position", &translate.x, -200.0f, 400.0f);
                ImGui::Text("帧率: %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            shader.bind();
            shader.setUniformMat4f("u_MVP", proj * view * model);
            renderer.draw(va, ib, shader);

            

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // glfw: 交换颜色缓冲，检查IO等事件
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw: 正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}

// glfw: 实时调整视口大小的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
