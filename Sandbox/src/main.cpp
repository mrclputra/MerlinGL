#include <Merlin/Core.h>

int main() {
	auto engine = std::make_unique<Merlin::Engine>();
	engine->Run();
	return 0;
}