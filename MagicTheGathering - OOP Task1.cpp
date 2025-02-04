#include <iostream>
#include <cstring>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>

const int MAX_PLAYER_NAME = 127;
const int MAX_CARD_NAME = 63;
const int DECK_SIZE = 3;         // “Œ¬¿ œŒ ”—ÀŒ¬»≈ “–ﬂ¡¬¿ ƒ¿ ≈ 60 - —À≈ƒ  ¿“Œ —≈ Õ¿œ–¿¬» ƒ¿ –¿¡Œ“» “–ﬂ¡¬¿ ƒ¿ —≈ —Ã≈Õ»


enum Color {
    Red,
    Black,
    Blue,
    White,
    Green
};


class Player
{
public:
    Player() = default;

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



class Card 
{
public:
    Card() = default;

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

    std::string GetPlayerName() const {
        return playerName;
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
bool Report(const std::vector<Deck>& Decks, const std::vector<Card>& Cards);
std::string ToColor(Color color);


int main()
{
    bool running = true;
    std::string command = "";
    std::vector<Deck> Decks;
    std::vector<Player> Players;
    std::vector<Card> Cards;

    //This will read the binary file for players
    std::ifstream readingPlayers("players.dat", std::ios::binary);
    if (!readingPlayers.is_open()) {
        std::cout << "Error! File was not opened!\n";
    }
    else {
        Player tempPlayer;

        while (readingPlayers.read(reinterpret_cast<char*> (&tempPlayer), sizeof(Player))) {
            std::cout << "Name: " << tempPlayer.GetName() <<  "  ID Number: " << tempPlayer.GetPlayerID() << "\n";
        }

        readingPlayers.close();
    }

    //This will read the binary file for cards
    std::ifstream readingCards("cards.dat", std::ios::binary);
    if (!readingCards.is_open()) {
        std::cout << "Error! File was not opened!\n";
    }
    else {
        Card tempCard;
        while (readingCards.read(reinterpret_cast<char*> (&tempCard), sizeof(Card))) {
            Color tempColor = tempCard.GetColor();
            std::cout << "Card Name: " << tempCard.GetName() << "  Card ID: " << tempCard.GetCardID() << "  Card Color: " << ToColor(tempColor) << "\n";
        }

        readingCards.close();
    }

    //This will read the binary file for decks
    std::ifstream readingDecks("decks.dat", std::ios::binary);

    if (!readingDecks.is_open()) {
        std::cout << "Error! File was not opened!\n";
    }
    else {
        Deck deck;
        while (readingDecks.read(reinterpret_cast<char*> (&deck), sizeof(Deck))) {
            Color tempColor = deck.GetColor();
            std::cout << "Deck Owner: " << deck.GetPlayerName() << "  Deck Color: " << ToColor(tempColor) << "\n";
        }
        readingDecks.close();
    }


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
            DeckColor(Decks,Cards);
        }
        else if(command == "report"){
            if (Report(Decks, Cards)) {
                std::cout << "\nReport Created!\n";
            }
        }
        else if (command == "exit") {
            running = false;
        }
        else {
            return 1;
        }
    }




}

//Menu of the program
void Menu() {
    std::cout << "\nChoose one of the following commands\n";
    std::cout << "----------------------------------\n";
    std::cout << "'cp' - To create a Player\n";
    std::cout << "'cc' - To create a Card\n";
    std::cout << "'cd' - To create a Deck\n";
    std::cout << "'report' - To generate a file for all players\n";
    std::cout << "'exit' - To exit\n\n";
}

//This function will create a player with an unique ID number
void CreatePlayer(std::vector<Player>& Players) {
    char name[MAX_PLAYER_NAME];

    std::cout << "Enter Player name: ";
    std::cin.getline(name, sizeof(name));

    //This verifies if a player already exists
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

//This function will create the cards and verify if they are unique
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

    //This verifies if the card name already exists
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


//This function will create the decks
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
    std::cout << "\nEnter player name: ";
    getline(std::cin, name);

    bool playerFound = false;
    for (const auto& player : Players) {
        if (name == player.GetName()) {
            std::cout << "\nPlayer was found!\n";
            playerFound = true;
            break;
        }
    }

        if (!playerFound) {
            std::cout << "\nPlayer was not found!\n";
            return;
        }


    std::cout << "\n       Name of Card\n";
    std::cout << "------------------------------\n";
    for (const auto& card : Cards) {
        std::cout<< card.GetName() <<"\n";
    }

   Deck deck(name);
   Decks.push_back(deck);

   std::cin.ignore();

}

//This function will set the color of the created decks
void DeckColor(std::vector<Deck>& Decks, const std::vector<Card>& Cards) {

    int red = 0;
    int black = 0;
    int blue = 0;
    int white = 0;
    int green = 0;
    Color setColor;

    //This checks the color of the cards in the deck
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

        //This checks what color the deck should be
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

        //This sets the color of the deck
        deck.SetColor(setColor);

        //This resets the colors so it can evaluated correctly for the next deck
        red = 0;
        black = 0;
        blue = 0;
        white = 0;
        green = 0;

    }
    
}


