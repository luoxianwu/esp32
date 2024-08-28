#!/bin/bash

# Stop the script if any command fails
set -e -x

# Function to check if version1 is greater than or equal to version2; return 0 if $1 => $2
version_ge() {
    printf '%s\n%s\n' "$2" "$1" | sort -V -C   # return 0 if $2 <= $1
}

check_versions() {
    CMAKE_MIN_VERSION="3.20.5"
    PYTHON_MIN_VERSION="3.10.0"
    DTC_MIN_VERSION="1.4.6"

    CMAKE_VERSION=$(cmake --version | grep -oE '[0-9]+\.[0-9]+\.[0-9]+')
    PYTHON_VERSION=$(python3 --version | grep -oE '[0-9]+\.[0-9]+\.[0-9]+')
    DTC_VERSION=$(dtc --version | grep -oE '[0-9]+\.[0-9]+\.[0-9]+')

    if  ! version_ge "$CMAKE_VERSION" "$CMAKE_MIN_VERSION"; then
        echo "Error: CMake version $CMAKE_VERSION is less than required $CMAKE_MIN_VERSION."
        exit 1
    fi

    if ! version_ge "$PYTHON_VERSION" "$PYTHON_MIN_VERSION"; then
        echo "Error: Python version $PYTHON_VERSION is less than required $PYTHON_MIN_VERSION."
        exit 1
    fi

    if ! version_ge "$DTC_VERSION" "$DTC_MIN_VERSION"; then
        echo "Error: Device Tree Compiler version $DTC_VERSION is less than required $DTC_MIN_VERSION."
        exit 1
    fi

    echo "All dependencies are up to date."
}

# Update and upgrade the system
sudo apt update
sudo apt upgrade -y

# Install the required dependencies
sudo apt install --no-install-recommends -y git cmake ninja-build gperf \
  ccache dfu-util device-tree-compiler wget \
  python3-dev python3-pip python3-setuptools python3-tk python3-wheel xz-utils file \
  make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1

# Check if Ubuntu version is older than 22.04, and if so, add Kitware APT repository
if [[ $(lsb_release -rs) < "22.04" ]]; then
  wget https://apt.kitware.com/kitware-archive.sh
  sudo bash kitware-archive.sh
fi

# Verify the versions of the main dependencies
echo "Verifying installed versions:"
cmake --version
python3 --version
dtc --version

# Check version compatibility
check_versions

# Install Python venv package
sudo apt install -y python3-venv

# Create a new virtual environment
python3 -m venv ~/zephyrproject/.venv

# Activate the virtual environment
source ~/zephyrproject/.venv/bin/activate

# Install West
pip install west

# Get the Zephyr source code
west init ~/zephyrproject
cd ~/zephyrproject
west update

# Export the Zephyr CMake package
west zephyr-export

# Install additional Python dependencies for Zephyr
pip install -r ~/zephyrproject/zephyr/scripts/requirements.txt

# Check if the Zephyr SDK is already installed
if [ ! -d "$HOME/zephyr-sdk-0.16.8" ]; then
    cd ~
    wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.8/zephyr-sdk-0.16.8_linux-x86_64.tar.xz
    wget -O - https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.8/sha256.sum | shasum --check --ignore-missing
    # Extract and install the Zephyr SDK
    tar -xvf zephyr-sdk-0.16.8_linux-x86_64.tar.xz
    cd zephyr-sdk-0.16.8
    ./setup.sh
    # Set environment variables for Zephyr SDK
    export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
    export ZEPHYR_SDK_INSTALL_DIR=~/zephyr-sdk-0.16.8
else
    echo "Zephyr SDK is already installed."
fi


# Set environment variables for Zephyr SDK
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=~/zephyr-sdk-0.16.8

# Install udev rules, which allow you to flash most Zephyr boards as a regular user:
sudo cp ~/zephyr-sdk-0.16.8/sysroots/x86_64-pokysdk-linux/usr/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d
sudo udevadm control --reload

# Inform the user that the setup is complete
echo "Zephyr development environment setup is complete."
echo "Remember to activate the virtual environment with 'source ~/zephyrproject/.venv/bin/activate' before working on your projects."

<< 'COMMENT'
build "Hello World" Sample
cd ~/zephyrproject/zephyr/samples/hello_world/
west build -p always -b esp32_devkitc_wroom --pristine=always .
west flash --esp-device /dev/ttyUSB0
picocom -b 115200 /dev/ttyUSB0
    press reset button on ESP32 board. you will see hello world log

COMMENT