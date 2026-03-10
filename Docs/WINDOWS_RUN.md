# Windows Local Run Instructions (Milestone 1)

## Requirements
- Windows 10/11
- Unreal Engine 5.x installed locally
- Visual Studio 2022 with C++ game development tools

## First run
1. Open `NeuralColony.uproject` in Unreal Engine.
2. When prompted, allow project files/build files to generate.
3. Build the `Development Editor` target if Unreal asks to compile modules.
4. Press **Play In Editor**.

## What Milestone 1 simulation now provides
- local mock colony runtime (`UColonySimulationSubsystem`)
- node lifecycle states including baby nodes
- family-tree data fields on nodes
- visible alert feed events for conversations and progress updates
- project build progress and stage changes
- time controls (pause, 1x, 2x, 3x) exposed for UI binding
- editable goals persisted with local SaveGame

## How to wire into UMG quickly
- In any Widget Blueprint, get world subsystem: `Get World Subsystem -> ColonySimulationSubsystem`.
- Bind text widgets to:
  - `GetPlayerGoal`
  - `GetNeuralNetworkGoal`
- Bind list widgets to:
  - `GetNodes`
  - `GetProjects`
  - `GetAlerts`
- Hook button clicks to `SetTimeRate` for pause/1x/2x/3x.
- Hook editable text commit events to `SetPlayerGoal` and `SetNeuralNetworkGoal`.

## Offline guarantee
This milestone implementation is fully local simulation-only:
- no API keys
- no cloud services
- no internet requirement


## Camera controls
- Pan: `W/A/S/D`
- Zoom: mouse wheel
- Select node: left mouse click on a node actor
