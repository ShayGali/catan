#include "Player.hpp"

#include "../Catan.hpp"

using std::cout, std::cin;

Player::Player(PlayerColor color) {
    this->color = color;
    victoryPoints = 0;
    knights_counter = 0;
    for (int i = 0; i < 5; i++) {
        resourceCount[i] = 0;
    }
}

void Player::play_turn(Catan &game) {
    char choice;
    while (true) {
        cout << "Choose an action:\n";
        cout << "\t1. Roll dice\n";
        cout << "\t2. Use development card\n";

        // get user input from console
        cin >> choice;
        if (choice == '1') {
            game.roll_dice();
            break;
        } else if (choice == '2') {
            use_dev_card(game);

            // if he use a development card, he can't roll the dice
            return;
        } else {
            cout << "Invalid choice\n";
            continue;
        }
    };

    while (true) {
        cout << "Choose an action:\n";
        cout << "\t1. Place settlement\n";
        cout << "\t2. Place road\n";
        cout << "\t3. Place city\n";
        cout << "\t4. Buy development card\n";
        cout << "\t5. Trade\n";
        cout << "\t6. Display resources\n";
        cout << "\t7. Display development cards\n";
        cout << "\t8. End turn\n";

        cin >> choice;

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
                make_trade(game);
                break;
            }
            case '6':
                display_resources();
                break;
            case '7':
                display_dev_cards();
                break;
            case '8': {
                return;
            }
            default: {
                cout << "Invalid choice\n";
                break;
            }
        }
    }
}

int Player::get_victory_points() {
    return victoryPoints;
}

void Player::add_victory_points(int points) {
    victoryPoints += points;
}

