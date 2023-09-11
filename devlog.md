# 2023/09/11
There are several bugs in MCO configuration in CubeMX:
- The GPIO speed for MCO pin must be set to **VERY HIGH** manually otherwise no output will be observed.
- When MCO source is selected to **PLLCLK**, the output frequency showed in CubeMX does not match with measurement. In my case, it is 4x higher.