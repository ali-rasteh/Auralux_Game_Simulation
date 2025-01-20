# Auralux Game Simulation

## Overview

Auralux Game Simulation is a C++ project that simulates the strategic gameplay of Auralux. The simulation includes managing teams, colleges, and agents, and executing various commands to simulate gameplay.

## Features

- Simulate team movements and actions.
- Manage colleges by controlling their production and ownership.
- Execute commands to move agents, set destinations, and change product types.
- Display game state in a text-based format using ncurses and a simple graphic using Qt.

## Getting Started

### Prerequisites

- C++ compiler (e.g., g++)
- ncurses library
- Qt library (for graphical display)

### Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/ali-rasteh/Auralux_Game_Simulation.git
    ```

2. Navigate to the project directory:
    ```sh
    cd Auralux_Game_Simulation/Codes
    ```

3. Compile the program:
    ```sh
    g++ -o Auralux_Game_Simulation main.cpp -lncurses -lQt
    ```

4. Run the program:
    ```sh
    ./Auralux_Game_Simulation
    ```

## Usage

- The simulation reads commands from an input file (input.txt).
- Use commands such as move, go, and change to control the game entities.
- The simulation updates and displays the game state after each set of commands.

## Dependencies

- ncurses library
- Qt library

## Authors

- Ali Rasteh
- Masroor Bayati
