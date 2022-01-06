#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include "cpu_usage.h"

int main(int argc, char* argv[]) {
    int sleep_ms = 1000;

    // parse sampling duration from command line argument
    if (argc == 2) {
        std::istringstream ss(argv[1]);
        ss >> sleep_ms;
    }

    cpu_usage usage;
    cpu_usage::cpu_info_map_t cpu_usage = usage.get_cpu_usage();

    std::cout << "average CPU usage since start:" << std::endl;
    for (auto const& kv : cpu_usage) {
        std::cout << kv.first << " "
                  << kv.second.usage_ << "% usage "
                  << kv.second.user_ << "% user "
                  << kv.second.system_ << "% system "
                  << kv.second.iowait_ << "% iowait "
                  << kv.second.irq_ << "% irq "
                  << kv.second.softirq_ << "% softirq"
                  << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
    cpu_usage = usage.get_cpu_usage();

    std::cout << std::endl
              << "current CPU usage:" << std::endl;
    for (auto const& kv : cpu_usage) {
        std::cout << kv.first << " "
                  << kv.second.usage_ << "% usage "
                  << kv.second.user_ << "% user "
                  << kv.second.system_ << "% system "
                  << kv.second.iowait_ << "% iowait "
                  << kv.second.irq_ << "% irq "
                  << kv.second.softirq_ << "% softirq"
                  << std::endl;
    }

    return 0;
}
