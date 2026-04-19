#include "order_manager.h"
#include <iostream>

int OrderManager::place_order(Side side, double price, int qty) {
    int id = next_id++;
    auto order = std::make_unique<MyOrder>();
    order->id = id;
    order->side = side;
    order->price = price;
    order->quantity = qty;
    std::cout << "[Strategy] Placing " << (side == Side::Buy ? "BUY" : "SELL")
              << " order at " << price << " x " << qty << " (ID = " << id << ")\n";
    orders[id] = std::move(order);
    return id;
}

void OrderManager::cancel(int id) {
    auto it = orders.find(id);
    if (it == orders.end()) {
        std::cerr << "[Order] Cancel failed: ID " << id << " not found\n";
        return;
    }
    it->second->status = OrderStatus::Cancelled;
    std::cout << "[Order] Order " << id << " cancelled\n";
    orders.erase(it);
}

void OrderManager::handle_fill(int id, int filled_qty) {
    auto it = orders.find(id);
    if (it == orders.end()) {
        std::cerr << "[Execution] Fill for unknown order ID " << id << "\n";
        return;
    }
    MyOrder& order = *it->second;
    order.filled += filled_qty;
    std::cout << "[Execution] Order " << id << " filled: " << filled_qty << "\n";

    if (order.filled >= order.quantity) {
        order.status = OrderStatus::Filled;
        std::cout << "[Order] Order " << id << " completed ("
                  << order.filled << " / " << order.quantity << ") and removed\n";
        orders.erase(it);
    } else {
        order.status = OrderStatus::PartiallyFilled;
        std::cout << "[Order] Order " << id << " partially filled: "
                  << order.filled << " / " << order.quantity << "\n";
    }
}

void OrderManager::print_active_orders() const {
    if (orders.empty()) {
        std::cout << "[Order] No active orders\n";
        return;
    }
    for (const auto& [id, order] : orders) {
        std::cout << "[Order] ID=" << id
                  << " " << (order->side == Side::Buy ? "BUY" : "SELL")
                  << " " << order->price << " x " << order->quantity
                  << " filled=" << order->filled << "\n";
    }
}
