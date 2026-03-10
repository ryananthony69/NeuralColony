# AssetGapReport

## Milestone 1 gap summary

### Already covered by current placeholders (functional)
- Zone visualization geometry
- Node visual actor
- Build-site visual actor
- Speech text indicators

### High-priority visual gaps
1. **Character readability gap**
   - Current node visuals are abstract primitives.
   - Need role-readable silhouettes and lifecycle variants (baby/adult).

2. **Project staging gap**
   - Project site still reads as geometric placeholder.
   - Need stage-specific construction pieces for Foundation/Frame/Validation.

3. **Zone identity gap**
   - Zones are labeled but not prop-authored enough to read at a glance.
   - Need curated room props per zone for top-down recognition.

4. **Communication clarity gap**
   - Bubble text exists but source-target linkage can be stronger.
   - Need optional world-space marker/arrow FX mesh.

5. **Ancestry visibility gap**
   - Family tree exists in data/inspector but not in world dressing.
   - Need nursery lineage prop/signage for faster conceptual understanding.

## Proposed sourcing outcomes

### Bucket A expected to close
- 60-70% of environment and prop readability needs.
- Ground/floor/wall/console/workshop identity needs.

### Bucket B expected to close
- 20-25% with cleanup effort (Fab pack filtering, modular normalization).

### Bucket C (custom)
- Remaining 10-15% for simulation-specific visuals:
  - lifecycle staged node meshes,
  - staged project modules,
  - message-link mesh,
  - lineage prop.

## Licensing/usage tracking table template

| Asset ID | Name | Source | URL | License | Local File | Imported Path | Notes |
|---|---|---|---|---|---|---|---|
| KEN_ENV_001 | Prototype Floor Tile | Kenney | https://kenney.nl/assets/prototype-kit | CC0 1.0 | `/ArtSources/Kenney/...` | `/Game/NeuralColony/Meshes/Environment/Tiles/...` | |

## Risk notes
- Fab licensing is per-listing; each selected item must be explicitly documented before import.
- Mixed art packs can harm readability if palettes are not unified; enforce `ArtStyleGuide.md`.
- Avoid photoreal material packs for Milestone 1 readability from top-down camera.
