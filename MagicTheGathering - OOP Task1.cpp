#include <iostream>
#include <cstring>
#include <iomanip>
#include <vector>
#include <string>

const int MAX_PLAYER_NAME = 127;
const int MAX_CARD_NAME = 63;
const int DECK_SIZE = 3;         // ТОВА ПО УСЛОВИЕ ТРЯБВА ДА Е 60 - СЛЕД КАТО СЕ НАПРАВИ ДА РАБОТИ ТРЯБВА ДА СЕ СМЕНИ

//Това ще се ползва за цвета на картите и на тестетата
enum Color {
    Red,
    Black,
    Blue,
    White,
    Green
};

// Ще се използва да се създават играчи
class Player
{
public:

    Player(const char* name) : playerID(++nextID) {
        strncpy_s(playerName, sizeof(playerName), name, _TRUNCATE);
        playerName[MAX_PLAYER_NAME] = '\0';
    }

    size_t GetPlayerID() {
        return playerID;
    }

    const char* GetName() const {
        return playerName;
    }

private:
    char playerName[MAX_PLAYER_NAME + 1];
    size_t playerID = 0;
    static size_t nextID;
};

size_t Player::nextID = 0;


//Ще се използва да се създават карти
class Card 
{
public:
    Card(const char* name, Color c) : cardID(++cID), color(c) {
        strncpy_s(cardName, sizeof(cardName), name, _TRUNCATE);
        cardName[MAX_CARD_NAME] = '\0';
    }

    const char* GetName() const {
        return cardName;
    }

    size_t GetCardID() const {
        return cardID;
    }

    Color GetColor() const {
        return color;
    }

private:
    char cardName[MAX_CARD_NAME + 1] = "";
    size_t cardID = 0;
    static size_t cID;
    Color color;
};

size_t Card::cID = 0;

//Ще се използва за създаване на тестета
class Deck
{
public:
    Deck() {
        for (size_t i = 0; i < DECK_SIZE; ++i) {
            cardNames[i] = "";
        }
    }

    Deck(std::string n) : playerName(n) {
        std::cout << "\n";
        std::cout << "Add " << DECK_SIZE << " cards to your deck!\n";
        std::cin.ignore();
        for (size_t i = 0; i < DECK_SIZE; ++i) {
            std::cout << "Enter Card name to add it to your deck (" << i + 1 << " out of " << DECK_SIZE << "): ";
            getline(std::cin, cardNames[i]);
        }
    };

    Color GetColor() const {
        return color;
    }

    const std::string* GetDeckCardNames() const {
        return cardNames;
    }

    void SetColor(Color c) {
        color = c;
    }

private:
    std::string playerName;
    std::string cardNames[DECK_SIZE];
    Color color;
};

void Menu();
void CreatePlayer(std::vector<Player>& Players);
void CreateCard(std::vector<Card>& Cards);
void CreateDeck(const std::vector<Card>& Cards, const std::vector<Player>& Players, std::vector<Deck>& Decks);
void DeckColor(std::vector<Deck>& Decks, const std::vector<Card>& Cards);


int main()
{
    bool running = true;
    std::string command = "";
    std::vector<Deck> Decks;
    std::vector<Player> Players;
    std::vector<Card> Cards;


    std::cout << "Welcome to the game 'Magic The Gathering' \n";
    while (running) {
        Menu();
        std::cin >> command;
        std::cin.ignore();
        if (command == "cp") {
            CreatePlayer(Players);
        }
        else if (command == "cc") {
            CreateCard(Cards);
        }
        else if (command == "cd") {
            CreateDeck(Cards,Players,Decks);
        }
        else if(command == "report"){

        }
        else if (command == "exit") {
            running = false;
        }
        else {
            return 1;
        }
    }

    DeckColor(Decks, Cards);
    for (auto& deck : Decks) {
        std::cout << deck.GetColor() << "\n";
    }


}

void Menu() {
    std::cout << "\nChoose one of the following commands\n";
    std::cout << "----------------------------------\n";
    std::cout << "'cp' - To create a Player\n";
    std::cout << "'cc' - To create a Card\n";
    std::cout << "'cd' - To create a Deck\n";
    std::cout << "'report' - To generate a file for all players\n";
    std::cout << "'exit' - To exit\n\n";
}

