#include "market_snapshot.h"
#include <iostream>

void MarketSnapshot::update_bid(double price, int qty) {
    if (qty == 0) {
        if (bids.erase(price))
            std::cout << "[Market] Bid " << price << " removed\n";
    } else {
        if (bids.count(price))
            bids[price]->quantity = qty;
        else
            bids[price] = std::make_unique<PriceLevel>(price, qty);
        std::cout << "[Market] Best Bid: " << get_best_bid()->price
                  << " x " << get_best_bid()->quantity << "\n";
    }
}

void MarketSnapshot::update_ask(double price, int qty) {
    if (qty == 0) {
        if (asks.erase(price))
            std::cout << "[Market] Ask " << price << " removed\n";
    } else {
        if (asks.count(price))
            asks[price]->quantity = qty;
        else
            asks[price] = std::make_unique<PriceLevel>(price, qty);
        std::cout << "[Market] Best Ask: " << get_best_ask()->price
                  << " x " << get_best_ask()->quantity << "\n";
    }
}

const PriceLevel* MarketSnapshot::get_best_bid() const {
    if (bids.empty()) return nullptr;
    return bids.begin()->second.get();
}

const PriceLevel* MarketSnapshot::get_best_ask() const {
    if (asks.empty()) return nullptr;
    return asks.begin()->second.get();
}
