#pragma once
#include <Merlin.h>
#include <Merlin/Gui/Widgets/Console.h>
#include <Merlin/Gui/Widgets/Profiler.h>

class Editor : public Merlin::Layer {
public:
	Editor() : Merlin::Layer("Editor") {}
	void OnGuiRender() override;

private:
	Merlin::Console m_Console;
	Merlin::Profiler m_Profiler;
};