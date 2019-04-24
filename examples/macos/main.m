// Copyright 2019 Dietrich Epp.
// This file is licensed under the terms of the MIT license. See LICENSE.txt
// for details.
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
- (void)showKey:(unsigned)keyCode hidCode:(unsigned)hidCode;
@end

@implementation KCView
- (void)showKey:(unsigned)keyCode hidCode:(unsigned)hidCode {
    const char *keyName = keycode_macos_rawname(keyCode);
    if (keyName == NULL) {
        keyName = "unknown";
    }
    _macOSKeyCode.stringValue = [NSString stringWithFormat:@"0x%02x (%s)", keyCode, keyName];
    const char *hidName = keycode_macos_name(hidCode);
    if (hidName == NULL) {
        hidName = "unknown";
    }
    _hidKeyCode.stringValue = [NSString stringWithFormat:@"0x%02x (%s)", hidCode, hidName];
}
- (void)keyDown:(NSEvent *)event {
    unsigned keyCode = [event keyCode];
    unsigned hidCode = keyCode < 128 ? KEYCODE_MACOS_TO_HID[keyCode] : 0;
    [self showKey:keyCode hidCode:hidCode];
}
- (void)flagsChanged:(NSEvent *)event {
    unsigned keyCode = [event keyCode];
    unsigned hidCode = keyCode < 128 ? KEYCODE_MACOS_TO_HID[keyCode] : 0;
    unsigned mask = keycode_macos_modifier(keyCode);
    unsigned modifiers = [event modifierFlags];
    if ((mask & modifiers) != 0) {
        // Key down.
        [self showKey:keyCode hidCode:hidCode];
    }
}
- (BOOL)acceptsFirstResponder {
    return YES;
}
@end

int main(int argc, const char *argv[]) {
    return NSApplicationMain(argc, argv);
}
