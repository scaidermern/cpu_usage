# cpu_usage

Prints current total and per-core CPU usage on Linux.

The included example program will show both the current CPU core usage and CPU core usage since start.
The sampling duration can be specified in milliseconds via an optional command line argument.
The default samping duration is one second.

## Example
```
$ cpu_usage 5000

average CPU usage since start:
cpu 4.68224%
cpu0 5.53707%
cpu1 4.10635%
cpu2 4.81156%
cpu3 4.277%

current CPU usage:
cpu 0.505817%
cpu0 0.410678%
cpu1 1%
cpu2 0.400802%
cpu3 0.608519%
```

## License
[CC0 1.0](https://creativecommons.org/publicdomain/zero/1.0/legalcode)
(c) [Alexander Heinlein](http://choerbaert.org)
