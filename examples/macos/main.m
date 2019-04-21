// Copyright 2019 Dietrich Epp.
// This file is licensed under the terms of the MIT license. See LICENSE.txt
// for details.
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
@end

@implementation KCView
- (void)keyDown:(NSEvent *)event {
    int macOSCode = [event keyCode];
    int hidCode = 0 <= macOSCode && macOSCode < 128 ? MAC_NATIVE_TO_HID[macOSCode] : 255;
    _macOSKeyCode.stringValue = [NSString stringWithFormat:@"%d", macOSCode];
    _hidKeyCode.stringValue =
        hidCode == 255
            ? @"none"
            : [NSString stringWithFormat:@"%d (%s)", hidCode, keyid_name_from_code(hidCode)];
}
- (BOOL)acceptsFirstResponder {
    return YES;
}
@end

int main(int argc, const char *argv[]) {
    return NSApplicationMain(argc, argv);
}
