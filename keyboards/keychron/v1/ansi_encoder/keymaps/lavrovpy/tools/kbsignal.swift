// kbsignal: light up the Keychron V1 from the Mac.
//   kbsignal done|attention|error|clear   play a lighting signal
//   kbsignal flags                        show the Polish and Ukrainian flags for 20 s
//   kbsignal wb GREEN BLUE                set the white balance gains (0-255) until unplugged
//   kbsignal debounce                     print the debounce setting stored on the board
// Exits 0 when the keyboard is absent, so hooks that call it never fail.
// Build: swiftc -O kbsignal.swift -o ~/.local/bin/kbsignal

import Foundation
import IOKit.hid

let signals: [String: UInt8] = ["done": 1, "attention": 2, "error": 3, "clear": 4, "wb": 5, "flags": 6]
let debounceNames = ["defer global", "defer per row", "defer per key", "eager per row",
                     "eager per key", "eager press / deferred release per key", "none"]
let usage = "usage: kbsignal done|attention|error|clear|flags|debounce, or kbsignal wb GREEN BLUE\n"

let args = Array(CommandLine.arguments.dropFirst())
guard let command = args.first, command == "debounce" || signals[command] != nil else {
    FileHandle.standardError.write(usage.data(using: .utf8)!)
    exit(2)
}
var gains: [UInt8] = []
if command == "wb" {
    gains = args.dropFirst().compactMap { UInt8($0) }
    guard gains.count == 2 else {
        FileHandle.standardError.write(usage.data(using: .utf8)!)
        exit(2)
    }
}

// The raw HID interface QMK exposes for VIA; Karabiner only seizes the keyboard interface.
let manager = IOHIDManagerCreate(kCFAllocatorDefault, IOOptionBits(kIOHIDOptionsTypeNone))
let matching: [String: Any] = [
    kIOHIDVendorIDKey: 0x3434, kIOHIDProductIDKey: 0x0311,
    kIOHIDPrimaryUsagePageKey: 0xFF60, kIOHIDPrimaryUsageKey: 0x61,
]
IOHIDManagerSetDeviceMatching(manager, matching as CFDictionary)
IOHIDManagerOpen(manager, IOOptionBits(kIOHIDOptionsTypeNone))
guard let device = (IOHIDManagerCopyDevices(manager) as? Set<IOHIDDevice>)?.first else {
    FileHandle.standardError.write("kbsignal: Keychron V1 not connected\n".data(using: .utf8)!)
    exit(0)
}

var reply: [UInt8]? = nil
let inputBuffer = UnsafeMutablePointer<UInt8>.allocate(capacity: 32)
IOHIDDeviceRegisterInputReportCallback(device, inputBuffer, 32, { _, _, _, _, _, report, length in
    reply = Array(UnsafeBufferPointer(start: report, count: length))
}, nil)
IOHIDDeviceScheduleWithRunLoop(device, CFRunLoopGetCurrent(), CFRunLoopMode.defaultMode.rawValue)

var packet = [UInt8](repeating: 0, count: 32)
if command == "debounce" {
    packet[0] = 0xA7 // Keychron misc command group
    packet[1] = 0x05 // debounce get
} else {
    packet[0] = 0x07 // VIA custom set value
    packet[1] = 0x00 // custom channel
    packet[2] = 0x4C // signal magic, see keymap.c
    packet[3] = signals[command]!
    for (i, gain) in gains.enumerated() { packet[4 + i] = gain }
}
let status = IOHIDDeviceSetReport(device, kIOHIDReportTypeOutput, 0, packet, packet.count)
guard status == kIOReturnSuccess else {
    FileHandle.standardError.write("kbsignal: write failed (\(status))\n".data(using: .utf8)!)
    exit(1)
}

let deadline = Date().addingTimeInterval(1)
while reply == nil && Date() < deadline {
    CFRunLoopRunInMode(.defaultMode, 0.05, true)
}

if command == "debounce" {
    guard let r = reply, r.count > 5, r[0] == 0xA7 else {
        print("no reply: firmware without Keychron debounce support")
        exit(1)
    }
    let type = Int(r[4])
    print("\(type < debounceNames.count ? debounceNames[type] : "type \(type)"), \(r[5]) ms")
}
