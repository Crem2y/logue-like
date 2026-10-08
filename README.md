# Logue-like

A lightweight, turn-based roguelike inspired by the original **Rogue**.

Logue-like is an experimental roguelike built around **limited information and spatial awareness**. Instead of constantly displaying the player's position and nearby enemies, the game encourages players to navigate using exploration, memory, and contextual feedback.

The project is being developed as a portable game core, initially targeting PC terminals and a custom handheld console.

> **Development Status:** Early prototype. Core gameplay and map generation are under development.

<!--
## Screenshots

![Gameplay Screenshot](doc/screenshot.png)
-->

## Game Concept

The game uses a traditional roguelike dungeon structure with rooms, corridors, enemies, items, and stairs.

Its main distinction is how information is presented to the player.

### Normal Mode

The intended primary game mode.

- Explored terrain remains visible.
- The player's position is normally hidden.
- Enemy positions are hidden unless actively detected.
- Items are visible within the current field of view.
- The Search command provides temporary information about nearby surroundings.
- Players must infer their position and potential threats from previous observations and game events.

The goal is not precise map memorization, but making decisions with incomplete information.

### Insight Mode

A more conventional roguelike experience.

- The player's position is always visible.
- Enemies are displayed within the field of view.
- The same underlying game rules and dungeon generation are used.

### Debug Mode

Displays the entire map and all entities for development and testing.

## Gameplay

The game currently focuses on a small set of basic actions.

| Action | Key | Description |
|---|---|---|
| Move | W,A,S,D | Move in one of four cardinal directions |
| Attack | H | Attack in the current facing direction |
| Search | G | Inspect nearby surroundings |

Movement and combat are separate actions. Attempting to move into an occupied tile does not automatically attack its occupant.

### Dungeon Generation

- Procedurally generated rooms and corridors
- Seed-based map generation
- Multiple enemies and items per floor
- Stairs connecting dungeon floors

The generator follows a traditional room-and-corridor approach inspired by classic roguelikes.

## Architecture

The project separates game simulation, dungeon generation, and rendering into independent components.

```text
logue-like/
├── src/
├── include/
└── lib/
    ├── logue_core/
    ├── logue_generator/
    └── logue_renderer/
```

| Component | Responsibility |
|---|---|
| LogueCore | Game state, player actions, combat, and turn processing |
| LogueGenerator | Procedural dungeon generation and entity placement |
| LogueRenderer | Visibility rules and presentation |

The architecture is designed to keep the game logic independent of the target platform.

## Building

### Requirements

- CMake
- C++17-compatible compiler

<!-- 
## Roadmap

Planned features include:

- [ ] Complete Search mechanics
- [ ] Player and enemy combat system
- [ ] Item effects and interactions
- [ ] Dungeon progression
- [ ] Full-clear rewards
- [ ] Secret rooms
- [ ] Game events and contextual feedback
- [ ] Handheld console integration

The roadmap is subject to change as gameplay is tested and refined.
-->
## Inspiration

Logue-like is inspired by **Rogue** and other traditional roguelikes.

The game engine, dungeon generator, and gameplay systems are independently implemented without using the original Rogue source code.

While the project shares familiar roguelike conventions, its primary design focus is gameplay based on incomplete information rather than continuous visibility.

## License
- This project is licensed under the MIT License.  
- See [LICENSE](./LICENSE) for details.