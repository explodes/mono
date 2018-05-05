#include <unistd.h>
#include <iostream>
#include <vector>
#include "hoist/hoist.h"

static const int numRanks = 4;
static const int numCards = 13;
static const int dealerStandsOn = 17;
static const int maxScore = 21;
static const unsigned int dealerWaitMicros = 1000 * 500;

static const char *cards[][4]{
    // Two of Spades	Two of Hearts	Two of Diamonds	Two of Clubs
    {"\U0001F0A2", "\U0001F0B2", "\U0001F0C2", "\U0001F0D2"},
    // Three of Spades	Three of Hearts	Three of Diamonds
    // Three of Clubs
    {"\U0001F0A3", "\U0001F0B3", "\U0001F0C3", "\U0001F0D3"},
    // Four of Spades	Four of Hearts	Four of Diamonds	Four of Clubs
    {"\U0001F0A4", "\U0001F0B4", "\U0001F0C4", "\U0001F0D4"},
    // Five of Spades	Five of Hearts	Five of Diamonds	Five of Clubs
    {"\U0001F0A5", "\U0001F0B5", "\U0001F0C5", "\U0001F0D5"},
    // Six of Spades	Six of Hearts	Six of Diamonds	Six of Clubs
    {"\U0001F0A6", "\U0001F0B6", "\U0001F0C6", "\U0001F0D6"},
    // Seven of Spades	Seven of Hearts	Seven of Diamonds
    // Seven of Clubs
    {"\U0001F0A7", "\U0001F0B7", "\U0001F0C7", "\U0001F0D7"},
    // Eight of Spades	Eight of Hearts	Eight of Diamonds
    // Eight of Clubs
    {"\U0001F0A8", "\U0001F0B8", "\U0001F0C8", "\U0001F0D8"},
    // Nine of Spades	Nine of Hearts	Nine of Diamonds	Nine of Clubs
    {"\U0001F0A9", "\U0001F0B9", "\U0001F0C9", "\U0001F0D9"},
    // Ten of Spades	Ten of Hearts	Ten of Diamonds	Ten of Clubs
    {"\U0001F0AA", "\U0001F0BA", "\U0001F0CA", "\U0001F0DA"},
    // Jack of Spades	Jack of Hearts	Jack of Diamonds	Jack of Clubs
    {"\U0001F0AB", "\U0001F0BB", "\U0001F0CB", "\U0001F0DB"},
    // Queen of Spades	Queen of Hearts	Queen of Diamonds
    // Queen of Clubs
    {"\U0001F0AD", "\U0001F0BD", "\U0001F0CD", "\U0001F0DD"},
    // King of Spades	King of Hearts	King of Diamonds	King of Clubs
    {"\U0001F0AE", "\U0001F0BE", "\U0001F0CE", "\U0001F0DE"},
    // Ace of Spades	Ace of Hearts	Ace of Diamonds	Ace of Clubs
    {"\U0001F0A1", "\U0001F0B1", "\U0001F0C1", "\U0001F0D1"},
};

enum class Rank {
  Rank2 = 0,
  Rank3 = 1,
  Rank4 = 2,
  Rank5 = 3,
  Rank6 = 4,
  Rank7 = 5,
  Rank8 = 6,
  Rank9 = 7,
  Rank10 = 8,
  RankJack = 9,
  RankQueen = 10,
  RankKing = 11,
  RankAce = 12,
};

int getRankValue(const Rank rank) {
  switch (rank) {
    case Rank::Rank2:
      return 2;
    case Rank::Rank3:
      return 3;
    case Rank::Rank4:
      return 4;
    case Rank::Rank5:
      return 5;
    case Rank::Rank6:
      return 6;
    case Rank::Rank7:
      return 7;
    case Rank::Rank8:
      return 8;
    case Rank::Rank9:
      return 9;
    case Rank::Rank10:
    case Rank::RankJack:
    case Rank::RankQueen:
    case Rank::RankKing:
      return 10;
    case Rank::RankAce:
      return 11;
  }
  return 0;
}

enum class Suit {
  Spades = 0,
  Hearts = 1,
  Diamonds = 2,
  Clubs = 3,
};

