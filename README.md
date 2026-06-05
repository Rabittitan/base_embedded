# AK embedded base kit - STM32L151C8T6

[![AK foundation logo](/hardware/images/ak-foundation-logo.png)](<https://github.com/the-ak-foundation>)

This kit would not have been possible without the help of [EPCB](https://epcb.vn/pages/frontpage), a Vietnamese IoT solutions provider.

AK embedded base kit, utilizing STM32L151C8T6 chip, is an evaluation kit for advanced embedded software learners.

## Features

- 1.54'' OLED LCD
- 3 push buttons
- 1 buzzer
- RS485
- QWIIC Connect System
- Grove Ecosystem

## Purpose

Students who are enrolled in the AK foundation's embedded training program will make use of this evaluation kit to develop a small unique video game that will be able to run smoothly as well as closely follow an event driven paradigm in embedded systems programming. This repository also contains all the code which would form the AK framework that students can use to facilitate their development process.

We also hope that this repository will also be useful for those are on the look out for a well-built kit to practice their embedded systems programming skills.

![AK base kit](/hardware/images/ak-embedded-base-kit-version-3.jpg)

## Memory map

AK base kit uses the following memory map to run its application code

| Address    | Type        | Binary                                                             | Note                                                 |
|------------|-------------|--------------------------------------------------------------------|------------------------------------------------------|
| 0x08000000 | Boot        | [Boot](/hardware/bin/ak-base-kit-stm32l151-boot.bin)               | N/A                                                  |
| 0x08002000 | BSF         | N/A                                                                | Memory for data sharing between Boot and Application |
| 0x08003000 | Application | [Application](/hardware/bin/ak-base-kit-stm32l151-application.bin) | N/A                                                  |

**Note:** After loading the boot and application firmware, you can use [AK - Flash](https://github.com/ak-embedded-software/ak-flash), a CLI to work with the AK base kit, to load the application directly through the kit's USB port.

```sh
ak_flash /dev/ttyUSB0 ak-base-kit-stm32l151-application.bin 0x08003000
```

## Hardware

[AK base kit's schematic](/hardware/schematic/schematic-ak-embedded-base-kit-version-3.pdf)

![AK base kit top view schematic](/hardware/images/board-view-top.png)
![AK base kit bottom view schematic](/hardware/images/board-view-bottom.png)

## Reference

| Topic | Link |
| ------ | ------ |
| Tutorials | <https://epcb.vn/blogs/ak-embedded-software> |
| Vendor | <https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu> |
