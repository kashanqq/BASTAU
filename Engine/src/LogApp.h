#pragma once
#include <IrrIMGUI/IrrIMGUI.h>

struct LogApp
{
	ImGuiTextBuffer Buf;
	bool ScrollToBottom;
	void Clear()
	{
		Buf.clear();
	}

	void AddLog(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		Buf.appendv(fmt, args);
		va_end(args);
		ScrollToBottom = true;
	}

	void Draw(const char* title, bool& isCollapsed, bool* p_opened = NULL, ImGuiWindowFlags flag = 0)
	{
		ImGui::Begin(title, p_opened, flag);
		isCollapsed = ImGui::IsWindowCollapsed();
		if (ImGui::Button("Clear")) Clear();
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::Separator();
		ImGui::BeginChild("scrolling");
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
		if (copy) ImGui::LogToClipboard();
		ImGui::TextUnformatted(Buf.begin());
		if (ScrollToBottom)
			ImGui::SetScrollHere(1.0f);
		ScrollToBottom = false;
		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::End();
	}
};
