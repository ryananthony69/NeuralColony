# ImportChecklist

## 1) Before download
- [ ] Asset source is in approved list (Kenney / Quaternius / Poly Haven / Fab free with clear license).
- [ ] License recorded in `Docs/AssetGapReport.md` table.
- [ ] Asset is relevant to Milestone 1 readability goals.

## 2) Raw source intake (do not import directly into gameplay folders)
- [ ] Store raw downloads under `Content/NeuralColony/ArtSources/<SourceName>/<PackName>/Raw`.
- [ ] Keep original archive and readme/license text next to raw files.
- [ ] Do not rename original source files in Raw folder.

## 3) DCC cleanup (if needed)
- [ ] Unit scale set so 1m -> 100 Unreal units.
- [ ] Pivot points corrected (floor pieces at bottom center, wall pieces at base edge).
- [ ] Triangulation and normals verified.
- [ ] LODs generated if asset is heavy (generally minimal for low-poly packs).
- [ ] Material slots reduced and named consistently.

## 4) Unreal import settings (static meshes)
- [ ] Import Uniform Scale = 1.0 (or documented override).
- [ ] Generate Missing Collisions ON for props, OFF for decorative no-hit items.
- [ ] Lightmap UVs generated if static lighting variant is used.
- [ ] Nanite OFF by default for low-poly Milestone 1 props.
- [ ] Folder target follows `Docs/ContentFolderPlan.md`.

## 5) Unreal import settings (skeletal meshes)
- [ ] Skeleton assignment validated.
- [ ] Retarget base pose checked.
- [ ] Physics asset generated only if needed.
- [ ] Root motion disabled unless explicitly required.

## 6) Post-import validation
- [ ] Top-down readability check (Play In Editor from default camera).
- [ ] Silhouette and role distinction check at gameplay zoom levels.
- [ ] Collision sanity pass (no invisible blockers in lanes).
- [ ] Material palette pass (no noisy or photoreal outliers).
- [ ] Performance sanity pass (no unnecessary high-poly meshes).

## 7) Replacement workflow safety
- [ ] Replace placeholder mesh references in one system at a time.
- [ ] Keep old placeholder assets until replacement validated in PIE.
- [ ] Use stable naming convention (`SM_NC_*`, `SK_NC_*`, `MI_NC_*`).
- [ ] Update `Docs/AssetReplacementManifest.csv` for each swap.

## 8) Final compliance checks
- [ ] No runtime internet dependency added.
- [ ] No plugin/backend/API/cloud dependency added.
- [ ] Milestone scope unchanged.
