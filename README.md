# Modular Stat & Progression Framework

A clean, engine-agnostic stat and progression system implemented in C (with a legacy TypeScript reference). Designed to be easy to integrate into RPGs or other games requiring data-driven stats.

## Features

-   **Core Stats**: Flexible `Stat` structure managing base values and final calculated values.
-   **Modifier System**: Supports both **Additive** (e.g., +10 Strength) and **Multiplicative** (e.g., x1.5 Damage) modifiers.
-   **Buffs & Debuffs**: Temporary effects with duration timers that automatically expire.
-   **Progression**: Experience (XP) system with leveling curves and stat growth on level up.
-   **Autotick**: Built-in game loop simulation to advance time automatically for testing.
-   **Clean UI**: Console-based interface with automatic terminal clearing for a smooth experience.

## Setup & Usage

### Prerequisites
-   GCC Compiler (or any standard C compiler)
-   Windows (for `conio.h` and `windows.h` dependencies used in the demo)

### Installation

1.  **Clone the repository**:
    ```bash
    git clone https://github.com/Vehzx/ProgressionFramework.git
    cd ProgressionFramework
    ```

2.  **Compile the C implementation**:
    ```bash
    gcc main.c -o stat_framework
    ```

3.  **Run the demo**:
    ```bash
    ./stat_framework
    ```

## Controls

-   **1-7**: Interact with the menu (Add XP, Apply Buffs, etc.)
-   **8**: Toggle **Autotick** (advances time by 1s every second)
-   **0**: Exit
