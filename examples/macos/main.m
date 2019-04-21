// Copyright 2019 Dietrich Epp.
// This file is licensed under the terms of the MIT license. See LICENSE.txt
// for details.
#import "keycode.h"
#import "keyid.h"
#import "keytable.h"

#import <Cocoa/Cocoa.h>

@interface KCApplicationDelegate : NSObject
@end

@implementation KCApplicationDelegate
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
    return YES;
}
@end

@interface KCView : NSView
@property IBOutlet NSTextField *macOSKeyCode;
@property IBOutlet NSTextField *hidKeyCode;
- (void)showKey:(int)keyCode hidCode:(int)hidCode;
@end

struct keycode_macmodifier {
    unsigned hid_code;
    unsigned modifier_mask;
};

// For modifiers, we receive NSEventTypeFlagsChanged. This contains the keycode
// of the modifier which changed, but in order to tell the difference between
// key up and key down, we have to examine the modifier flags. This maps the
// modifiers to their associated flags.
//
// The flag values can be found in IOLLEvent.h. Note that we do not include the
// function key. This is by design.
static const unsigned MAC_MODIFIERS[] = {
    0x00000010, // Right Command: NX_DEVICERCMDKEYMASK
    0x00000008, // Left Command: NX_DEVICELCMDKEYMASK
    0x00000002, // Left Shift: NX_DEVICELSHIFTKEYMASK
    0x00010000, // Caps Lock: NX_ALPHASHIFTMASK
    0x00000020, // Left Option: NX_DEVICELALTKEYMASK
    0x00000001, // Left Control: NX_DEVICELCTLKEYMASK
    0x00000004, // Right Shift: NX_DEVICERSHIFTKEYMASK
    0x00000040, // Right Option: NX_DEVICERALTKEYMASK
    0x00002000, // Right Control NX_DEVICERCTLKEYMASK
};

@implementation KCView
- (void)showKey:(int)keyCode hidCode:(int)hidCode {
    _macOSKeyCode.intValue = keyCode;
    NSString *hidStr = @"none";
    if (hidCode != 255) {
        const char *name = keyid_name_from_code(hidCode);
        if (name == NULL) {
            name = "unknown";
        }
        hidStr = [NSString stringWithFormat:@"%d (%s)", hidCode, name];
    }
    _hidKeyCode.stringValue = hidStr;
}
- (void)keyDown:(NSEvent *)event {
    int keyCode = [event keyCode];
    int hidCode = 0 <= keyCode && keyCode < 128 ? MAC_NATIVE_TO_HID[keyCode] : 255;
    [self showKey:keyCode hidCode:hidCode];
}
- (void)flagsChanged:(NSEvent *)event {
    unsigned keyCode = [event keyCode];
    if (54 <= keyCode && keyCode <= 62) {
        unsigned hidCode = MAC_NATIVE_TO_HID[keyCode];
        unsigned modMask = MAC_MODIFIERS[keyCode - 54];
        unsigned modFlags = [event modifierFlags];
        NSLog(@"modflags: %08x", modFlags);
        if ((modFlags & modMask) != 0) {
            [self showKey:keyCode hidCode:hidCode];
        }
    }
}
- (BOOL)acceptsFirstResponder {
    return YES;
}
@end

int main(int argc, const char *argv[]) {
    return NSApplicationMain(argc, argv);
}
