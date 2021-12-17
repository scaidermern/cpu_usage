#ifndef CPU_USAGE_H
#define CPU_USAGE_H

#include <cstdint>
#include <map>
#include <string>

/// calculates current CPU usage
class cpu_usage {
public:
    /// holds information about where CPU time has been spent
    class cpu_stat {
    public:
        cpu_stat();

        /// return sum over all fields
        uint64_t sum() const;

        uint64_t user_, nice_, system_, idle_,
                 iowait_, irq_, softirq_, steal_;
    };

    /// CPU usage mapping '<cpu, usage>'
    using cpu_usage_map_t = std::map<std::string, double>;

    /// updates CPU statistics and returns average CPU usage since last call,
    /// will return an empty map during the first call
    cpu_usage_map_t get_cpu_usage();

private:
    /// update CPU statistics
    void update_cpu_stats();

    /// CPU statistics mapping '<cpu, stats>'
    using cpu_stats_map_t = std::map<std::string, cpu_stat>;

    /// last CPU statistics
    cpu_stats_map_t stats_;
};

#endif // CPU_USAGE_H