std::string Player::get_color() {
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

std::string Player::get_color_code() {
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

std::string Player::get_player_info() {
    std::string info = "Player color: " + get_color() + "\n";
    info += "Resources: \n";
    info += "Wood: " + std::to_string(resourceCount[0]) + "\n";
    info += "Clay: " + std::to_string(resourceCount[1]) + "\n";
    info += "Sheep: " + std::to_string(resourceCount[2]) + "\n";
    info += "Wheat: " + std::to_string(resourceCount[3]) + "\n";
    info += "Stone: " + std::to_string(resourceCount[4]) + "\n";
    return info;
}

int Player::get_resource_count(resource resource) {
    return resourceCount[static_cast<int>(resource)];
}

void Player::add_resource(resource resource, int count) {
    if (resource == resource::NONE || resource == resource::DESERT)
        return;
    resourceCount[static_cast<int>(resource)] += count;
}

void Player::use_resource(resource resource, int count) {
    if (resourceCount[static_cast<int>(resource)] < count) {
        throw std::invalid_argument("Not enough resources");
    }
    resourceCount[static_cast<int>(resource)] -= count;
}

void Player::display_resources() {
    resource wood = resource::WOOD, clay = resource::CLAY, sheep = resource::SHEEP, wheat = resource::WHEAT, stone = resource::STONE;

    cout << "Resources: \n"
         << "\t" << wood.get_emoji() << ":" << resourceCount[0] << "\n"
         << "\t" << clay.get_emoji() << ":" << resourceCount[1] << "\n"
         << "\t" << sheep.get_emoji() << ":" << resourceCount[2] << "\n"
         << "\t" << wheat.get_emoji() << ":" << resourceCount[3] << "\n"
         << "\t" << stone.get_emoji() << ":" << resourceCount[4] << "\n";
}

void Player::display_dev_cards() {
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
    if (resourceCount[0] < 1 || resourceCount[2] < 1 || resourceCount[3] < 1) {
        cout << "Not enough resources to buy a development card\n";
        return;
    }
    try {
        game.get_dev_card(*this);
    } catch (std::invalid_argument &e) {
        cout << e.what() << "\n";
    }
    use_resource(resource::WHEAT, 1);

    // Buy a random development card
    Card *card = game.get_dev_card(*this);
    devCards.push_back(card);
}

int Player::place_settlement(Catan &game, bool first_round) {
    // check if the player has enough resources
    if (!first_round && (resourceCount[0] < 1 || resourceCount[1] < 1 || resourceCount[2] < 1 || resourceCount[3] < 1)) {
        cout << "Not enough resources to place a settlement\n";
        return -1;
    }

    int x;
    while (true) {
        cout << "Enter the coordinates of the settlement (-1 to cancel)\n";
        cin >> x;

        if (x == -1) {
            if (first_round) {
                cout << "You must place a settlement\n";
                continue;
            } else
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
    return x;
}

void Player::place_road(Catan &game, bool first_round) {
    // check if the player has enough resources
    if (!first_round && (resourceCount[0] < 1 || resourceCount[1] < 1)) {
        cout << "Not enough resources to place a road\n";
        return;
    }

    while (true) {
        cout << "Enter the coordinates of the road (-1 to cancel)\n";
        int x;
        cin >> x;
        if (x == -1) {
            if (first_round) {
                cout << "You must place a road\n";
                continue;
            } else
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

    while (true) {
        cout << "Enter the coordinates of the city (-1 to cancel)\n";
        int x;
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
    throw std::logic_error("Not implemented");

    vector<pair<resource, int>> offer_res;
    vector<pair<Card *, int>> offer_dev;

    cout << "Choose a offer resource (enter -1 to end the resources selection)\n";
    display_resources();

    int choice;
    while (true) {
        cin >> choice;
        if (choice == -1) {
            break;
        }
        if (choice < 0 || choice > 4) {
            cout << "Invalid choice\n";
            continue;
        }
        cout << "Enter the amount of the resource\n";
        int amount;
        cin >> amount;
        if (amount > resourceCount[choice]) {
            cout << "You don't have enough resources\n";
            continue;
        }
        offer_res.push_back({resource::from_int(choice), amount});
    }

    cout << "Choose a offer development card (enter -1 to end the development cards selection)\n";
    for (int i = 0; i < devCards.size(); i++) {
        cout << i << ". " << devCards[i]->emoji() << "\n";
    }

    vector<bool> selected(devCards.size(), false);

    while (true) {
        cin >> choice;
        if (choice == -1) {
            break;
        }
        if (choice < 0 || choice > devCards.size()) {
            cout << "Invalid choice\n";
            continue;
        }
        cout << "Enter development card index\n";
        int index;
        cin >> index;
        if (index > devCards.size() || index < 0) {
            cout << "Invalid index\n";
            continue;
        }
        if (selected[index]) {
            cout << "You already selected this card\n";
            continue;
        }
        selected[index] = true;
        offer_dev.push_back({devCards[index], 1});
    }

    vector<pair<resource, int>> request_res;
    vector<pair<CardType, int>> request_dev;

    cout << "Choose a request resource (enter -1 to end the resources selection)\n";
    cout << "1. Wood, 2. Clay, 3. Sheep, 4. Wheat, 5. Stone\n";
    while (true) {
        cin >> choice;
        if (choice == -1) {
            break;
        }
        if (choice < 0 || choice > 4) {
            cout << "Invalid choice\n";
            continue;
        }
        cout << "Enter the amount of the resource\n";
        int amount;
        cin >> amount;
        request_res.push_back({resource::from_int(choice), amount});
    }

    cout << "Choose a request development card (enter -1 to end the development cards selection)\n";
    cout << "1. Knight, 2. Road Building, 3. Year of Plenty, 4. Monopoly\n";
    while (true) {
        cin >> choice;
        if (choice == -1) {
            break;
        }
        if (choice < 0 || choice > 4) {
            cout << "Invalid choice\n";
            continue;
        }
        cout << "Enter the amount of the development card\n";
        int amount;
        cin >> amount;
        request_dev.push_back({Card::from_int(choice), amount});
    }
}

bool Player::trade_request(Player &trader, const vector<pair<resource, int>> &offer_res, const vector<pair<Card *, int>> &offer_dev, const vector<pair<resource, int>> &request_res, const vector<pair<Card *, int>> &request_dev) {
    cout << "Player " << get_color() << " received a trade request from player " << trader.get_color() << "\n";
    cout << "Offered resources: \n";
    for (auto &res : offer_res) {
        cout << "\t" << res.second << " " << res.first.get_emoji() << "\n";
    }
    cout << "Offered development cards: \n";
    for (auto &dev : offer_dev) {
        cout << "\t" << dev.second << " " << dev.first->emoji() << "\n";
    }

    cout << "Requested resources: \n";
    for (auto &res : request_res) {
        cout << "\t" << res.second << " " << res.first.get_emoji() << "\n";
    }

    cout << "Requested development cards: \n";
    for (auto &dev : request_dev) {
        cout << "\t" << dev.second << " " << dev.first->emoji() << "\n";
    }

    cout << "Your resources: \n";
    display_resources();

    cout << "Your development cards: \n";
    display_dev_cards();

    cout << "Do you accept the trade? (y/n)\n";
    char choice;
    cin >> choice;
    return choice == 'y' || choice == 'Y';
}

void Player::use_dev_card(Catan &game) {
    throw std::logic_error("Not implemented");
    display_dev_cards();

    if (devCards.empty()) {
        return;
    }

    while (true) {
        cout << "Enter 1 to use a development card, 2 to show info about a development card, -1 to cancel\n";
        int choice;
        cin >> choice;
        if (choice == -1) {
            return;
        }
        if (choice == 1) {
            break;
        }
        if (choice == 2) {
            cout << "Enter the index of the development card\n";
            int index;
            cin >> index;
            if (index < 0 || index >= devCards.size()) {
                cout << "Invalid index\n";
                continue;
            }
            cout << devCards[index]->get_description() << "\n";
        }
    }

    cout << "Choose a development card to use\n";
    int index;
    cin >> index;
    if (index <= 0 || index > devCards.size()) {
        cout << "Invalid index\n";
        return;
    }
    Card *card = devCards[index - 1];

    game.use_dev_card(*this, card);
}

void Player::add_knight() {
    knights_counter++;
}

int Player::get_knights() {
    return knights_counter;
}

void Player::remove_dev_card(Card *card) {
    devCards.erase(std::remove(devCards.begin(), devCards.end(), card), devCards.end());
}

void Player::add_dev_card(Card *card) {
    devCards.push_back(card);
}

int Player::get_dev_card_count(const CardType &type) {
    throw std::logic_error("no im");
    int count = 0;
    for (int i = 0; i < devCards.size(); i++) {
        // if (devCards[i]->get_type() == type) {
        // count++;
        // }
    }
    return count;
}

bool Player::operator==(const Player &other) const {
    return this->color == other.color;
}

bool Player::operator!=(const Player &other) const {
    return !(*this == other);
}

int Player::get_total_resources() {
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
    while (true) {
        int sheep_count;
        int wheat_count;
        int stone_count;
        int clay_count;
        int wood_count;
        while (true) {
            if (get_resource_count(resource::WOOD) > 0) {
                cout << "How many wood do you want to return?\n";
                cin >> wood_count;
                if (wood_count > get_resource_count(resource::WOOD)) {
                    cout << "You don't have enough wood\n";
                    continue;
                }

                break;
            }
        }

        while (true) {
            if (get_resource_count(resource::CLAY) > 0) {
                cout << "How many clay do you want to return?\n";
                cin >> clay_count;
                if (clay_count > get_resource_count(resource::CLAY)) {
                    cout << "You don't have enough clay\n";
                    continue;
                }

                break;
            }
        }

        while (true) {
            if (get_resource_count(resource::SHEEP) > 0) {
                cout << "How many sheep do you want to return?\n";
                cin >> sheep_count;
                if (sheep_count > get_resource_count(resource::SHEEP)) {
                    cout << "You don't have enough sheep\n";
                    continue;
                }

                break;
            }
        }

        while (true) {
            if (get_resource_count(resource::WHEAT) > 0) {
                cout << "How many wheat do you want to return?\n";
                cin >> wheat_count;
                if (wheat_count > get_resource_count(resource::WHEAT)) {
                    cout << "You don't have enough wheat\n";
                    continue;
                }

                break;
            }
        }

        while (true) {
            if (get_resource_count(resource::STONE) > 0) {
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

        use_resource(resource::WOOD, wood_count);
        use_resource(resource::CLAY, clay_count);
        use_resource(resource::SHEEP, sheep_count);
        use_resource(resource::WHEAT, wheat_count);
        use_resource(resource::STONE, stone_count);

        break;
    }
}