#include <progress_bar.h>
#include <iostream>
#include <chrono>
#include <ranges>
#include <thread>

int main() {
	using namespace std::chrono_literals;
	for (int i : prhrs::prhrs(std::ranges::iota_view{0, 100})) {
		std::this_thread::sleep_for(50ms);
	}
	std::cout << '\n';
}
