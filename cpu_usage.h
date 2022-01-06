#ifndef CPU_USAGE_H
#define CPU_USAGE_H

#include <cstdint>
#include <map>
#include <string>

/// calculates current CPU usage
class cpu_usage {
public:
    /// holds resulting information about where CPU time has been spent
    class cpu_info {
    public:
        cpu_info();

        double usage_, user_, system_, iowait_, irq_, softirq_;
    };

    /// CPU usage mapping '<cpu, cpu_info>'
    using cpu_info_map_t = std::map<std::string, cpu_info>;

    /// updates CPU statistics and returns average CPU usage since last call,
    /// will return an empty map during the first call
    cpu_info_map_t get_cpu_usage();

private:
    /// holds raw information about where CPU time has been spent, for internal usage
    class cpu_stat {
    public:
        cpu_stat();

        /// return sum over all fields
        uint64_t sum() const;

        uint64_t user_, nice_, system_, idle_,
                 iowait_, irq_, softirq_, steal_;
    };

    /// update CPU statistics
    void update_cpu_stats();

    /// CPU statistics mapping '<cpu, stats>'
    using cpu_stats_map_t = std::map<std::string, cpu_stat>;

    /// last CPU statistics
    cpu_stats_map_t stats_;
};

#endif // CPU_USAGE_H
