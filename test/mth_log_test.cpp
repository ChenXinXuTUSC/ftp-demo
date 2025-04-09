#include "common.h"
#include "utils/logger.h"

#include <vector>


init __init__;
int main(int argc, char** argv)
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(
            [i]() {
                std::this_thread::sleep_for(std::chrono::seconds(std::rand() % 3 + 1));

                std::stringstream ss;
                ss << std::this_thread::get_id();
                DBUG(std::to_string(i), ss.str());
            }
        );
    };
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}
