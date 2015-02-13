/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*    MacOSX/LMWindow.mm                             oooooo       oooooo      */
/*                                                 oooooooooo   oooooooooo    */
/*                                                         o%%%%%o            */
/*                                                         %:::::%            */
/*                                                        %:::::::%           */
/*    This file is part of the                             %:::::%            */
/*    Lums library.                                         %%%%%             */
/*                                                                            */
/* ************************************************************************** */

#import <Lums/Event.hpp>
#import "LMWindow.hpp"

@implementation LMWindow

-(void)keyDown:(NSEvent*)event
{
    lm::Event e;

    e.type = lm::Event::Type::KeyDown;
    e.key = static_cast<lm::Key>([event keyCode]);
    [self window]->pushEvent(e);
}

-(void)keyUp:(NSEvent*)event
{
    lm::Event e;

    e.type = lm::Event::Type::KeyUp;
    e.key = static_cast<lm::Key>([event keyCode]);
    [self window]->pushEvent(e);
}

void
gamepadWasAdded(void* inContext, IOReturn inResult, void* inSender, IOHIDDeviceRef device)
{
    lm::Event e;

    e.type = lm::Event::Type::GamepadConnected;
    e.gamepad.id = (uintptr_t)device;
    [(LMWindow*)inContext window]->pushEvent(e);
}

void
gamepadWasRemoved(void* inContext, IOReturn inResult, void* inSender, IOHIDDeviceRef device)
{
    lm::Event e;

    e.type = lm::Event::Type::GamepadDisconnected;
    e.gamepad.id = (uintptr_t)device;
    [(LMWindow*)inContext window]->pushEvent(e);
}

void
gamepadAction(void* inContext, IOReturn inResult, void* inSender, IOHIDValueRef value)
{
    static float lx = 0;
    static float ly = 0;
    static float rx = 0;
    static float ry = 0;
    float v;

    IOHIDElementRef element = IOHIDValueGetElement(value);
    IOHIDDeviceRef device = IOHIDElementGetDevice(element);
    int elementValue = IOHIDValueGetIntegerValue(value);
    int usageid = IOHIDElementGetUsage(element);
    int usagepage = IOHIDElementGetUsagePage(element);
    lm::Event event;

    event.gamepad.id = (uintptr_t)device;

    NSLog(@"Page %i ID %i Value %i", usagepage, usageid, elementValue);
    switch (usagepage)
    {
        case 9:
            if (elementValue)
                event.type = lm::Event::Type::ButtonDown;
            else
                event.type = lm::Event::Type::ButtonUp;
            event.gamepad.button = usageid;
            break;

        case 1:
            switch (usageid)
            {
                case 50:
                case 53:
                    if (usageid == 50)
                        event.type = lm::Event::Type::LeftTrigger;
                    else
                        event.type = lm::Event::Type::RightTrigger;
                    event.gamepad.trigger = (float)elementValue / 255.0f;
                    break;

                case 48: // Left stick x
                case 49: // Left stick y
                    event.type = lm::Event::Type::LeftStick;
                    v = (float)elementValue / 32768.0f;
                    if (usageid == 48)
                        lx = v;
                    else
                        ly = v;
                    event.gamepad.stick.x = lx;
                    event.gamepad.stick.y = ly;
                    break;

                case 51: // Right stick x
                case 52: // Right stick y
                    event.type = lm::Event::Type::RightStick;
                    v = (float)elementValue / 32768.0f;
                    if (usageid == 51)
                        lx = v;
                    else
                        ly = v;
                    event.gamepad.stick.x = lx;
                    event.gamepad.stick.y = ly;
                    break;

                default:
                    return;
            }
            break;

        default:
            return;
    }
    [(LMWindow*)inContext window]->pushEvent(event);
}

-(void)setupHid
{
    _hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    NSMutableDictionary* criterion = [[NSMutableDictionary alloc] init];
    [criterion setObject: [NSNumber numberWithInt: kHIDPage_GenericDesktop] forKey: (NSString*)CFSTR(kIOHIDDeviceUsagePageKey)];
    [criterion setObject: [NSNumber numberWithInt: kHIDUsage_GD_GamePad] forKey: (NSString*)CFSTR(kIOHIDDeviceUsageKey)];
    IOHIDManagerSetDeviceMatching(_hidManager, (__bridge CFDictionaryRef)criterion);
    IOHIDManagerRegisterDeviceMatchingCallback(_hidManager, gamepadWasAdded, (void*)self);
    IOHIDManagerRegisterDeviceRemovalCallback(_hidManager, gamepadWasRemoved, (void*)self);
    IOHIDManagerScheduleWithRunLoop(_hidManager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    IOReturn tIOReturn = IOHIDManagerOpen(_hidManager, kIOHIDOptionsTypeNone);
    IOHIDManagerRegisterInputValueCallback(_hidManager, gamepadAction, (void*)self);
}

@synthesize window = _window;

@end