class Card {
 public:
  Card(const Rank rank, const Suit suit) : rank_{rank}, suit_{suit} {}
  void print() const {
    const char *val = cards[static_cast<int>(rank_)][static_cast<int>(suit_)];
    std::cout << val;
  }
  int value() const { return getRankValue(rank_); }
  Rank rank() const { return rank_; }
  Suit suit() const { return suit_; }

 private:
  Rank rank_;
  Suit suit_;
};

class Deck {
 public:
  Deck() {
    for (int suit = 0; suit < numRanks; suit++) {
      for (int rank = 0; rank < numCards; rank++) {
        Card card(static_cast<Rank>(rank), static_cast<Suit>(suit));
        cards_.push_back(card);
      }
    }
  }

  void print() const {
    int count{0};
    for (const auto &card : cards_) {
      card.print();
      std::cout << ' ';
      count++;
      if (count % 13 == 0) {
        std::cout << std::endl;
      }
    }
    if (count % 13 != 0) {
      std::cout << std::endl;
    }
  }

  void shuffle() {
    for (unsigned int i = 0u; i < cards_.size() - 1; i++) {
      int j = Hoist::RNG::rand<int>(i, cards_.size());
      std::iter_swap(cards_.begin() + i, cards_.begin() + j);
    }
  }

  Card draw() {
    Card last = cards_.back();
    cards_.pop_back();
    return last;
  }

 private:
  std::vector<Card> cards_;
};

class Player {
 public:
  Player(const std::string &name) : cards_(), name_(std::move(name)) {}

  void take(Card card) { cards_.push_back(card); }

  int value() {
    int value = 0;
    for (const auto &card : cards_) {
      value += card.value();
    }
    return value;
  }

  void print() {
    std::cout << name_ << '(' << value() << ") ";
    for (unsigned int index = 0u, size = cards_.size(); index < cards_.size();
         index++) {
      cards_[index].print();
      if (index + 1 < size) {
        std::cout << ' ';
      }
    }
    std::cout << std::endl;
  }

 private:
  std::vector<Card> cards_;
  std::string name_;
};

void dealersTurn(Player &dealer, Deck &deck) {
  while (dealer.value() < dealerStandsOn) {
    dealer.take(deck.draw());
    dealer.print();
    usleep(dealerWaitMicros);
  }
}

void playersTurn(Player &player, Deck &deck) {
  while (player.value() < maxScore) {
    char c;
    do {
      c = Hoist::mustPrompt<char>("(h)it or (s)tay?", "invalid");
    } while (c != 'h' && c != 's');
    switch (c) {
      case 'h':
        player.take(deck.draw());
        player.print();
        break;
      case 's':
        return;
    }
  }
}

void checkGame(Player &dealer, Player &player) {
  const int dScore = dealer.value();
  const int pScore = player.value();

  if (pScore > maxScore) {
    std::cout << "BUST!!" << std::endl;
  } else if (dScore > maxScore) {
    std::cout << "Dealer busts, you win!" << std::endl;
  } else if (dScore > pScore) {
    std::cout << "Dealer wins." << std::endl;
  } else if (dScore < pScore) {
    std::cout << "Player wins!" << std::endl;
  } else {
    std::cout << "Push." << std::endl;
  }
}

void playBlackjack() {
  Deck deck;
  deck.print();
  deck.shuffle();

  Player dealer("Dealer");
  Player player("Player");

  player.take(deck.draw());
  dealer.take(deck.draw());
  player.take(deck.draw());

  dealer.print();
  player.print();

  playersTurn(player, deck);
  if (player.value() > maxScore) {
    std::cout << "BUST!!" << std::endl;
  } else {
    dealersTurn(dealer, deck);
    checkGame(dealer, player);
  }

  deck.print();
}

int main() {
  Hoist::Init();
  playBlackjack();

  while (true) {
    char c;
    do {
      c = Hoist::mustPrompt<char>("(b)et or (e)xit?", "invalid");
    } while (c != 'b' && c != 'e');
    switch (c) {
      case 'b':
        playBlackjack();
        break;
      case 'e':
        return 0;
    }
  }
}