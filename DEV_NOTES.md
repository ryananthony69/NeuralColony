# DEV_NOTES

## Milestone 1 focus delivered
This update adds a fully local, mock colony simulation runtime in C++ for the Milestone 1 demonstration layer. It is intentionally simulation-first and does not rely on any online service, API key, or backend integration.

## Major files
- `Source/NeuralColony/ColonySimulationTypes.h`
  - Core enums and records for node, project, alert, and time-speed data.
  - Includes family tree fields (`ParentA`, `ParentB`, `Generation`) and lifecycle states (including `Baby`).
- `Source/NeuralColony/ColonyGoalSaveGame.h`
  - SaveGame object used to persist editable goal panel text locally.
- `Source/NeuralColony/ColonySimulationSubsystem.h`
  - Blueprint-facing world subsystem API for reading colony data and controlling simulation speed.
  - Delegates for UI refresh and new alerts.
- `Source/NeuralColony/ColonySimulationSubsystem.cpp`
  - Seed data, deterministic mock updates, visible conversation events, project progress, node state changes, and periodic baby-node spawning.

## Runtime behavior summary
- Starts in local mock mode with an architect, builder, and baby node.
- Simulates project build progress through stages until completion.
- Rotates node lifecycle states (`Baby -> Idle -> Running -> Complete`).
- Emits alert feed entries for key events and conversation-style information passing.
- Supports time control multipliers: pause, 1x, 2x, 3x.
- Persists player goals and neural-network goals using local SaveGame slot.

## Extension points for Milestone 2
- Add explicit job queue records and assignment policies in subsystem state.
- Split simulation loop into dedicated managers (`NodeLifecycle`, `JobRouter`, `ProjectPipeline`).
- Add richer failure and repair loops by introducing fail probabilities and role-based responses.
- Expose project-site world actors and conversation bubble widgets that subscribe to subsystem delegates.
- Add inspector selection binding by mapping selected actor IDs to `FNodeRecord::Id`.
