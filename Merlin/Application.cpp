#include <Merlin/Application.h>
#include <iostream>

namespace Merlin {
	void Application::Run() {
		logger.info("MerlinGL Engine is flying!!!");
		for (int i = 0; ; i = (i + 1) % 1000000) {
			logger.info("loop ", i);
		}
	}
}