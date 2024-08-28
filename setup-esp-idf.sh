#!/bin/bash

set -e

ESP32_PROJ="esp32"

# Function to check if a directory exists and prompt for deletion
check_dir() {
    local DIR="$1"

    # Check if the directory exists
    if [ -d "$DIR" ]; then
        # Prompt the user for confirmation
        read -p "Directory '$DIR' already exists. Do you want to delete it? (y/n): " choice

        # If the user confirms, delete the directory and return 0
        if [ "$choice" == "y" ]; then
            rm -rf "$DIR"
            echo "Directory '$DIR' has been deleted."
            return 0
        else
            echo "Directory '$DIR' was not deleted."
            return 1
        fi
    fi

    # If the directory doesn't exist, return 0
    return 0
}

# Create project folder only if check_dir returns 0
if check_dir "$ESP32_PROJ"; then
    mkdir -p "$ESP32_PROJ"
    cd "$ESP32_PROJ"
else
    echo "Exiting script."
    exit 1
fi


# Update package list and install prerequisite tools
sudo apt-get update
sudo apt-get install -y git wget flex bison gperf python3 python3-pip python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0


# Get ESP-IDF source
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf

# Run the install script and export the environment
./install.sh
. ./export.sh

# Check versions
python3 --version
cmake --version
ninja --version
idf.py --version

<< 'COMMENT'
ESP-WROOM-32 ESP32 ESP-32S Development Board 

1. hello world
cd ~/esp-idf/esp-idf/examples/get-started/hello_world
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash
idf.py -p /dev/ttyUSB0 monitor

2. blink LED
cd ~/esp32/esp-idf/examples/get-started/blink/
idf.py set-target esp32
idf.py menuconfig
    In the Example Configuration menu:
        Select the LED type in the Blink LED type option. (default)
            Use GPIO for regular LED (default)
                change pin number from 5(default) to 2 for ESP32-WROOM
idf.py build
idf.py -p /dev/ttyUSB0 flash
idf.py -p /dev/ttyUSB0 monitor

COMMENT
