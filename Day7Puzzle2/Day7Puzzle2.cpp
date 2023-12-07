#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

enum class HandType {
	High = 0,
	One,
	Two,
	Three,
	Full,
	Four,
	Five
};

struct Hand {
	int bid = 0;
	std::string hand;
	HandType type;
	static std::unordered_map<char, int> cardStrength;
	friend std::istream& operator>>(std::istream& is, Hand& hand) {
		is >> hand.hand >> hand.bid;
		hand.classify();
		return is;
	};
	void classify() {
		std::unordered_map<char, int> cardCount;
		bool hasJoker = false;
		for (char card : hand) {
			cardCount[card]++;
			hasJoker = hasJoker or card == 'J';
		}
		switch (cardCount.size()) {
		case 5:
			type = hasJoker ? HandType::One : HandType::High;
			break;
		case 4:
			type = hasJoker ? HandType::Three : HandType::One;
			break;
		case 3:
			if (hasJoker) {
				bool three = false;
				for (const auto& [card, count] : cardCount) {
					if (count == 3)
						three = true;
				}
				type = three or cardCount['J'] == 2 ? HandType::Four : HandType::Full;
			} else {
				for (const auto& [card, count] : cardCount) {
					if (count == 3)
						type = HandType::Three;
					if (count == 2)
						type = HandType::Two;
				}
			}
			break;
		case 2:
			if (hasJoker) {
				type = HandType::Five;
			} else {
				for (const auto& [card, count] : cardCount) {
					if (count == 4)
						type = HandType::Four;
					if (count == 3)
						type = HandType::Full;
				}
			}
			break;
		case 1:
			type = HandType::Five;
			break;
		}
	}
	friend bool operator<(const Hand& lhs, const Hand& rhs) {
		if (lhs.type != rhs.type) {
			return lhs.type < rhs.type;
		}
		for (const auto& [lHand, rHand] : std::views::zip(lhs.hand, rhs.hand)) {
			if (Hand::cardStrength[lHand] != Hand::cardStrength[rHand])
				return Hand::cardStrength[lHand] < Hand::cardStrength[rHand];
		}
		return false;
	}
};

std::unordered_map<char, int> Hand::cardStrength = { {'A', 13}, {'K', 12}, {'Q', 11}, {'T', 10}, {'9', 9}, {'8', 8}, {'7', 7}, {'6', 6}, {'5', 5}, {'4', 4}, {'3', 3}, {'2', 2}, {'J', 1} };

auto toWinnings = [](const auto& tuple) {
	const auto& [rank, hand] = tuple;
	return (rank + 1) * hand.bid;
	};

int main() {
	std::ifstream input("input.txt");
	std::vector<Hand> hands = std::views::istream<Hand>(input) | std::ranges::to<std::vector>();
	std::ranges::sort(hands, std::less{});
	std::cout << *std::ranges::fold_left_first(hands | std::views::enumerate | std::views::transform(toWinnings), std::plus<long long>());
}