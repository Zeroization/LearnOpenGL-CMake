#include "Core/Renderer.h"

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

namespace GLCore
{
    Renderer::Renderer(GLFWwindow* window)
        : m_window(window), m_resPath(PROJ_RES_PATH)
    {
    }

    Renderer::~Renderer()
    {

    }

    void Renderer::draw(const GLVertexArray& va, int pointsCount, const GLShader& shader) const
    {
        shader.bind();
        va.bind();
        GLCall(glDrawArrays(GL_TRIANGLES, 0, pointsCount));
    }

    void Renderer::draw(const GLVertexArray& va, const GLIndexBuffer& ib, const GLShader& shader) const
    {
        shader.bind();
        va.bind();
        ib.bind();
        GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
    }

    void Renderer::clear() const
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void Renderer::imGuiOnAttach() const
    {
        if (m_window == nullptr)
        {
            LOG_CRITICAL("glfw_window is nullptr!");
            GL_ASSERT(false);
        }

        // ImGui: 初始化本体
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // 允许键盘控制
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // 允许Docking
        io.Fonts->AddFontFromFileTTF((m_resPath + "/Fonts/OPPOSans M.ttf").c_str(), 18.0f,
                                     nullptr, io.Fonts->GetGlyphRangesChineseFull());

        // ImGui: 初始化渲染后端
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init();
    }

    void Renderer::imGuiOnDetach() const
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Renderer::imGuiNewFrame() const
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Renderer::imGuiRender() const
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}