#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>

#include "cpu_usage.h"

cpu_usage::cpu_info::cpu_info() :
    usage_(0.0), user_(0.0), system_(0.0),
    iowait_(0.0), irq_(0.0), softirq_(0.0)
{}

cpu_usage::cpu_stat::cpu_stat() :
    user_(0), nice_(0), system_(0), idle_(0),
    iowait_(0), irq_(0), softirq_(0), steal_(0)
{}

uint64_t cpu_usage::cpu_stat::sum() const {
    return user_ + nice_ + system_ + idle_ +
           iowait_ + irq_ + softirq_ + steal_;
}

cpu_usage::cpu_info_map_t cpu_usage::get_cpu_usage() {
    cpu_stats_map_t old_stats = stats_;

    update_cpu_stats();

    cpu_info_map_t cpu_info_map;
    for (auto const& kv : stats_) {
        uint64_t sum_diff;
        uint64_t idle_diff;
        uint64_t user_diff;
        uint64_t system_diff;
        uint64_t iowait_diff;
        uint64_t irq_diff;
        uint64_t softirq_diff;
        if (0 == old_stats.count(kv.first)) {
            // first run or CPU newly appeared,
            // calculate CPU usage since start
            sum_diff = kv.second.sum();
            idle_diff = kv.second.idle_;
            user_diff = kv.second.user_ + kv.second.nice_;
            system_diff = kv.second.system_;
            iowait_diff = kv.second.iowait_;
            irq_diff = kv.second.irq_;
            softirq_diff = kv.second.softirq_;
        } else {
            // previous run present,
            // calculate CPU usage since previous run
            sum_diff  = kv.second.sum() - old_stats[kv.first].sum();
            idle_diff = kv.second.idle_ - old_stats[kv.first].idle_;
            user_diff = kv.second.user_ + kv.second.nice_ - old_stats[kv.first].user_ - old_stats[kv.first].nice_;
            system_diff = kv.second.system_ - old_stats[kv.first].system_;
            iowait_diff = kv.second.iowait_ - old_stats[kv.first].iowait_;
            irq_diff = kv.second.irq_ - old_stats[kv.first].irq_;
            softirq_diff = kv.second.softirq_ - old_stats[kv.first].softirq_;
        }

        cpu_info info;
        if (sum_diff > 0) {
            info.usage_ = 100.0 - (idle_diff / (double)sum_diff) * 100.0;
            info.user_ = (user_diff / (double)sum_diff) * 100.0;
            info.system_ = (system_diff / (double)sum_diff) * 100.0;
            info.iowait_ = (iowait_diff / (double)sum_diff) * 100.0;
            info.irq_ = (irq_diff / (double)sum_diff) * 100.0;
            info.softirq_ = (softirq_diff / (double)sum_diff) * 100.0;
        }
        cpu_info_map.emplace(kv.first, info);
    }

    return cpu_info_map;
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
