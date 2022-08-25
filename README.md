# joycond XBOX HACK
joycond is a linux daemon which uses the evdev devices provided by hid-nintendo (formerly known as hid-joycon) to implement joycon pairing.

hid-nintendo is a Linux kernel driver that handles pro controllers and individual joycons and presents them as gamepads.
hid-nintendo is currently in review on the linux-input mailing list.
The most recent patches are currently found at https://github.com/DanielOgorchock/linux
If the module is not already provided by your kernel/distro, [install the DKMS](https://github.com/nicman23/dkms-hid-nintendo).

This is a fork of [vanilla joycond](https://github.com/DanielOgorchock/joycond) that does the following (potentially terrible) things:
* Creates virtual controllers that look like wired XBox 360 controllers
* Swaps face buttons A and B to match expected confirm/back behavior
* Blocks games from accessing the IMU motion control "controller"

# Installation
1. Clone the repo
2. Install requirements (`sudo apt install libevdev-dev` or `sudo dnf install libevdev-devel libudev-devel`)
3. `cmake .`
4. `sudo make install`
5. `sudo systemctl enable --now joycond`
6. (Only if you need Motion Controls) `sudo rm /lib/udev/rules.d/99-joycond-disable-imu.rules`

# Usage
When a joy-con or pro controller is connected via bluetooth or USB, the player LEDs should start blinking periodically.
This signals that the controller is in pairing mode.

**NOT REALLY USEFUL:**
For the pro controller, pressing both triggers will "pair" it in passthrough mode.

**THIS IS WHAT YOU WANT IN THIS HACK:**
For the pro controller, pressing Plus and Minus will pair it as a virtual XBox 360 controller.
This is useful when using Steam (or anything else!).

**BROKEN, NEED TO FIX:**
With the joy-cons, to use a single contoller alone, hold ZL and L at the same time (ZR and R for the right joy-con).
Alternatively, hold both S triggers at once.

To combine two joy-cons into a virtual input device, press a *single* trigger on both of them at the same time.
A new uinput device will be created called "Nintendo Switch Combined Joy-Cons".

Rumble support is now functional for the combined joy-con uinput device.

# Why the fork?
Support for the Pro controller in most games inside and outside of Steam is a terrifying mess, and this presents an easy solution to the problem.
Some good information is in the [Arch Wiki](https://wiki.archlinux.org/title/Gamepad#Nintendo_Switch_Pro_Controller_and_Joy-Cons), but I'll mention some specific issues here.

Thanks to SDL making a game with great controller support shouldn't be hard, but many still do screw it up.
Usually in the following ways:
* They are hardcoded or tested to only work with a specific set of controllers, probably just XBox controllers.
* They only look at the "first" controller, rather than determine first controller by a button press.
* They get confused by the "IMU" motion controls "controller", particularly if it is "first"

Steam adds two more wrinkles:
* Steam tries to provide a conflicting userspace driver for Pro controllers over hidraw, and fails to fallback to generic support if it can't
* Steam provides its own virtualized controller remapping interface over uinput

To fix these problems we need to:
* Block Steam from accessing hidraw (vanilla joycond already does this with a udev rule)
* Change the controller ID so Steam doesn't try to use hidraw, otherwise it refuses to work (vanilla joycond does this by providing a special "Nintendo Switch Combined Joy-Cons" ID)
* Make sure Steam does actually have access to uinput (it should via its own udev rules), because the controller configuration can be turned on and off and allows for the best of both worlds
* Block out IMU access from games that can't actually handle it properly, it also helps that its less likely to take the first controller spot
* Just give up on a saner world and make the damn thing look like an XBox controller

This way all native games see just an XBox controller, and probably "just work".
Steam also sees an XBox controller and doesn't try hidraw shenanigans, but still provides an excellent remapping interface.
If the remap interface doesn't work, it can be "Forced off" to act like a native game.
It the native interface doesn't work, or you have need of remapping, you can try Steam's configuration interface and that might work.

If you have need of motion controls then blocking IMU might be a problem.
You can try removing the udev rule in this case, just keep in mind the above mentioned issues might come into play.

For debugging purposes I recommend checking out [sdl-jstest](https://github.com/Grumbel/sdl-jstest).
It's very useful to know what gamepads are enumerated by a typical SDL or SDL2 game and in what order.

[xboxdrv](https://xboxdrv.gitlab.io/) also presents a virtualized XBox controller, as does [MoltenGamepad](https://github.com/jgeumlek/MoltenGamepad).
At time of writing, both of these can be difficult to use, configure, or even build.
joycond "just works", and is something you need anyway 90% of the time when using these controllers.

# Planned further changes?
* Make combined joycons work
* Always pair Pro controller "virtually"
* Compile time switches for A/B and X/Y face button swaps
* Compile time switch for XBox Controller vs. Combined Joy-Cons
* Runtime face button swap support on button combo
* Runtime Combined vs. XBox swap on button combo
* Runtime Combined vs. XBox vs IMU on button combo (Remove udev hack and handle gatekeep IMU in joycond)
* List sdl and sdl2 enumerations?
* Ensure Steam has uinput access?
