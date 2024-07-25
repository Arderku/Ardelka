#include <cmath>
#include "EditorStyle.h"

namespace {
    // Define the main color and background color parameters
    const ImVec4 MAIN_COLOR = ImVec4(0.055f, 0.412f, 0.443f, 1.00f); // Main color (converted from hex 0e6971)
    const ImVec4 BACKGROUND_COLOR = ImVec4(0.1686f, 0.1765f, 0.1882f, 1.00f); // Dark background color

    // Function to create a brighter version of a color
    ImVec4 Brighter(const ImVec4& color, float factor) {
        return ImVec4(
                fmin(color.x + factor, 1.0f),
                fmin(color.y + factor, 1.0f),
                fmin(color.z + factor, 1.0f),
                color.w
        );
    }

    const ImVec4 BRIGHTER_MAIN_COLOR = Brighter(MAIN_COLOR, 0.1f);
    const ImVec4 BRIGHTEST_MAIN_COLOR = Brighter(MAIN_COLOR, 0.2f);
}

void EditorStyle::ApplyStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.ChildRounding = 5.0f;
    style.FrameRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 5.0f;
    style.ScrollbarRounding = 5.0f;
    style.FramePadding = ImVec2(8, 10);
    style.ItemSpacing = ImVec2(10, 10);
    style.WindowPadding = ImVec2(10, 10);

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
    colors[ImGuiCol_WindowBg] = BACKGROUND_COLOR;
    colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.27f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.30f, 0.33f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = MAIN_COLOR;
    colors[ImGuiCol_ScrollbarGrabHovered] = BRIGHTER_MAIN_COLOR;
    colors[ImGuiCol_ScrollbarGrabActive] = BRIGHTEST_MAIN_COLOR;
    colors[ImGuiCol_CheckMark] = MAIN_COLOR;
    colors[ImGuiCol_SliderGrab] = MAIN_COLOR;
    colors[ImGuiCol_SliderGrabActive] = BRIGHTER_MAIN_COLOR;
    colors[ImGuiCol_Button] = MAIN_COLOR;
    colors[ImGuiCol_ButtonHovered] = BRIGHTER_MAIN_COLOR;
    colors[ImGuiCol_ButtonActive] = BRIGHTEST_MAIN_COLOR;
    colors[ImGuiCol_Header] = MAIN_COLOR;
    colors[ImGuiCol_HeaderHovered] = BRIGHTER_MAIN_COLOR;
    colors[ImGuiCol_HeaderActive] = BRIGHTEST_MAIN_COLOR;
    colors[ImGuiCol_Separator] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.28f, 0.29f, 0.30f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.34f, 0.35f, 0.36f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = MAIN_COLOR;
    colors[ImGuiCol_ResizeGripHovered] = BRIGHTER_MAIN_COLOR;
    colors[ImGuiCol_ResizeGripActive] = BRIGHTEST_MAIN_COLOR;
    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
    colors[ImGuiCol_TabHovered] = BACKGROUND_COLOR;
    colors[ImGuiCol_TabActive] = BACKGROUND_COLOR;
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.16f, 0.17f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = BACKGROUND_COLOR;
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = BRIGHTER_MAIN_COLOR;
    colors[ImGuiCol_DragDropTarget] = MAIN_COLOR;
    colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
}
