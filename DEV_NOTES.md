# DEV_NOTES

## Milestone 1 presentation quality upgrade
This update reworks the previous debug-like presentation into a clearer colony-sim scene with purposeful spatial layout, readable zone hierarchy, and stronger simulation readability.

The runtime remains fully local and simulation-first:
- no cloud dependency
- no API keys
- no backend integration

## Major quality improvements

### 1) Intentional colony layout and zone hierarchy
The world now builds around explicit colony spaces instead of arbitrary row placement:
- Architect Core
- Nursery
- Build Workshop
- Verification area
- Repair/Debug area
- Dedicated build project site lane

`AColonyPresentationManager` now creates this structure at startup via `BuildColonyLayout`, and each zone is visualized by `AColonyZoneVisualActor` with readable labels, floor tinting, and boundary slabs.

### 2) Node visuals upgraded for state readability
`AColonyNodeVisualActor` now has:
- a body mesh + base ring for stronger silhouette
- improved color palette per lifecycle state
- per-state size differences (babies are visibly smaller)
- interpolated movement (not static teleport-only feel)
- timed speech bubble text behavior
- clearer selected highlight behavior

### 3) Purposeful movement and role-based destinations
Node placement is no longer arbitrary.
`AColonyPresentationManager::ResolveTargetLocationForNode` routes node targets by state/role, so actors travel through meaningful locations:
- babies remain in Nursery
- builders/planners move between workshop and project lane
- verifiers use verification zone
- repairers use repair zone
- architects operate from architect core
- complete/error states shift to corresponding finishing/repair positions

This creates a clearer “who is doing what” read from top-down.

### 4) Better project/build-site presentation
`AColonyProjectSiteActor` now represents build progress as:
- dedicated site pad
- expanding build mass (construction volume grows over time)
- horizontal progress bar with fill motion
- clearer stage/progress text hierarchy

This makes project progression feel like visible construction rather than a single abstract marker.

### 5) Communication readability improved
Conversation alerts now drive in-world readable behavior:
- source node bubble: “Passing instruction”
- destination node bubble: “Instruction received”

This provides clearer causal behavior for message passing in the colony.

### 6) HUD readability tweaks
`UColonyHUDWidget` top bar now surfaces stronger simulation telemetry:
- total nodes
- running nodes
- baby count
- error count
- alert count

Alert list formatting is also improved for quick scanning.

## Files added/updated for this upgrade
- Added:
  - `Source/NeuralColony/ColonyZoneVisualActor.h`
  - `Source/NeuralColony/ColonyZoneVisualActor.cpp`
- Reworked:
  - `Source/NeuralColony/ColonyPresentationManager.h/.cpp`
  - `Source/NeuralColony/ColonyNodeVisualActor.h/.cpp`
  - `Source/NeuralColony/ColonyProjectSiteActor.h/.cpp`
  - `Source/NeuralColony/ColonyHUDWidget.cpp`

## Next logical Milestone 1 polish (still local)
- add subtle lane decals between zones for stronger traffic readability
- add tiny role icons above nodes (architect/builder/verifier/repairer)
- add short-lived directional “message pulse” markers between speaker/listener nodes
- add family-tree quick view in inspector (parent names/generation lineage)
