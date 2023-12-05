#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

auto toLongLong = [](const auto& s) {
	std::string_view num(s);
	long long val{};
	std::from_chars(num.data(), num.data() + num.size(), val);
	return val;
	};

int main() {
	std::ifstream input("input.txt");
	std::string line;
	std::getline(input, line);
	auto seeds = (line | std::views::split(std::string(": ")) | std::views::drop(1)).front() | std::views::split(std::string(" ")) | std::views::transform(toLongLong) | std::ranges::to<std::vector>();
	std::getline(input, line); // skip empty line
	std::map<long long, std::pair<long long, long long>> sourceToDestination; // first = destination, second = length
	while (std::getline(input, line)) {
		if (line.find(':') != std::string::npos) {
			sourceToDestination.clear();
			continue;
		}
		if (line.empty()) {
			for (long long& seed : seeds) {
				auto it = sourceToDestination.upper_bound(seed);
				if (it == sourceToDestination.begin())
					continue;
				--it;
				if (long long delta = seed - (*it).first; delta >= (*it).second.second) {
					continue;
				} else {
					seed = (*it).second.first + delta;
				}
			}
			if (input.eof())
				break;
			else
				continue;
		}
		const auto& [destination, source, length] = (line | std::views::split(std::string(" ")) | std::views::transform(toLongLong) | std::views::adjacent<3>).front();
		sourceToDestination[source] = { destination, length };
		line.clear();
	}
	std::cout << *std::min_element(seeds.begin(), seeds.end());
}