# 2023/09/11
## Note
There are several bugs in MCO configuration in CubeMX:
- The GPIO speed for MCO pin must be set to **VERY HIGH** manually otherwise no output will be observed
- When MCO source is selected to **PLLCLK**, the output frequency showed in CubeMX does not match with measurement. In my case, it is 4x higher
## Todo
- Isolate USB feedback and audio data transfer
	1. Setup **TIM2** for SOF counting
	2. Setup **TIM5** for system clock counting
	3. Update feedback value in IRQ handler of **TIM2**
    4. The feedback value is store in a global variable

Respect to the standard, the sampling rate should be measured for not less than one second (i.e. 8000 SOFs). However, if the sampling clock is derived from a higher frequency clock (e.g. SYSCLK), then it is prefectly fine to measure the high frequency clock, for a shorter duration, as long as precision is not compromised.

# 2023/9/12
## Todo
- Make feedback work for every sampling frequency
- Switch sync source from TIM simulation to real I2S transfer complete DMA IRQ

I want the I2S work in circular mode, but generate interrption after transmitting every sample, instead of all. The HAL library is a complete nonsense to me. Maybe I should write my own "LL" library? Looks some workaround is required...

Okay, finally got I2S and DMA working. The major problem is interrupt priority: The USB_OTG_IRQHandler takes too long that blocks I2S DMA transfer complete callbacks. Priotize this solves the problem. 

# 2023/9/13
## Todo
- Make audio buffer adaptive to packet size
- Implement sampling clock change logic (PLL settings)
- Support different bit depths

The DMA IRQ is overwhelming the core... I am considering to use a dedicated TIM for synchronization and change DMA back to circular mode.

I have came up a point: Connect LRCK to an external interrupt to serve as sync signal... Hope this will work.

Ok, that works!

