# Modular Stat & Progression Framework

A standalone stat and progression system implemented in C.

## Features

-   **Core Stats**: Flexible `Stat` structure managing base values and final calculated values.
-   **Modifier System**: Supports both **Additive** (e.g., +10 Strength) and **Multiplicative** (e.g., x1.5 Damage) modifiers.
-   **Buffs & Debuffs**: Temporary effects with duration timers that automatically expire.
-   **Progression**: XP system with leveling curves and stat growth on level up.
-   **Autotick**: Built-in game loop simulation to advance time automatically (great for testing!).
-   **Clean UI**: Console interface with automatic terminal clearing.

## Setup & Usage

### Prerequisites
-   GCC Compiler (or any C compiler)

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