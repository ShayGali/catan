#include "Player.hpp"

#include "../Catan.hpp"
#include "../cards/KngihtCard.hpp"
#include "../cards/MonopolyCard.hpp"
#include "../cards/PromotionCard.hpp"
#include "../cards/RoadBuildCard.hpp"
#include "../cards/YearOfPlentyCard.hpp"

Player::Player(PlayerColor color) : resourceCount(5, 0) {
    this->color = color;
    victoryPoints = 0;
    knights_counter = 0;
}

Player::Player(const Player &other) {
    // deep copy the other player's data
    color = other.color;
    victoryPoints = other.victoryPoints;
    knights_counter = other.knights_counter;
    for (int i = 0; i < 5; i++) {
        resourceCount[i] = other.resourceCount[i];
    }

    for (const auto *card : other.devCards) {
        devCards.push_back(card->clone());  // clone the card
    }
}

Player::~Player() {
    // free the memory allocated for the development cards
    for (auto *card : devCards) {
        delete card;
    }
}

Player &Player::operator=(const Player &other) {
    if (this == &other) {
        return *this;
    }

    // copy the other player's data and delete the current player's data
    color = other.color;
    victoryPoints = other.victoryPoints;
    knights_counter = other.knights_counter;
    for (int i = 0; i < 5; i++) {
        resourceCount[i] = other.resourceCount[i];
    }

    for (auto *const card : devCards) {
        delete card;
    }
    devCards.clear();

    for (auto *const card : other.devCards) {
        devCards.push_back(card->clone());
    }

    return *this;
}

void Player::play_turn(Catan &game) {
    char choice = 0;
    while (true) {  // loop until the player rolls the dice or uses a development card
        cout << "Choose an action:\n"
             << "\t1. Roll dice\n"
             << "\t2. Use development card\n"
             << "\tR. Display resources\n"
             << "\tD. Display development cards\n"
             << "\tV. Display victory points\n"
             << "\tK. Display knights\n";

        // get user input from console
        cin >> choice;

        if (choice == '1') {
            game.roll_dice();
            break;
        }
        if (choice == '2') {
            try {
                use_dev_card(game);
                // if he use a development card, he can't roll the dice
                return;
            } catch (std::exception &e) {
                cout << e.what() << "\n";
            }
        } else if (choice == 'R') {
            display_resources();
        } else if (choice == 'D') {
            display_dev_cards();
        } else if (choice == 'V') {
            cout << "Victory points: " << victoryPoints << "\n";
        } else if (choice == 'K') {
            cout << "Knights: " << knights_counter << "\n";
        } else {
            cout << "Invalid choice\n";
        }
    };

    while (true) {  // loop until the player ends his turn or uses a development card
        cout << "Choose an action:\n"
             << "\t1. Place settlement\n"
             << "\t2. Place road\n"
             << "\t3. Place city\n"
             << "\t4. Buy development card\n"
             << "\t5. Use development card\n"
             << "\t6. Trade\n"
             << "\tR. Display resources\n"
             << "\tD. Display development cards\n"
             << "\tV. Display victory points\n"
             << "\tK. Display knights\n"
             << "\tB. Display Board\n"
             << "\tE. End turn\n";

        cin >> choice;

        try {
            switch (choice) {
                case '1': {
                    place_settlement(game);
                    break;
                }
                case '2': {
                    place_road(game);
                    break;
                }
                case '3': {
                    place_city(game);
                    break;
                }
                case '4': {
                    buy_dev_card(game);
                    break;
                }
                case '5': {
                    use_dev_card(game);
                    return;
                }
                case '6': {
                    make_trade(game);
                    break;
                }
                case 'R':
                    display_resources();
                    break;
                case 'D':
                    display_dev_cards();
                    break;
                case 'V':
                    cout << "Victory points: " << victoryPoints << "\n";
                    break;
                case 'K':
                    cout << "Knights: " << knights_counter << "\n";
                    break;
                case 'B':
                    game.display_board();
                    break;
                case 'E': {
                    return;
                }
                default: {
                    cout << "Invalid choice\n";
                    break;
                }
            }
        } catch (std::exception &e) {
            cout << e.what() << "\n";
        }
    }
}