//This will generate the text file "Report" where decks will be separated by colors and info like Deck Owner, First Card Deck and First Card Color will be displayed
bool Report(const std::vector<Deck>& Decks, const std::vector<Card>& Cards) {

    std::ofstream file("Report.txt", std::ios::out);

    if (!file.is_open()) {
        std::cout << "\n Error! File not created!\n";
        return false;
    }
    else {
                file << "Red\n";
                for (auto& deck : Decks) {
                    if (deck.GetColor() == Color::Red) {
                        std::string firstCardName = deck.GetDeckCardNames()[0];
                        Color firstCardColor = Color::Red;

                        for (auto& card : Cards) {
                            if (card.GetName() == firstCardName) {
                                firstCardColor = card.GetColor();
                            }

                        }

                        file << "    Owner: " << deck.GetPlayerName() << "  Card: " << deck.GetDeckCardNames()[0] << "  Color: " << ToColor(firstCardColor) << "\n";
                    }
                    } 



                file << "Black\n";
                for (auto& deck : Decks) {
                    if (deck.GetColor() == Color::Black) {
                        std::string firstCardName = deck.GetDeckCardNames()[0];
                        Color firstCardColor = Color::Black;

                        for (auto& card : Cards) {
                            if (card.GetName() == firstCardName) {
                                firstCardColor = card.GetColor();
                            }

                        }

                        file << "    Owner: " << deck.GetPlayerName() << "  Card: " << deck.GetDeckCardNames()[0] << "  Color: " << ToColor(firstCardColor) << "\n";
                    }
                }

                file << "Blue\n";
                for (auto& deck : Decks) {
                    if (deck.GetColor() == Color::Blue) {
                        std::string firstCardName = deck.GetDeckCardNames()[0];
                        Color firstCardColor = Color::Blue;

                        for (auto& card : Cards) {
                            if (card.GetName() == firstCardName) {
                                firstCardColor = card.GetColor();
                            }

                        }

                        file << "    Owner: " << deck.GetPlayerName() << "  Card: " << deck.GetDeckCardNames()[0] << "  Color: " << ToColor(firstCardColor) << "\n";
                    }
                }

                file << "White\n";
                for (auto& deck : Decks) {
                    if (deck.GetColor() == Color::White) {
                        std::string firstCardName = deck.GetDeckCardNames()[0];
                        Color firstCardColor = Color::White;

                        for (auto& card : Cards) {
                            if (card.GetName() == firstCardName) {
                                firstCardColor = card.GetColor();
                            }

                        }

                        file << "    Owner: " << deck.GetPlayerName() << "  Card: " << deck.GetDeckCardNames()[0] << "  Color: " << ToColor(firstCardColor) << "\n";
                    }
                }

                file << "Green\n";
                for (auto& deck : Decks) {
                    if (deck.GetColor() == Color::Green) {
                        std::string firstCardName = deck.GetDeckCardNames()[0];
                        Color firstCardColor = Color::Green;

                        for (auto& card : Cards) {
                            if (card.GetName() == firstCardName) {
                                firstCardColor = card.GetColor();
                            }

                        }

                        file << "    Owner: " << deck.GetPlayerName() << "  Card: " << deck.GetDeckCardNames()[0] << "  Color: " << ToColor(firstCardColor) << "\n";
                    }
                }

        file.close();
    }
    return true;
}

//This will convert the color into string so it can be saved in the text file
std::string ToColor(Color color) {
    switch (color) {
    case Red: return "Red";
    case Black: return "Black";
    case Blue: return "Blue";
    case White: return "White";
    case Green: return "Green";
    default: return "Unknown";
    }
}

