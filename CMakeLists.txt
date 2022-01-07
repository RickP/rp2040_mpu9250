cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT on)

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(rp2040_mpu9250)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(rp2040_mpu9250
    src/main.c
    src/mpu-9250.c
    src/kiss_fft.c
    src/kiss_fftr.c
)

pico_enable_stdio_usb(rp2040_mpu9250 1)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(rp2040_mpu9250 pico_stdlib hardware_i2c)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(rp2040_mpu9250)

add_custom_target(flash
    COMMAND openocd -f interface/picoprobe.cfg -f target/rp2040.cfg -c "program rp2040_koax.elf verify reset exit"
    DEPENDS rp2040_koax
)
