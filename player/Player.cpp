#include "Player.hpp"

#include "../Catan.hpp"

using std::cout, std::cin;

Player::Player(PlayerColor color) {
    this->color = color;
    for (int i = 0; i < 5; i++) {
        resourceCount[i] = 0;
    }
}

void Player::play_turn(Catan &game) {
    throw std::logic_error("Not implemented");
    char choice;
    do {
        cout << "Player " << get_color() << " turn\n";
        cout << "1. Roll dice\n";
        cout << "2. Use development card\n";

        // get user input from console
        cin >> choice;
        if (choice == '1') {
            game.roll_dice();
        } else if (choice == '2') {
            // use development card -
            display_dev_cards();
            int card_index;
            cin >> card_index;
            use_dev_card(card_index);

            // if he use a development card, he can't roll the dice
            return;
        } else {
            cout << "Invalid choice\n";
            continue;
        }
    } while (choice != '1' || choice != '2');

    while (true) {
        cout << "Choose an action:\n";
        cout << "1. Place settlement\n";
        cout << "2. Place road\n";
        cout << "3. Place city\n";
        cout << "4. Buy development card\n";
        cout << "5. trade\n";
        cout << "6. End turn\n";

        cin >> choice;

        switch (choice) {
            case '1': {
                this->place_settlement(game);
                break;
            }
            case '2': {
                this->place_road(game);
                break;
            }
            case '3': {
                this->place_city(game);
                break;
            }
            case '4': {
                this->buy_dev_card(game);
                break;
            }
            case '5': {
                this->make_trade(game);
                break;
            }
            case '6': {
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
            return "RED";
        case PlayerColor::BLUE:
            return "BLUE";
        case PlayerColor::YELLOW:
            return "YELLOW";
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
    resourceCount[static_cast<int>(resource)] += count;
}

void Player::use_resource(resource resource, int count) {
    if (resourceCount[static_cast<int>(resource)] < count) {
        throw std::invalid_argument("Not enough resources");
    }
    resourceCount[static_cast<int>(resource)] -= count;
}

void Player::display_resources() {
    cout << "Resources: \n"
         << "\t🌲:" << resourceCount[0] << "\n"
         << "\t🧱:" << resourceCount[1] << "\n"
         << "\t🐑:" << resourceCount[2] << "\n"
         << "\t🌾:" << resourceCount[3] << "\n"
         << "\t🪨 :" << resourceCount[4] << "\n";
}

void Player::display_dev_cards() {
    throw std::logic_error("Not implemented");
    cout << "Development cards: \n";
    for (int i = 0; i < devCards.size(); i++) {
        cout << i + 1 << ". " << devCards[i]->get_description() << "\n";
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

void Player::place_settlement(Catan &game) {
    // check if the player has enough resources
    if (resourceCount[0] < 1 || resourceCount[1] < 1 || resourceCount[2] < 1 || resourceCount[3] < 1) {
        cout << "Not enough resources to place a settlement\n";
        return;
    }

    while (true) {
        cout << "Enter the coordinates of the settlement (-1 to cancel)\n";
        int x;
        cin >> x;

        if (x == -1) {
            return;
        }

        try {
            game.place_settlement(x, *this);
            break;
        } catch (std::invalid_argument &e) {
            cout << e.what() << "\n";
            continue;
        }
    }
}

void Player::place_road(Catan &game) {
    // check if the player has enough resources
    if (resourceCount[0] < 1 || resourceCount[1] < 1) {
        cout << "Not enough resources to place a road\n";
        return;
    }

    while (true) {
        cout << "Enter the coordinates of the road (-1 to cancel)\n";
        int x;
        cin >> x;
        if (x == -1) {
            return;
        }
        try {
            game.place_road(x, *this);
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
}

void Player::use_dev_card(int card_index) {
    throw std::logic_error("Not implemented");
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
    int count = 0;
    for (int i = 0; i < devCards.size(); i++) {
        if (devCards[i]->get_type() == type) {
            count++;
        }
    }
    return count;
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

bool Player::operator==(const Player &player) const {
    return this->color == player.color;
}