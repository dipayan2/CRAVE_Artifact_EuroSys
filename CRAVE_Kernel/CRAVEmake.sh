#!/bin/sh
make odroidxu4_defconfig
make -j8
sudo make modules_install
sudo cp -f arch/arm/boot/zImage /media/boot
sudo cp -f arch/arm/boot/dts/exynos5422-odroid*dtb /media/boot
sudo cp -f arch/arm/boot/dts/overlays/*.dtbo /media/boot/overlays
sync