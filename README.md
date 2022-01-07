# MPU 9250 driver for the RP2040

## Dependencies

    cmake
    make
    gcc-arm-eabi
    pico-sdk (optional, will be downloaded if not in path)

## Build

    mkdir build
    cd build
    cmake..
    make
    
## Flash over USB

Press the "boot" button and plug in the Pico board. 
Copy the file "rp2040_mpu9250.uf2" to the mass storage device.


## Flash with a programmer

With pico-probe debugger (Using a pico as programmer)

In build directory:

    make flash
    
Modify the "CMake.txt" to use a different debugger.