//Тази функция ще създаде играч с уникален идентификационен номер
void CreatePlayer(std::vector<Player>& Players) {
    char name[MAX_PLAYER_NAME];

    std::cout << "Enter Player name: ";
    std::cin.getline(name, sizeof(name));

    //Това проверява дали вече съществува играч с подаваното име
    for (const auto& player : Players) {
        if (strcmp(name, player.GetName()) == 0) {
            std::cout << "\nPlayer already exists!\n";
            return;
        }
    }
        Player player(name);
        Players.push_back(player);

        std::cout << "\nPlayer created successfully!\n";
   

}

//Тази функция ше създава картите и ще проверява дали са уникални
void CreateCard(std::vector<Card>& Cards) {
    char name[MAX_CARD_NAME];
    std::string color;
    Color selectedColor; 
    bool running = true;

    while (running) {
        std::cout << "\nChoose Card color\n";
        std::cout << "Options: Red, Black, Blue, White, and Green\n";
        std::cout << "Enter color: ";
        std::cin >> color;
        if (color == "Red") {
            selectedColor = Color::Red;
            running = false;
        }
        else if (color == "Black") {
            selectedColor = Color::Black;
            running = false;
        }
        else if (color == "Blue") {
            selectedColor = Color::Blue;
            running = false;
        }
        else if (color == "White") {
            selectedColor = Color::White;
            running = false;
        }
        else if (color == "Green") {
            selectedColor = Color::Green;
            running = false;
        }
        else {
            std::cout << "Invalid value! Try again!\n";
        }    
    }
    std::cin.ignore();

    std::cout << "Enter Card name: ";
    std::cin.getline(name, sizeof(name));

    //Това проверява дали името на картата вече съществува
    for (auto& card : Cards) {
        if (strcmp(name, card.GetName()) == 0) {
            std::cout << "\nCard already exists!\n";
            return;
        }
    }
        Card card(name, selectedColor);
        Cards.push_back(card);

        std::cout << "\nCard created successfully!\n";
}


//Тази фунцкия ще създава тестетата
void CreateDeck(const std::vector<Card>& Cards, const std::vector<Player>& Players, std::vector<Deck>& Decks) {
    if (Players.empty()) {
        std::cout << "\nCurrently there are no players available! Create a Player!\n";
        return;
    }

    if (Cards.empty()) {
        std::cout << "\nCurrently there are no cards available! Create Cards!\n";
        return;
    }
    
    std::string name;
    std::cout << "Enter player name: ";
    std::cin >> name;

 

    for (const auto& player : Players) {
        if (name == player.GetName()) {
            std::cout << "\nPlayer was found!\n";
            break;
        }
    }

    std::cout << "\n       Name of Card\n";
    std::cout << "------------------------------\n";
    for (const auto& card : Cards) {
        std::cout<< card.GetName() <<"\n";
    }

   Deck deck(name);
   Decks.push_back(deck);

}

//Тази функция ще намира цвета на тестетата
void DeckColor(std::vector<Deck>& Decks, const std::vector<Card>& Cards) {

    int red = 0;
    int black = 0;
    int blue = 0;
    int white = 0;
    int green = 0;
    Color setColor;

    //Това проверява цвета на картите
    for (auto& deck : Decks) {
        const std::string* cardName = deck.GetDeckCardNames();
        for (size_t i = 0; i < DECK_SIZE; ++i) {
            for (auto& card : Cards) {
                if (cardName[i] == card.GetName()) {
                    if (card.GetColor() == 0) {
                        red++;
                    }
                    else if (card.GetColor() == 1) {
                        black++;
                    }
                    else if (card.GetColor() == 2) {
                        blue++;
                    }
                    else if (card.GetColor() == 3) {
                        white++;
                    }
                    else {
                        green++;
                    }
                    
                }
            }
        }

        //Това проверява какъв цвят трябва да е тестето
        if (red > black && red > blue && red > white && red > green) {
            setColor = Color::Red;
        }
        else if (black > red && black > blue && black > white && black > green) {
            setColor = Color::Black;
        }
        else if (blue > red && blue > black && blue > white && blue > green) {
            setColor = Color::Blue;
        }
        else if (white > red && white > black && white > blue && white > green) {
            setColor = Color::White;
        }
        else {
            setColor = Color::Green;
        }

        //Това задава цвета на тестето
        deck.SetColor(setColor);

        //Това занулява стойностите за цветовете за да може да се изчисли правилно за следващото тесте
        red = 0;
        black = 0;
        blue = 0;
        white = 0;
        green = 0;

    }
    
}


