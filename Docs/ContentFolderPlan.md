# ContentFolderPlan

## Unreal content structure (proposed)

`/Game/NeuralColony`
- `ArtSources/` (raw source files, never referenced directly by gameplay)
  - `Kenney/`
  - `Quaternius/`
  - `PolyHaven/`
  - `Fab/`
- `Meshes/`
  - `Characters/`
    - `Nodes/`
    - `NodeLifecycle/`
  - `Environment/`
    - `Tiles/`
    - `Walls/`
    - `Rooms/`
  - `Props/`
    - `Workshop/`
    - `Systems/`
    - `Nursery/`
  - `ProjectSites/`
  - `Communication/`
- `Materials/`
  - `Master/`
  - `Instances/`
  - `Decals/`
- `Textures/`
- `Animations/`
- `FX/`
  - `Communication/`
- `UI/`
  - `WorldIcons/`
- `Blueprints/`
  - `Presentation/`
  - `Characters/`
- `Maps/`
- `Placeholders/`

## Naming conventions
- Static Mesh: `SM_NC_<Category>_<Name>`
- Skeletal Mesh: `SK_NC_<Category>_<Name>`
- Material Master: `M_NC_<Purpose>`
- Material Instance: `MI_NC_<Purpose>_<Variant>`
- Texture: `T_NC_<Purpose>_<Suffix>`
- Niagara: `NS_NC_<Purpose>`
- Blueprint: `BP_NC_<Purpose>`

## Placeholder-safe replacement rule
- Keep live gameplay references pointing to stable wrapper Blueprints/components.
- Swap underlying mesh/material inside wrapper, not direct code path where possible.
- Old placeholder assets move to `/Game/NeuralColony/Placeholders/Archive` after validation.

## License tracking location
- `Docs/AssetGapReport.md` table (source + license + imported path).
- Keep original license/readme files under each `ArtSources/<Source>/<Pack>/Raw` folder.
