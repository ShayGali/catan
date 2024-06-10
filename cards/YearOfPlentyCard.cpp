// @author: shay.gali@msmail.ariel.ac.il
#include "YearOfPlentyCard.hpp"

#include "../player/Player.hpp"

std::string YearOfPlentyCard::get_description() const {
    return "Year of Plenty: Gain any two resources from the bank.";
}

std::string YearOfPlentyCard::emoji() const {
    return "🌟";
}

CardType YearOfPlentyCard::type() const {
    return CardType::YEAR_OF_PLENTY;
}

Card* YearOfPlentyCard::clone() const {
    return new YearOfPlentyCard(*this);
}

void YearOfPlentyCard::use(Catan& game, Player& player) {
    std::cout << "Choose two resources to gain from the bank.\n"
              << "\t1. Wood\n"
              << "\t2. Clay\n"
              << "\t3. Sheep\n"
              << "\t4. Wheat\n"
              << "\t5. Stone\n";

    int resource1 = 0 , resource2 = 0;
    std::cin >> resource1;
    std::cin >> resource2;

    if (resource1 < 1 || resource1 > 5 || resource2 < 1 || resource2 > 5) {
        throw std::invalid_argument("Invalid resource choice");
    }

    resource res1 = resource::from_int(resource1 - 1);
    resource res2 = resource::from_int(resource2 - 1);

    player.add_resource(res1, 1);
    player.add_resource(res2, 1);
}