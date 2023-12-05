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

auto lengthToEnd = [](const auto& tuple) {
	const auto& [start, length] = tuple;
	return std::tuple{ start, start + length - 1 };
	};

int main() {
	std::fstream input("input.txt");
	std::string line;
	std::getline(input, line);
	auto seeds = (line | std::views::split(std::string(": ")) | std::views::drop(1)).front() | std::views::split(std::string(" ")) | std::views::transform(toLongLong) | std::views::adjacent<2> | std::views::stride(2) | std::views::transform(lengthToEnd) | std::ranges::to<std::map>();
	std::getline(input, line); // skip empty line
	std::map<long long, long long> ranges; // start => end
	std::map<long long, long long> deltas;
	while (std::getline(input, line) or input.eof()) {
		if (line.find(':') != std::string::npos) {
			ranges.clear();
			deltas.clear();
			continue;
		}
		if (line.empty()) {
			long long prev = ranges.begin()->first - 1;
			for (const auto& [first, second] : ranges) {
				if (first != prev + 1) {
					ranges[prev + 1] = first - 1;
					deltas[prev + 1] = 0;
					break;
				}
				prev = second;
			}
			decltype(seeds) newRange;
			for (std::pair<long long, long long> seed : seeds) {
				 auto& [start, end] = seed;
				// seed range outside any tranform ranges
				if (start > std::prev(ranges.end())->second or end < ranges.begin()->first) {
					newRange[start] = end;
					continue;
				}
				// seed range on transform ranges start boundary
				if (start < ranges.begin()->first) {
					newRange[start] = ranges.begin()->first - 1;
					start = ranges.begin()->first;
				}
				// seed range on transform end boundary
				if (end > std::prev(ranges.end())->second) {
					newRange[std::prev(ranges.end())->second + 1] = end;
					end = std::prev(ranges.end())->second;
				}
				for (auto it = std::prev(ranges.upper_bound(start)); it != ranges.upper_bound(end); ++it) {
					long long delta = deltas[it->first];
					newRange[std::max(start, it->first) + delta] = std::min(it->second, end) + delta;
				}

			}
			seeds = newRange;
			if (input.eof())
				break;
			else
				continue;
		}
		const auto& [destination, source, length] = (line | std::views::split(std::string(" ")) | std::views::transform(toLongLong) | std::views::adjacent<3>).front();
		ranges[source] = source + length - 1;
		deltas[source] = destination - source;
		line.clear();
	}
	std::cout << seeds.begin()->first;
}