# Multiple USB Cameras On A Raspberry Pi
(Or other PC like device with a single USB controller chip)


## Why is this an issue
Most cheap UVC cameras only offer streaming over _isochronous_ USB, as opposed to bulk transfer.  That means that any bandwidth they plan to consume must be reserved, and the total reservations on the USB controller must come in under 80% of the 480Mbps offered by USB2.  With uncompressed frames it's likely that you can't reserve enough bandwidth to use even 2 cameras on a single controller, and the PI 4 and most other single board computers surveyed only has one USB2 controller.

## Things that don't work
USB3 doesn't help because it doesn't actually separate USB2 streams, it just shunts them all to the usb2 controller.

A hub doesn't help because the controller is still shared.

Lowering frame rate often doesn't help because the camera has to send the frames at some high speed even if it then doesn't use the reservation for a while between frames.  This seems dumb, but reflects experiments with our cameras.  It seems that the camera can't buffer the frame to stretch it out across more time when sending.

## The start of a solution
Now, what about compressed frames?  Switch to MJPEG compression and the frames get _waaaayy_ smaller, so it should use less bandwidth, right?  Unfortunately most cheap cameras still report a very large bandwidth reservation need.  Far more than the compressed frame will use.

The solution is to use MJPEG and to hack the UVC camera driver to ignore the erroneously high bandwidth required in compressed mode.  You set a bandwidth that is large enough for the actual compressed frames but as small as possible. (Note that bandwidth is not a continuous number here -- the camera or your hack selects a bandwidth in bytes per USB frame (125 microseconds), then the USB endpoint gets set to one of several possible values available on that camera such that the configuration is the smallest one that offers at least as much bandwidth as you asked for.

## Putting it together
We found a patch (source below) for an older version of the kernel to hack the uvc_video driver to give a module parameter that allows forcing the camera bandwidth as required.  The patch was rejected from the kernel for being too "hacky" which is frankly BS because after reviewing the other "workarounds" already in the UVC driver this would have been no less hacky.

Patching a Linux kernel module involves getting the source code, modifying it, and building it, and installing it.  The patch included in this repo has been modified from the original to work with the newer kernel on our PI

A downloadable binary version we built on our pi with kernel 5.15.84-v8+ is
[here](uvcvideo.ko). You can download it and skip to "remove the original driver and load our new
replacement" in the instructions below.

## Follow these steps to build your own patched driver
```bash
#rebuilding uvc_driver on a 64 bit pi

#This sequence was patched together from my command history after the fact.  If you find a flaw, open a github issue so we can fix the instructions

#get packages needed to build kernel
sudo apt install git bc bison flex libssl-dev
#get python 2 for rpi-source to run
sudo apt install python2

#rpi-source is included here, but was originally fetched with
#wget https://raw.githubusercontent.com/RPi-Distro/rpi-source/ceee03b5fb9cee65dd933f4784bb455cfa872a76/rpi-source
#that version (included here) is patched to work with the new raspberry pi 64 bit kernels

#get the kernel source
python2 rpi-source  -q --tag-update
python2 rpi-source --architecture 1
#rpi-source seems to put the source in your home directory regardless of
#where you ran it.  yay.
cd ~/linux

#patch it to allow a new bandwidth_cap parameter
#this patch is from https://www.spinics.net/lists/linux-media/msg175596.html
#but has been modified to work with hte kernel log changes present in newer kernels
#download it from (here)[uvc_bw_patch].
patch -p1 < ../uvc_bw_patch 

#build just the module you want
make -C . M=drivers/media/usb/uvc

#as root, remove the original driver and load our new replacement
sudo rmmod uvcvideo
sudo insmod drivers/media/usb/uvc/uvcvideo.ko

#as root, set a bandwidth cap using the new driver parameter:
#1000 means 1000 bytes per frame, and a frame is 125us, so 1000 is 64 Mbit.
#this seems to allow 4 MJPEG streams at 1280x1024 for us.
echo 1000 | sudo tee /sys/module/uvcvideo/parameters/bandwidth_cap


#if you have problems, you can see the driver debug info by turning up the loging level then watching the kernel log:
echo 0xffff |sudo tee /sys/module/uvcvideo/parameters/trace
sudo dmesg --follow

#if things are working, make it permanent by copying to the system module location:

#if your kernel is different, change this path
cd  /usr/lib/modules/5.15.84-v8+/kernel/drivers/media/usb/uvc

sudo mv uvcvideo.ko.xz uvcvideo.ko.xz.bak

#modify this path to where you built the driver
sudo cp /home/robotics/linux/drivers/media/usb/uvc/uvcvideo.ko .
sudo depmod

#now make the bandwidth cap permanent
cat | sudo tee /etc/modprobe.d/uvc.conf << END
options uvcvideo bandwidth_cap=1000
END



```

## More information and source links that this solution is derived from:

[Blog entry](https://www.thegoodpenguin.co.uk/blog/multiple-uvc-cameras-on-linux/) on the problem and original patch author

[rpi-source script](https://github.com/RPi-Distro/rpi-source) for retrieving kernel source code on a raspberry pi

[building a single kernel module](https://stackoverflow.com/questions/8744087/how-to-recompile-just-a-single-kernel-module) on a modern kernal for when you don't have all day

[patched rpi-source](https://github.com/aircrack-ng/rtl8812au/issues/933) that works with newer 64 bit kernels

