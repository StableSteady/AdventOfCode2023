#include <cctype>
#include <iostream>
#include <fstream>
#include <ranges>

auto digit = [](const char c) { return std::isdigit(c); };
auto toInt = [](const char c) { return static_cast<int>(c - '0'); };

int main() {
	std::ifstream input("input.txt");
	int sum = 0;
	for (const auto& s : std::views::istream<std::string>(input)) {
		auto digits = s | std::views::filter(digit) | std::views::transform(toInt);
		sum += digits.front() * 10 + digits.back();
	}
	std::cout << sum << '\n';
}