#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <cassert>
#include <vector>
#include <numeric>

constexpr int DEALER_MAX_SUM{ 17 };
constexpr int MAX_SUM{ 21 };

enum class CardSuit
{
	club,
	diamond,
	heart,
	spade,

	max_suits
};

enum class CardRank
{
	rank_2,
	rank_3,
	rank_4,
	rank_5,
	rank_6,
	rank_7,
	rank_8,
	rank_9,
	rank_10,
	rank_jack,
	rank_queen,
	rank_king,
	rank_ace,

	max_ranks
};

enum class BlackjackResult
{
	player_win,
	dealer_win,
	tie
};

struct Card
{
	CardRank rank{};
	CardSuit suit{};
};

std::string printCard(const Card& card)
{
	std::string cardName{};

	switch (card.rank)
	{
	case (CardRank::rank_2):
		cardName.append("2");
		break;
	case (CardRank::rank_3):
		cardName.append("3");
		break;
	case (CardRank::rank_4):
		cardName.append("4");
		break;
	case (CardRank::rank_5):
		cardName.append("5");
		break;
	case (CardRank::rank_6):
		cardName.append("6");
		break;
	case (CardRank::rank_7):
		cardName.append("7");
		break;
	case (CardRank::rank_8):
		cardName.append("8");
		break;
	case (CardRank::rank_9):
		cardName.append("9");
		break;
	case (CardRank::rank_10):
		cardName.append("10");
		break;
	case (CardRank::rank_jack):
		cardName.append("11");
		break;
	case (CardRank::rank_king):
		cardName.append("12");
		break;
	case (CardRank::rank_queen):
		cardName.append("13");
		break;
	case (CardRank::rank_ace):
		cardName.append("A");
		break;
	default:
		cardName.append("?");
		break;
	}

	switch (card.suit)
	{
	case (CardSuit::club):
		cardName.append("C");
		break;
	case (CardSuit::diamond):
		cardName.append("D");
		break;
	case (CardSuit::heart):
		cardName.append("H");
		break;
	case (CardSuit::spade):
		cardName.append("S");
		break;
	default:
		cardName.append("?");
		break;
	}

	return cardName;
}

using Deck = std::array<Card, 52>;

Deck createDeck()
{
	Deck deck{};

	size_t index{ 0 };

	for (size_t suit{ 0 }; suit < static_cast<int>(CardSuit::max_suits); suit++)
	{
		for (size_t rank{ 0 }; rank < static_cast<int>(CardRank::max_ranks); rank++)
		{
			deck[index].suit = static_cast<CardSuit>(suit);
			deck[index].rank = static_cast<CardRank>(rank);
			++index;
		}
	}

	return deck;
}

void printDeck(const Deck& deck)
{
	for (size_t i{}; i < deck.size(); i++)
	{
		printCard(deck[i]);
		std::cout << ' ';
	}

	std::cout << '\n';
}

void shuffleDeck(Deck& deck)
{
	static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) }; // random

	std::shuffle(deck.begin(), deck.end(), mt);
}

int getCardValue(const Card& card)
{
	switch (card.rank)
	{
	case (CardRank::rank_2):
		return 2;
	case (CardRank::rank_3):
		return 3;
	case (CardRank::rank_4):
		return 4;
	case (CardRank::rank_5):
		return 5;
	case (CardRank::rank_6):
		return 6;
	case (CardRank::rank_7):
		return 7;
	case (CardRank::rank_8):
		return 8;
	case (CardRank::rank_9):
		return 9;
	case (CardRank::rank_10):
		return 10;
	case (CardRank::rank_jack):
		return 10;
	case (CardRank::rank_king):
		return 10;
	case (CardRank::rank_queen):
		return 10;
	case (CardRank::rank_ace):
		return 11;
	default:
		assert(false && "should never happen");
		return -1;
	}
}

int getSum(const std::vector<Card> cards)
{
	int sum{0};
	for (auto& i : cards)
	{
		int currentValue{ getCardValue(i) };
		if (((sum + currentValue) > 21) && (i.rank == CardRank::rank_ace))
		{
			sum += (currentValue - 10);
		}
		else
		{
			sum += currentValue;
		}	
	}
	return sum;
}

BlackjackResult playBlackjack(const Deck& deck)
{
	std::vector<Card> dealer{};
	std::vector<Card> player{};
	
	std::string option{};

	for (size_t it{ 0 }; it < deck.size(); it++)
	{
		if (it == 0)
		{
			dealer.push_back(deck.at(it));
			std::cout << "The dealer score is: " << getSum(dealer) << '\n';
			continue;
		}

		if (it == 1 || it == 2)
		{
			player.push_back(deck.at(it));
			continue;
		}

		if (getSum(player) > MAX_SUM)
		{
			std::cout << "Current score DSADASDASDASD: " << getSum(player) << '\n';
			return BlackjackResult::dealer_win;
		}

		if (getSum(player) == MAX_SUM)
		{
			std::cout << "Current score: " << getSum(player) << '\n';
			return BlackjackResult::player_win;
		}

		if (option == "stand")
		{
			std::cout << "The dealer score is: " << getSum(dealer) << '\n';
			std::cout << "The dealer has drawn: " << '(' << printCard(deck.at(it)) << ") " << '\n';

			dealer.push_back(deck.at(it));

			if (getSum(dealer) > MAX_SUM)
			{
				std::cout << "The deal has a score of: " << getSum(dealer) << " which is above 21" << '\n';
				return BlackjackResult::player_win;
			}

			if (getSum(dealer) >= DEALER_MAX_SUM)
			{
				std::cout << "The deal has a score of: " << getSum(dealer) << " and you of: " << getSum(player) << '\n';
				if (getSum(dealer) > getSum(player))
				{
					return BlackjackResult::dealer_win;
				}
				if (getSum(dealer) == getSum(player))
				{
					return BlackjackResult::tie;
				}
				return BlackjackResult::player_win;
			}
			continue;
		}

		std::cout << "Current score: " << getSum(player) << '\n';
		std::cout << "Do you want to hit or stand?: ";
		std::cin >> option;

		if (option == "stand")
		{
			std::cout << '\n';
			continue;
		}

		player.push_back(deck.at(it));

		std::cout << "You draw: " << '(' << printCard(deck.at(it)) << ") " << '\n';
	}
}

int main()
{
	Deck deck{ createDeck() };

	shuffleDeck(deck);

	BlackjackResult game{ playBlackjack(deck) };

	if (game == BlackjackResult::player_win)
		std::cout << "you won!" << '\n';
	else if (game == BlackjackResult::tie)
		std::cout << "it's tie!" << '\n';
	else
		std::cout << "you lost!" << '\n';
}