#include "InterfaceStyle.h"

void InterfaceStyle::setThemeBlue(ImGuiStyle & style)
{
	style.WindowFillAlphaDefault = 9.6f;
	style.FrameRounding = 0.2f;
	style.WindowRounding = 0.f;
	style.GrabRounding = 3.f;
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.01f, 0.01f, 0.03f, 0.99f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.16f, 0.16f, 0.22f, 1.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.16f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.33f, 0.53f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.31f, 0.55f, 0.70f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.02f, 0.03f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.02f, 0.03f, 0.07f, 0.88f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.04f, 0.05f, 0.10f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.13f, 0.13f, 0.13f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.04f, 0.03f, 0.05f, 0.84f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.35f, 0.36f, 0.48f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.47f, 0.47f, 0.47f, 0.69f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.24f, 0.53f, 0.87f, 0.50f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.53f, 0.87f, 0.50f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.24f, 0.53f, 0.87f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.13f, 0.24f, 0.42f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.17f, 0.28f, 0.46f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.09f, 0.46f, 0.68f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.09f, 0.46f, 0.90f, 0.44f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.56f, 0.87f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.39f, 0.67f, 1.00f, 0.80f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.10f, 0.17f, 0.28f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.34f, 0.48f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.36f, 0.64f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.28f, 0.28f, 0.30f, 1.00f);
}
