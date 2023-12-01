#include <cctype>
#include <iostream>
#include <fstream>
#include <optional>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>

constexpr int MIN_LEN = 3;
constexpr int MAX_LEN = 5;

auto notNull = [](const std::optional<int>& optional) { return optional != std::nullopt; };
auto optionalToInt = [](const std::optional<int>& optional) { return optional.value(); };

int main() {
	std::ifstream input("input.txt");
	std::unordered_map<std::string, int> wordToDigit{ {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9} };
	int sum = 0;
	for (const auto& s : std::views::istream<std::string>(input)) {
		auto toOptionalInt = [&s, &wordToDigit](const std::tuple<std::ptrdiff_t, char>& t) -> std::optional<int> {
			const auto& [index, ch] = t;
			if (std::isdigit(ch)) {
				return static_cast<int>(ch - '0');
			} else {
				for (int i = MIN_LEN; i <= MAX_LEN and std::cmp_less_equal(index + i, s.size()); i++) {
					std::string word = s.substr(index, i);
					if (wordToDigit.contains(word)) {
						return wordToDigit[word];
					}
				}
				return std::nullopt;
			}
		};
		auto digits = s | std::views::enumerate | std::views::transform(toOptionalInt) | std::views::filter(notNull) | std::views::transform(optionalToInt);
		sum += digits.front() * 10 + digits.back();
	}
	std::cout << sum << '\n';
}