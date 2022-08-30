#ifndef JOYCOND_VIRT_CTLR_H
#define JOYCOND_VIRT_CTLR_H

#include "phys_ctlr.h"

#include <memory>
#include <vector>

class virt_ctlr
{
    private:

    public:
        virt_ctlr() {}
        virtual ~virt_ctlr() {}

        virtual void handle_events(int fd) = 0;
        virtual bool contains_phys_ctlr(std::shared_ptr<phys_ctlr> const ctlr) const = 0;
        virtual bool contains_phys_ctlr(char const *devpath) const = 0;
        virtual bool contains_fd(int fd) const = 0;
        virtual std::vector<std::shared_ptr<phys_ctlr>> get_phys_ctlrs() = 0;
        virtual void remove_phys_ctlr(const std::shared_ptr<phys_ctlr> phys) = 0;
        virtual void add_phys_ctlr(std::shared_ptr<phys_ctlr> phys) = 0;
        virtual enum phys_ctlr::Model needs_model() = 0;
        virtual bool supports_hotplug() {return false;}
        virtual bool mac_belongs(const std::string& mac) const {return false;}

        // Used to determine if this virtual controller should be removed from paired controllers list
        virtual bool no_ctlrs_left() {return true;}
};

#define XBOX_HACK 1

// Nintendo Switch Pro Controller
//a:b0,b:b1,back:b8,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,dpup:h0.1,guide:b12,leftshoulder:b4,leftstick:b10,lefttrigger:b6,leftx:a0,lefty:a1,misc1:b13,rightshoulder:b5,rightstick:b11,righttrigger:b7,rightx:a2,righty:a3,start:b9,x:b2,y:b3,platform:Windows,
// Xbox 360 Controller
//a:b0,b:b1,back:b6,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,dpup:h0.1,leftshoulder:b4,lefttrigger:b10,rightshoulder:b5,righttrigger:b11,start:b7,x:b2,y:b3,platform:Windows,

#ifdef XBOX_HACK
//03000000 5e04 0000 8e02 0000 1001 0000,Xbox 360 Controller,
//a:b0,b:b1,back:b6,dpdown:h0.4,dpleft:h0.8,dpright:h0.2,dpup:h0.1,guide:b8,leftshoulder:b4,leftstick:b9,lefttrigger:a2,leftx:a0,lefty:a1,rightshoulder:b5,rightstick:b10,righttrigger:a5,rightx:a3,righty:a4,start:b7,x:b2,y:b3,platform:Linux,
#define VIRT_PRO_NAME   "Xbox 360 Controller"
#define VIRT_COMBINED   VIRT_PRO_NAME
#define VIRT_VENDOR     0x045e
#define VIRT_PRODUCT    0x028e
#define VIRT_VERSION    0x0110
#define ANALOG_TRIGGERS 1
#define TRIGGER_UNPRESS 0
// Max (and typical) is 255, but that triggers a nasty bug in Kodi that causes phantom scroll events
// Presumably real controllers don't often report the full values
// Experimentally 253 is the highest value you can get away with, rounding down a bit to be safe
// Anything greater than XINPUT_GAMEPAD_TRIGGER_THRESHOLD(30) should be fine for most things,
// but already observed it needs to be higher than 128 in some games (tested 192 to get working)
#define TRIGGER_PRESS   250
#else
//06000000 7e05 0000 0820 0000 0000 0000,Nintendo Switch Combined Joy-Cons,
//a:b0,b:b1,back:b9,dpdown:b15,dpleft:b16,dpright:b17,dpup:b14,guide:b11,leftshoulder:b5,leftstick:b12,lefttrigger:b7,leftx:a0,lefty:a1,rightshoulder:b6,rightstick:b13,righttrigger:b8,rightx:a2,righty:a3,start:b10,x:b3,y:b2,platform:Linux,
#define VIRT_PRO_NAME "Nintendo Switch Virtual Pro Controller"
#define VIRT_COMBINED "Nintendo Switch Combined Joy-Cons"
#define VIRT_VENDOR   0x057e
#define VIRT_PRODUCT  0x2008
#define VIRT_VERSION  0x0000
#endif

#endif
