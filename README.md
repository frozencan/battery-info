#  Battery Info (Linux / Qt6)

A lightweight, minimal, and modern battery monitor for Linux built with **Qt 6 (C++ & QML)**.

Unlike traditional battery monitors that poll `/sys/class/power_supply` or UPower on periodic timers (causing CPU wakeups that drain the battery), **Battery Info is 100% event-driven**:
- **Zero Interval Polling**: No `QTimer` or polling loops.
- **Sleeps in `epoll_wait`**: Uses `QDBusConnection::systemBus()` to react to `org.freedesktop.DBus.Properties.PropertiesChanged` signals directly from the UPower daemon.
- **Instantaneous Updates**: The UI updates the exact millisecond an AC adapter is plugged or unplugged, or when the ACPI controller reports a battery status change.
- **Multi-Device Support**: Automatically enumerates primary laptop batteries (`BAT0`, `BAT1`) and peripheral batteries (mouse, keyboard, headset), dynamically reacting to device attachment/removal via `DeviceAdded` and `DeviceRemoved`.

---

##  Features

- **Hero Battery Gauge**: Smooth circular battery gauge, animated charging glow, instantaneous wattage power rate (`+W` or `-W`), and time-remaining estimation.
- **Capacity & Health**: Current capacity, factory design capacity, and battery health percentage (`CapacityHealth %`).
- **Electrical Metrics**: Real-time voltage, minimum design voltage, and power rate.
- **Hardware Specs**: Battery model, vendor, serial number, chemistry/technology (Li-ion, Li-poly, etc.), and hardware cycle count.
- **Event-Driven Proof & Activity Ticker**: Live real-time footer displaying exact D-Bus signal events, timestamps, and total event count.
- **Quick Export**: One-click "Copy Specs" button to export a formatted markdown diagnostic summary to the clipboard.
- **CLI Mode**: Run `battery-info --cli` in the terminal for instant headless battery diagnostics.

---

##  Requirements

- Linux with `UPower` service (`systemd` / standard freedesktop stack)
- Qt 6 (Core, Gui, Qml, Quick, QuickControls2, DBus)
- CMake 3.20+
- C++17 compatible compiler (`g++` or `clang++`)
- Ninja or Make

---

##  Building & Running

### 1. Build
```bash
cmake -B build -G Ninja
cmake --build build
```

### 2. Run GUI
```bash
./build/battery-info
```

### 3. Run CLI / Diagnostics
```bash
./build/battery-info --cli
```

### 4. Install (Optional)
```bash
sudo cmake --install build
```