int Player::get_victory_points() const {
    return victoryPoints;
}

void Player::add_victory_points(int points) {
    victoryPoints += points;
}

std::string Player::get_color() const {
    switch (this->color) {
        case PlayerColor::RED:
            return "\033[1;31mRED\033[0m";
        case PlayerColor::BLUE:
            return "\033[1;34mBLUE\033[0m";
        case PlayerColor::YELLOW:
            return "\033[1;33mYELLOW\033[0m";
        default:
            throw std::runtime_error("Invalid color");
    }
}

std::string Player::get_color_code() const {
    switch (this->color) {
        case PlayerColor::RED:
            return "\033[1;31m";
        case PlayerColor::BLUE:
            return "\033[1;34m";
        case PlayerColor::YELLOW:
            return "\033[1;33m";
        default:
            throw std::runtime_error("Invalid color");
    }
}

int Player::get_resource_count(resource resource) const {
    if (resource == resource::NONE || resource == resource::DESERT) {
        return 0;
    }
    return resourceCount[resource.get_int()];
}

void Player::add_resource(resource resource, int count) {
    if (resource == resource::NONE || resource == resource::DESERT) {
        return;
    }
    resourceCount[resource.get_int()] += count;
}

void Player::use_resource(resource resource, int count) {
    if (resourceCount[resource.get_int()] < count) {
        throw std::invalid_argument("Not enough resources");
    }
    resourceCount[resource.get_int()] -= count;
}

void Player::display_resources() const {
    resource wood = resource::WOOD, clay = resource::CLAY, sheep = resource::SHEEP, wheat = resource::WHEAT, stone = resource::STONE;
    cout << "Resources: \n"
         << "\t1." << wood.get_emoji() << ":" << resourceCount[0] << "\n"
         << "\t2." << clay.get_emoji() << ":" << resourceCount[1] << "\n"
         << "\t3." << sheep.get_emoji() << ":" << resourceCount[2] << "\n"
         << "\t4." << wheat.get_emoji() << ":" << resourceCount[3] << "\n"
         << "\t5." << stone.get_emoji() << ":" << resourceCount[4] << "\n";
}

void Player::display_dev_cards() const {
    if (devCards.empty()) {
        cout << "No development cards\n";
        return;
    }

    cout << "Development cards: \n";
    for (int i = 0; i < devCards.size(); i++) {
        cout << i + 1 << ". " << devCards[i]->emoji() << "\n";
    }
}

void Player::buy_dev_card(Catan &game) {
    // check if the player has enough resources
    if (get_resource_count(resource::WHEAT) < 1 || get_resource_count(resource::SHEEP) < 1 || get_resource_count(resource::STONE) < 1) {
        throw std::runtime_error("Not enough resources to buy a development card");
    }

    // Buy a random development card
    Card *card = game.buy_dev_card(*this);
    devCards.push_back(card);

    // display the card bought
    cout << "You got: " << card->emoji() << "\n";
    cout << "Card description: " << card->get_description() << "\n";
}

int Player::place_settlement(Catan &game, bool first_round) {
    // check if the player has enough resources
    if (!first_round && (resourceCount[0] < 1 || resourceCount[1] < 1 || resourceCount[2] < 1 || resourceCount[3] < 1)) {
        cout << "Not enough resources to place a settlement\n";
        return -1;
    }

    // loop until the player places a settlement successfully
    int x = 0;
    while (true) {
        cout << "Enter the coordinates of the settlement (-1 to cancel)\n";
        cin >> x;

        if (x == -1) {
            if (first_round) {
                cout << "You must place a settlement\n";
                continue;
            }
            return -1;
        }

        try {
            game.place_settlement(x, *this, first_round);
            break;
        } catch (std::invalid_argument &e) {
            cout << e.what() << "\n";
            continue;
        }
    }

    // return the index of the settlement placed
    return x;
}

