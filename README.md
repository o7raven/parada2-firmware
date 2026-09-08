# PARADA2 Firmware

Firmware for a **CubeSat-oriented embedded system prototype**, developed by **Jakub Farník** as part of a **Komplexní profilová práce (KPP) for the Czech *maturita***.

The repository contains the firmware-side design and implementation for a Raspberry Pi Pico based controller. The code is organized around a small system state machine, hardware abstraction layers for common buses, sensor handling, communication initialization, logging, and a watchdog-driven main loop.

> **Project status:** This repository is a development/prototype project. Several parts of the intended CubeSat functionality are currently represented by interfaces, stubs, or TODOs rather than a finished flight-ready implementation.

## Project goals

The firmware is structured around a simple fault-tolerant operating concept:

- **CHECK** – verify the current system context before entering normal operation.
- **RUN** – perform normal sensing and system activity.
- **SAFE** – continue with reduced functionality when power, sensor, or communication conditions are not considered healthy.
- **ERROR** – terminal/recovery-oriented handling for critical faults.

The state machine evaluates values such as sensor, power, and radio health, along with low-battery and critical-fault conditions. A watchdog is updated from the main system loop to provide an additional recovery mechanism.

## Architecture

The source tree is divided into a few focused layers:

```text
src/
├── communication/   Communication initialization and logging
├── config/          Hardware and project configuration
├── drivers/         UART, I2C, and SPI hardware abstractions
├── misc/            Small utility modules
├── sensors/         Sensor interfaces and sensor implementations
└── system/          System lifecycle, context, statuses, and state machine
```

Additional project material is stored in:

```text
docs/                Development notes and TODOs
logic_diagrams/      Logic/state-machine diagrams
source tests/        Native unit tests
third_party/unity/   Bundled Unity C test framework
```

The firmware entry point is `src/main.c`. It initializes the system and then enters the system run loop. The system initialization currently brings up logging, the blink utility, communication interfaces, sensors, the state machine, and the watchdog before starting normal execution.

## State machine

The central state machine is implemented in `src/system/state_machine.c`.

At initialization, the machine starts in `STATE_CHECK`. It enters `STATE_RUN` only when the required health flags are good and the battery is not considered low. Otherwise it moves to `STATE_SAFE`.

The main transitions are:

```text
                  ┌───────────┐
                  │   CHECK   │
                  └─────┬─────┘
                        │
              healthy + battery OK
                        │
                        ▼
                  ┌───────────┐
         ┌───────▶│    RUN    │───────┐
         │        └─────┬─────┘       │
         │              │             │
  issues resolved       │ low battery │ critical fault
         │              ▼             ▼
         │        ┌───────────┐   ┌───────────┐
         └────────│   SAFE    │   │   ERROR   │
                  └─────┬─────┘   └───────────┘
                        │
                  critical fault
                        │
                        ▼
                   ┌─────────┐
                   │  ERROR  │
                   └─────────┘
```

The repository also contains the editable logic diagrams under [`logic_diagrams/`](logic_diagrams/).

## Hardware interfaces

The current firmware project uses the Raspberry Pi Pico SDK hardware APIs for:

- **UART** – serial communication / logging interface
- **I2C** – sensor and peripheral communication
- **SPI** – SPI peripherals
- **Hardware timer** – timekeeping support used by the system logic
- **Hardware watchdog** – watchdog supervision of the main loop

The corresponding project targets and libraries are declared in `CMakeLists.txt`. The current CMake configuration selects the **Raspberry Pi Pico** board and builds the firmware using C11/C++17 language settings.

## Sensors

A BME280 interface is present in `src/sensors/` with fields for:

- temperature
- pressure
- humidity

The repository currently contains the BME280 driver interface and the surrounding sensor abstraction, but the implementation is still marked as `STATUS_NOT_IMPLEMENTED`. The same applies to the general sensor read/health functions, so these should be treated as work in progress rather than production sensor support.

## Communication

`src/communication/communication.c` provides a single initialization path for the current hardware interfaces. It initializes the abstract UART, I2C, and SPI drivers and returns a dedicated status code when one of those initializations fails.

A lightweight logging module is also included and is used by system initialization and the state machine to report events and state changes.

## Build requirements

The project is based on the **Raspberry Pi Pico SDK** and CMake. The repository's `CMakeLists.txt` currently specifies:

- CMake minimum version: **3.13**
- C standard: **C11**
- C++ standard: **C++17**
- Raspberry Pi Pico SDK version: **2.3.0**
- picotool version: **2.3.0**
- configured board: **`pico`**

The Pico SDK import file is included in the repository and supports locating an installed SDK through `PICO_SDK_PATH`, or fetching the SDK through the supported CMake options.

