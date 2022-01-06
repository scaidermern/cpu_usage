# cpu_usage

Prints current total and per-core CPU usage on Linux.

The included example program will show both the current CPU core usage and CPU core usage since start.
The sampling duration can be specified in milliseconds via an optional command line argument.
The default samping duration is one second.

## Example
```
$ cpu_usage 5000
average CPU usage since start:
cpu 17.3559% usage 12.9871% user 4.10769% system 0.0979491% iowait 0% irq 0.163206% softirq
cpu0 16.9045% usage 12.9096% user 3.5879% system 0.104692% iowait 0% irq 0.302236% softirq
cpu1 18.0062% usage 13.1694% user 4.70669% system 0.0953437% iowait 0% irq 0.0348182% softirq
cpu2 18.561% usage 13.6061% user 4.55917% system 0.0943583% iowait 0% irq 0.301389% softirq
cpu3 15.9464% usage 12.2593% user 3.57735% system 0.0972373% iowait 0% irq 0.0125049% softirq

current CPU usage:
cpu 96.9424% usage 93.9348% user 2.05514% system 0.250627% iowait 0% irq 0.701754% softirq
cpu0 96.1924% usage 95.1904% user 1.002% system 0% iowait 0% irq 0% softirq
cpu1 99.6008% usage 91.6168% user 3.99202% system 1.1976% iowait 0% irq 2.79441% softirq
cpu2 100% usage 98% user 2% system 0% iowait 0% irq 0% softirq
cpu3 91.9679% usage 90.3614% user 1.20482% system 0.200803% iowait 0% irq 0.200803% softirq
```

## License
[CC0 1.0](https://creativecommons.org/publicdomain/zero/1.0/legalcode)
(c) [Alexander Heinlein](http://choerbaert.org)
