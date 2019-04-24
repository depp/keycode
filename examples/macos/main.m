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
- (void)showKey:(int)keyCode;
@end

@implementation KCView
- (void)showKey:(int)keyCode {
    const char *keyName = keycode_macos_rawname(keyCode);
    if (keyName == NULL) {
        keyName = "unknown";
    }
    _macOSKeyCode.stringValue = [NSString stringWithFormat:@"0x%02x (%s)", keyCode, keyName];
}
- (void)keyDown:(NSEvent *)event {
    int keyCode = [event keyCode];
    [self showKey:keyCode];
}
- (void)flagsChanged:(NSEvent *)event {
    unsigned keyCode = [event keyCode];
    [self showKey:keyCode];
}
- (BOOL)acceptsFirstResponder {
    return YES;
}
@end

int main(int argc, const char *argv[]) {
    return NSApplicationMain(argc, argv);
}