You will also need a working ARM embedded toolchain suitable for the selected Pico SDK setup.

## Building the firmware

### Command line

Set `PICO_SDK_PATH` to your local Raspberry Pi Pico SDK checkout, then configure and build:

```bash
export PICO_SDK_PATH=/path/to/pico-sdk
cmake -S . -B build
cmake --build build
```

On Windows PowerShell, the environment variable can be set with:

```powershell
$env:PICO_SDK_PATH = "C:\path\to\pico-sdk"
cmake -S . -B build
cmake --build build
```

The `.gitignore` intentionally excludes the generated `build/` directory.

### Visual Studio Code

The project contains the configuration expected by the Raspberry Pi Pico VS Code workflow. The repository's CMake file was generated from the Pico SDK Visual Studio Code extension and keeps the related SDK configuration in the project file.

## Unit tests

The project includes native C unit tests for the state machine under `tests/unit/` and bundles the **Unity** test framework in `third_party/unity/`.

The test suite is intentionally separate from the Pico firmware build: it is compiled with the host `gcc` compiler using C11.

Run the tests with:

```bash
cd tests
make
make test
```

The current tests cover initialization and several `STATE_CHECK` / `STATE_RUN` transition cases, including simulated sensor, radio, and power failures.

## Current limitations

This repository is best understood as an educational firmware foundation rather than completed CubeSat flight software. The current source contains explicit unfinished areas, including:

- sensor reading and sensor-health logic
- BME280 configuration and data acquisition
- radio implementation
- full SAFE-mode behavior
- complete error recovery behavior
- system simulation with dummy data
- additional hardware drivers

These items are consistent with the project's development notes in [`docs/todo.md`](docs/todo.md).

## Development notes

The firmware favors small modules with explicit status returns. Hardware-specific code is kept behind simple interfaces such as `abstract_uart_init()`, `abstract_i2c_init()`, and `abstract_spi_init()`, while the system layer decides how those results affect overall operation.

The state machine itself is deliberately kept separate from the policy performed inside each system state: `state_machine_step()` determines transitions, while `system_run()` performs the work associated with the current state.

## Credits and third-party software

### Project author

**Jakub Farník** is the author of the PARADA2 firmware project documented by this repository. The source file header identifies the project as part of a KPP and gives a project creation date of **04-09-2026**.

### Raspberry Pi Pico SDK

This project uses the **Raspberry Pi Pico SDK** from Raspberry Pi Trading Ltd. The firmware links against the SDK components declared by the project, including:

- `pico_stdlib`
- `hardware_spi`
- `hardware_i2c`
- `hardware_timer`
- `hardware_watchdog`

The Pico SDK is an external dependency and is **not authored by this project**. The SDK is distributed under the **BSD-3-Clause** license.

Project: <https://github.com/raspberrypi/pico-sdk>

### Unity Test Framework

Unit tests use **Unity**, the C unit-testing framework maintained by ThrowTheSwitch. A copy of the framework is included under `third_party/unity/`.

The bundled Unity headers identify the project as **Unity 2.7.2** and include the upstream copyright and **MIT** license notice.

Project: <https://github.com/ThrowTheSwitch/Unity>

### Source references

The project also relies on the standard documentation and examples provided by the Raspberry Pi Pico SDK and the Unity project where appropriate. Any third-party source included directly in the repository should remain subject to its original copyright and license terms.

## Originality and plagiarism statement

This README was **written from scratch for this repository**. It was prepared from the publicly visible project structure, source files, build configuration, and development notes rather than copied from another project's README.

No attempt has been made to present third-party libraries, SDK code, or testing frameworks as original work. Their respective authors and licenses are identified in the credits above.

For an academic submission, the repository owner should still review this document against the school's citation and AI-use rules and add any institutional references required by the KPP/maturita guidelines.

## AI-assisted documentation disclosure

**This README was created with the assistance of an AI model (GPT-5.6 Luna) following instructions provided for this repository.**

The AI was used to inspect the repository structure and source configuration, summarize the implementation at a documentation level, organize the README, and identify/credit visible third-party dependencies. The repository's author remains responsible for verifying the technical accuracy of this documentation and for complying with any school or examination rules concerning AI-assisted work.

## License

No project-level `LICENSE` file is currently present in the repository. Until an explicit project license is added by the author, do not assume that the project source itself is released under an open-source license merely because it uses openly licensed dependencies.

Third-party components retain their own licenses, including the **BSD-3-Clause** license for the Raspberry Pi Pico SDK and the **MIT** license for Unity.

## Repository

<https://github.com/o7raven/parada2-firmware>
