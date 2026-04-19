#include "feed_parser.h"
#include "market_snapshot.h"
#include "order_manager.h"

int main() {
    MarketSnapshot snapshot;
    OrderManager om;

    auto feed = load_feed("sample_feed.txt");

    for (const auto& event : feed) {
        if (event.type == FeedType::BID) {
            snapshot.update_bid(event.price, event.quantity);

            const PriceLevel* bid = snapshot.get_best_bid();
            if (bid && bid->price >= 100.17) {
                const PriceLevel* ask = snapshot.get_best_ask();
                if (ask) {
                    om.place_order(Side::Sell, ask->price, 50);
                }
            }
        } else if (event.type == FeedType::ASK) {
            snapshot.update_ask(event.price, event.quantity);

            const PriceLevel* ask = snapshot.get_best_ask();
            if (ask && ask->price <= 100.20) {
                om.place_order(Side::Buy, ask->price, 50);
            }
        } else if (event.type == FeedType::EXECUTION) {
            om.handle_fill(event.order_id, event.quantity);
        }
    }

    std::cout << "\n--- Final Active Orders ---\n";
    om.print_active_orders();

    return 0;
}
