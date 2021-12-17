#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>

#include "cpu_usage.h"

cpu_usage::cpu_stat::cpu_stat() :
    user_(0), nice_(0), system_(0), idle_(0),
    iowait_(0), irq_(0), softirq_(0), steal_(0)
{}

uint64_t cpu_usage::cpu_stat::sum() const {
    return user_ + nice_ + system_ + idle_ +
           iowait_ + irq_ + softirq_ + steal_;
}

cpu_usage::cpu_usage_map_t cpu_usage::get_cpu_usage() {
    cpu_stats_map_t old_stats = stats_;

    update_cpu_stats();

    cpu_usage_map_t cpu_usage;
    for (auto const& kv : stats_) {
        uint64_t sum_diff;
        uint64_t idle_diff;
        if (0 == old_stats.count(kv.first)) {
            // first run or CPU newly appeared,
            // calculate CPU usage since start
            sum_diff = kv.second.sum();
            idle_diff = kv.second.idle_;
        } else {
            // previous run present,
            // calculate CPU usage since previous run
            sum_diff  = kv.second.sum() - old_stats[kv.first].sum();
            idle_diff = kv.second.idle_ - old_stats[kv.first].idle_;
        }

        double usage;
        if (sum_diff <= 0) {
            usage = 0.0;
        } else {
            usage = 100.0 - (idle_diff / (double)sum_diff) * 100.0;
        }
        cpu_usage.emplace(kv.first, usage);
    }

    return cpu_usage;
}

void cpu_usage::update_cpu_stats() {
    std::ifstream file("/proc/stat", std::ifstream::in);
    if (!file.good()) {
        std::cerr << "Error: cannot open /proc/stat" << std::endl;
        stats_ = cpu_stats_map_t();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        assert(!line.empty());
        std::stringstream sstr(line);
        std::string entry;
        sstr >> entry;
        if (entry.rfind("cpu", 0) != 0) {
            // we read all "cpu..." entries
            break;
        }

        cpu_stat stat = {};
        // gather all fields but ignore the last two fields guest and guest_nice.
        // they are already included in user and nice, respectively. see
        // https://unix.stackexchange.com/questions/178045/proc-stat-is-guest-counted-into-user-time
        sstr >> stat.user_ >> stat.nice_ >> stat.system_ >> stat.idle_
             >> stat.iowait_ >> stat.irq_ >> stat.softirq_ >> stat.steal_;

        stats_[entry] = std::move(stat);
    }
};