void Player::place_road(Catan &game, bool first_round) {
    // check if the player has enough resources
    if (!first_round && (resourceCount[0] < 1 || resourceCount[1] < 1)) {
        cout << "Not enough resources to place a road\n";
        return;
    }

    // loop until the player places a road successfully
    int x = 0;
    while (true) {
        cout << "Enter the coordinates of the road (-1 to cancel)\n";
        cin >> x;
        if (x == -1) {
            if (first_round) {
                cout << "You must place a road\n";
                continue;
            }
            return;
        }
        try {
            game.place_road(x, *this, first_round);
            break;
        } catch (std::invalid_argument &e) {
            cout << e.what() << "\n";
            continue;
        }
    }
}

void Player::place_city(Catan &game) {
    // check if the player has enough resources
    if (resourceCount[3] < 2 || resourceCount[4] < 3) {
        cout << "Not enough resources to place a city\n";
        return;
    }

    // loop until the player places a city successfully
    int x = 0;
    while (true) {
        cout << "Enter the coordinates of the city (-1 to cancel)\n";
        cin >> x;
        if (x == -1) {
            return;
        }
        try {
            game.place_city(x, *this);
            break;
        } catch (std::invalid_argument &e) {
            cout << e.what() << "\n";
            continue;
        }
    }
}

void Player::make_trade(Catan &game) {
    vector<pair<resource, int>> offer_res;
    vector<Card *> offer_dev;

    display_resources();

    int choice = 0;
    while (true) {  // loop until the player ends the resources selection
        cout << "Choose a offer resource (enter -1 to end the resources selection)\n";
        cin >> choice;
        if (choice == -1) {
            break;
        }
        if (choice <= 0 || choice > 5) {
            cout << "Invalid choice\n";
            continue;
        }
        cout << "Enter the amount of the resource\n";
        int amount = 0;
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid amount\n";
            continue;
        }

        if (amount > resourceCount[choice - 1]) {
            cout << "You don't have enough resources\n";
            continue;
        }

        offer_res.push_back({resource::from_int(choice - 1), amount});
    }

    if (!devCards.empty()) {  // if the player has development cards
        vector<bool> selected(devCards.size(), false);

        cout << "Choose a offer development card (enter -1 to end the development cards selection)\n";
        for (int i = 0; i < devCards.size(); i++) {
            cout << (i + 1) << ". " << devCards[i]->emoji() << "\n";
        }

        while (true) {  // loop until the player ends the development cards selection
            cout << "Choose a offer development card (enter -1 to end the development cards selection)\n";
            cout << "Enter development card index\n";
            int index = 0;
            cin >> index;

            if (index == -1) {
                break;
            }

            if (index > devCards.size() || index <= 0) {
                cout << "Invalid index\n";
                continue;
            }
            if (selected[index - 1]) {
                cout << "You already selected this card\n";
                continue;
            }
            selected[index - 1] = true;
            offer_dev.push_back(devCards[index - 1]);
        }
    }

    vector<pair<resource, int>> request_res;
    vector<pair<CardType, int>> request_dev;

    while (true) {  // loop until the player ends the resources selection
        cout << "Choose a request resource (enter -1 to end the resources selection)\n";
        cout << "1. Wood, 2. Clay, 3. Sheep, 4. Wheat, 5. Stone\n";
        cin >> choice;
        if (choice == -1) {
            break;
        }
        if (choice < 0 || choice > 5) {
            cout << "Invalid choice\n";
            continue;
        }
        cout << "Enter the amount of the resource\n";
        int amount = 0;
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid amount\n";
            continue;
        }

        request_res.push_back({resource::from_int(choice - 1), amount});
    }

    while (true) {  // loop until the player ends the development cards selection
        cout << "Choose a request development card (enter -1 to end the development cards selection)\n";
        cout << "1. Knight, 2. Victory Point, 3. Road Building, 4. Monopoly, 5. Year of Plenty\n";
        cin >> choice;
        if (choice == -1) {
            break;
        }
        if (choice <= 0 || choice > 5) {
            cout << "Invalid choice\n";
            continue;
        }
        cout << "Enter the amount of the development card\n";
        int amount = 0;
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid amount\n";
            continue;
        }

        request_dev.push_back({Card::from_int(choice - 1), amount});
    }

    try {
        // call the game object to make the trade
        game.make_trade_offer(*this, offer_res, offer_dev, request_res, request_dev);
    } catch (std::invalid_argument &e) {
        cout << e.what() << "\n";
    }
}

