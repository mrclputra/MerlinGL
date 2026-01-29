#pragma once
#include <Merlin.h>

#include "Widgets/Console.h"
#include "Widgets/Profiler.h"

class Editor : public Merlin::Layer {
public:
	Editor();
	void OnGuiRender() override;

private:
	Merlin::Console m_Console;
	Merlin::Profiler m_Profiler;
};