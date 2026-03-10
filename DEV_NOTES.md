# DEV_NOTES

## Milestone 1 presentation layer update
This iteration builds the playable Unreal-side presentation layer on top of the existing local `UColonySimulationSubsystem` mock runtime.

Everything remains fully local/offline with no backend integration, no cloud dependency, and no API keys.

## Added gameplay/presentation classes
- `AColonyGameMode`
  - Sets `AColonyCameraPawn` and `AColonyPlayerController` as defaults.
  - Spawns `AColonyPresentationManager` at runtime.
- `AColonyCameraPawn`
  - Top-down camera with pan (WASD) and zoom (mouse wheel).
- `AColonyPlayerController`
  - Enables mouse cursor + click handling and forwards node selection to HUD inspector state.
- `AColonyPresentationManager`
  - Bridge between subsystem data and world/UI.
  - Spawns simple ground plane, node visual actors, and project site actor.
  - Keeps visuals synchronized with subsystem data every tick.
  - Handles node selection and visible conversation bubble updates.
- `AColonyNodeVisualActor`
  - World representation of nodes with text labels and conversation bubble text.
  - Visual state coloring for baby, idle, running, complete, and error.
- `AColonyProjectSiteActor`
  - Build-site placeholder with a world progress bar + project label/stage/progress.
- `UColonyHUDWidget`
  - Programmatic UMG layout with:
    - left panel (My Goal, Neural Network Goal, time controls)
    - top status bar
    - right persistent inspector
    - visible alert/event feed
    - project status summary
  - Wired directly to `UColonySimulationSubsystem` data and mutators.

## Config/build changes
- `NeuralColony.Build.cs`
  - Adds UMG/Slate dependencies for runtime widget support.
- `Config/DefaultInput.ini`
  - Adds axis mappings for camera movement/zoom:
    - `MoveForward` (W/S)
    - `MoveRight` (A/D)
    - `Zoom` (MouseWheelAxis)
- `Config/DefaultEngine.ini`
  - Sets `GlobalDefaultGameMode=/Script/NeuralColony.ColonyGameMode`.

## How the Milestone 1 scene now behaves
1. Launch game/PIE.
2. Top-down camera is immediately controllable.
3. Colony nodes appear as simple placeholder actors.
4. Node labels and bubble text update from simulation state/activity.
5. Project site shows visible progress/stage.
6. Alerts feed updates from subsystem events.
7. Selecting a node updates persistent inspector details.
8. Goal inputs edit and persist through subsystem SaveGame.
9. Time controls pause / 1x / 2x / 3x change simulation speed.

## Extension points for Milestone 2
- Replace placeholder meshes with authored assets while preserving current data bindings.
- Add world-space speech bubble widgets with timed fade behavior.
- Add explicit node movement/meeting logic tied to communication events.
- Expand inspector with parent lineage traversal view.
- Move from polling HUD refresh to delegate-driven UI diff updates.