bool Player::trade_request(Player &trader, const vector<pair<resource, int>> &offer_res, const vector<Card *> &offer_dev, const vector<pair<resource, int>> &request_res, const vector<pair<CardType, int>> &request_dev) {
    // if the trade request is invalid we return false
    if (offer_res.empty() && offer_dev.empty() && request_res.empty() && request_dev.empty()) {
        cout << "Invalid trade request\n";
        return false;
    }

    // display the trade offer
    cout << "Player " << get_color() << " received a trade request from player " << trader.get_color() << "\n";
    if (!offer_res.empty()) {
        cout << "Offered resources: \n";
        for (const auto &res : offer_res) {
            cout << "\t" << res.second << " " << res.first.get_emoji() << "\n";
        }
    }
    if (!offer_dev.empty()) {
        cout << "Offered development cards: \n";
        for (const auto &dev : offer_dev) {
            cout << "\t" << dev->emoji() << "\n";
        }
    }

    // display the trade request
    if (!request_res.empty()) {
        cout << "Requested resources: \n";
        for (const auto &res : request_res) {
            cout << "\t" << res.second << " " << res.first.get_emoji() << "\n";
        }
    }

    if (!request_dev.empty()) {
        cout << "Requested development cards: \n";
        for (const auto &dev : request_dev) {
            cout << "\t" << dev.second << " " << Card::emoji_from_type(dev.first) << "\n";
        }
    }

    cout << "Your resources: \n";
    display_resources();

    cout << "Your development cards: \n";
    display_dev_cards();

    cout << "Do you accept the trade? (y/n)\n";
    char choice = 0;
    cin >> choice;
    return choice == 'y' || choice == 'Y';
}

void Player::use_dev_card(Catan &game) {
    if (devCards.empty()) {
        throw std::invalid_argument("No development cards");
    }
    display_dev_cards();

    while (true) {  // loop until the player chooses a development card to use
        cout << "Enter 1 to use a development card, 2 to show info about a development card, -1 to cancel\n";
        int choice = 0;
        cin >> choice;
        if (choice == -1) {
            return;
        }
        if (choice == 1) {
            break;
        }
        if (choice == 2) {
            cout << "Enter the index of the development card\n";
            int index = 0;
            cin >> index;
            if (index <= 0 || index > devCards.size()) {
                cout << "Invalid index\n";
                continue;
            }
            cout << devCards[index - 1]->get_description() << "\n";
        }
    }

    // ask the player to choose a development card to use
    cout << "Choose a development card to use\n";
    int index = 0;
    cin >> index;
    if (index <= 0 || index > devCards.size()) {
        cout << "Invalid index\n";
        return;
    }

    Card *card = devCards[index - 1];

    // call the function that actually uses the development card
    use_dev_card(game, card);
}

void Player::use_dev_card(Catan &game, Card *card) {
    game.use_dev_card(*this, card);

    // if the card is Promotion, we need to delete it from the player's devCards
    if (dynamic_cast<PromotionCard *>(card) != nullptr) {
        devCards.erase(std::remove(devCards.begin(), devCards.end(), card), devCards.end());
        delete card;
    }
}

void Player::add_knight() {
    knights_counter++;
}

void Player::remove_knight() {
    knights_counter--;
}

