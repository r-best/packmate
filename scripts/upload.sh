# Build application
if ! /home/bobby/.pico-sdk/ninja/v1.12.1/ninja -C /home/bobby/Desktop/packmate/build; then
    echo "Build failed, stopping upload script"
    exit 1
fi

# If Pico is not already in BOOTSEL mode, reboot into it
if ! sudo fdisk -l | grep -q 'RP2350'; then
    sudo /home/bobby/.pico-sdk/picotool/2.2.0-a4/picotool/picotool reboot -f -u
    sleep 1
fi

# Upload to Pico
sudo /home/bobby/.pico-sdk/picotool/2.2.0-a4/picotool/picotool load /home/bobby/Desktop/packmate/build/packmate.elf -fx