int Player::get_knights() const {
    return knights_counter;
}

Card *Player::get_dev_card(CardType type) {
    for (int i = 0; i < devCards.size(); i++) {
        if (devCards[i]->type() == type) {
            return devCards[i];
        }
    }
    return nullptr;
}

Card *Player::remove_dev_card(Card *card) {
    for (int i = 0; i < devCards.size(); i++) {
        if (devCards[i] == card) {
            Card *removed = devCards[i];
            devCards.erase(devCards.begin() + i);
            return removed;
        }
    }
    return nullptr;
}

void Player::add_dev_card(Card *card) {
    devCards.push_back(card);
}

int Player::get_dev_card_count(const CardType &type) {
    int count = 0;
    for (int i = 0; i < devCards.size(); i++) {
        if (devCards[i]->type() == type) {
            count++;
        }
    }
    return count;
}

vector<Card *> Player::get_dev_cards() {
    return devCards;
}

bool Player::operator==(const Player &other) const {
    return this->color == other.color;
}

bool Player::operator!=(const Player &other) const {
    return !(*this == other);
}

int Player::get_total_resources() const {
    int total = 0;
    for (int i = 0; i < 5; i++) {
        total += resourceCount[i];
    }
    return total;
}

void Player::return_resources_on_seven_roll() {
    cout << "Player " << get_color() << " has more than 7 resources\n";
    int total = get_total_resources();
    int total_to_return = total / 2;
    cout << "You must return " << total_to_return << " resources\n";

    display_resources();
    while (true) {  // get the resources to return from the player until he returns exactly half of his resources
        int sheep_count = 0;
        int wheat_count = 0;
        int stone_count = 0;
        int clay_count = 0;
        int wood_count = 0;
        if (get_resource_count(resource::WOOD) > 0) {
            while (true) {
                cout << "How many wood do you want to return?\n";
                cin >> wood_count;
                if (wood_count > get_resource_count(resource::WOOD)) {
                    cout << "You don't have enough wood\n";
                    continue;
                }

                break;
            }
        }

        if (get_resource_count(resource::CLAY) > 0) {
            while (true) {
                cout << "How many clay do you want to return?\n";
                cin >> clay_count;
                if (clay_count > get_resource_count(resource::CLAY)) {
                    cout << "You don't have enough clay\n";
                    continue;
                }

                break;
            }
        }

        if (get_resource_count(resource::SHEEP) > 0) {
            while (true) {
                cout << "How many sheep do you want to return?\n";
                cin >> sheep_count;
                cout << sheep_count << "\n";
                if (sheep_count > get_resource_count(resource::SHEEP)) {
                    cout << "You don't have enough sheep\n";
                    continue;
                }
                break;
            }
        }

        if (get_resource_count(resource::WHEAT) > 0) {
            while (true) {
                cout << "How many wheat do you want to return?\n";
                cin >> wheat_count;
                if (wheat_count > get_resource_count(resource::WHEAT)) {
                    cout << "You don't have enough wheat\n";
                    continue;
                }

                break;
            }
        }

        if (get_resource_count(resource::STONE) > 0) {
            while (true) {
                cout << "How many stone do you want to return?\n";
                cin >> stone_count;
                if (stone_count > get_resource_count(resource::STONE)) {
                    cout << "You don't have enough stone\n";
                    continue;
                }

                break;
            }
        }

        int total_returned = wood_count + clay_count + sheep_count + wheat_count + stone_count;
        if (total_to_return != total_returned) {
            cout << "You must return exactly " << total_to_return << " resources\n";
            cout << "You returned " << total_returned << " resources\n";
            cout << "Try again\n";
            continue;
        }

        // return the resources
        use_resource(resource::WOOD, wood_count);
        use_resource(resource::CLAY, clay_count);
        use_resource(resource::SHEEP, sheep_count);
        use_resource(resource::WHEAT, wheat_count);
        use_resource(resource::STONE, stone_count);

        break;
    }
}