# FreeAssetPlan

## Scope
Milestone 1 only (simulation-first, top-down readability, local/offline runtime).

Approved source pool used in this plan:
- Kenney
- Quaternius
- Poly Haven
- Fab free listings with clear reusable license

## Asset buckets

## A) Free sourced assets ready to use (minimal cleanup)

| Asset Need | In-Game Purpose | Style | Source | URL | License | Format | Type | Nanite | UE5 Scale Notes | Collision | Material/Texture Notes | Animation/Rigging | Cleanup | Content Path | Replaces |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| Modular floor tiles (flat, clean) | Zone floors and lane readability | Stylized low-poly | Kenney (Prototype Kit) | https://kenney.nl/assets/prototype-kit | CC0 1.0 | FBX/OBJ/PNG | Static modular kit | No | 1m tile -> 100cm | Simple box | Flat color materials per zone | N/A | Create material instances by zone color | `Content/NeuralColony/Meshes/Environment/Tiles` | `AColonyZoneVisualActor` floor plane |
| Basic wall panels | Architect Core / workshop boundaries | Stylized low-poly | Kenney (Prototype Kit) | https://kenney.nl/assets/prototype-kit | CC0 1.0 | FBX/OBJ | Static modular kit | No | 2m wall -> 200cm | Box | Single-color readable palette | N/A | Pivot check + grid snap to 50cm | `Content/NeuralColony/Meshes/Environment/Walls` | Current implicit zone borders |
| Workshop props (crates, tables) | Build Workshop readability | Stylized low-poly | Kenney (Furniture Kit) | https://kenney.nl/assets/furniture-kit | CC0 1.0 | FBX/PNG | Static mesh set | No | Normalize tabletop to ~90cm height | Simple convex | Keep low saturation; reserve bright colors for states | N/A | Remove unused high-clutter pieces | `Content/NeuralColony/Meshes/Props/Workshop` | Empty workshop visuals |
| Sci-fi console set | Verification/Repair station visuals | Stylized sci-fi low-poly | Quaternius (Sci-Fi Essentials Kit) | https://quaternius.com/packs/scifiessentialskit.html | CC0 | FBX/Blend/Textures | Static mesh set | No | Consoles ~110-140cm tall | Box/convex | Emissive accents only on interactables | N/A | Re-export with uniform pivots | `Content/NeuralColony/Meshes/Props/Systems` | Basic cube markers |
| Low-poly humanoid robots | Colony member visuals (roles/states) | Stylized readable silhouette | Quaternius (Robot Characters) | https://quaternius.com/packs/robotcharacters.html | CC0 | FBX/Blend | Skeletal mesh | No | 160-180cm adults, 90cm baby variant | Capsule via character BP | Role color bands via material instance | Yes (basic walk/idle set) | Retarget to UE5 mannequin rig if needed | `Content/NeuralColony/Meshes/Characters/Nodes` | `AColonyNodeVisualActor` sphere body |
| Ground asphalt/concrete material | Base map readability and lane contrast | Clean non-photoreal (desaturated) | Poly Haven (Textures) | https://polyhaven.com/textures | CC0 | JPG/PNG | Material texture set | No | World-aligned tiling at 200-400cm | N/A | Use roughness/normal lightly to avoid noise | N/A | Create simplified master material | `Content/NeuralColony/Materials/Environment` | Current flat plane tint |
| Warning stripe decals | Work/repair hazard readability | Stylized utility markings | Kenney (Roads/Signs pack) | https://kenney.nl/assets/road-kit | CC0 1.0 | PNG/FBX | Decal/plane mesh | No | 100-300cm strip lengths | No collision | High contrast yellow/black | N/A | Convert textures to UI2D/no mip if needed | `Content/NeuralColony/Materials/Decals` | Missing lane emphasis |
| Generic icon planes (role markers) | Top-down role readability above nodes | Clean iconographic | Kenney (UI Pack) | https://kenney.nl/assets/ui-pack | CC0 1.0 | PNG | Sprite/plane | No | 30-50cm icon planes above head | No collision | Unlit for readability | N/A | Build icon material + billboard behavior | `Content/NeuralColony/UI/WorldIcons` | Text-only role readability |

## B) Free sourced assets that need cleanup/adaptation

| Asset Need | In-Game Purpose | Style | Source | URL | License | Format | Type | Nanite | UE5 Scale Notes | Collision | Material/Texture Notes | Animation/Rigging | Cleanup Needed | Content Path | Replaces |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| Modular sci-fi room kit | Architect/Nursery/Workshop room shells | Low-poly sci-fi | Quaternius (Ultimate Modular Sci-Fi) | https://quaternius.com/packs/ultimatemodularscifi.html | CC0 | FBX/Blend | Static modular kit | No | Re-scale modules to 400cm bay widths | Custom simple per module | Reduce texture variety for top-down clarity | N/A | Trim variant count, unify materials, fix pivots | `Content/NeuralColony/Meshes/Environment/Rooms` | Temporary zone floors only |
| Free Fab stylized factory props | Verification/repair machinery hero props | Stylized industrial | Fab Free Listing (curated) | https://www.fab.com/ | Fab Standard License (verify per listing) | FBX/uasset | Static set | Usually No | Validate import scale individually | Auto + manual simplify | Ensure consistent roughness and saturation | N/A | License review + rename to naming standard | `Content/NeuralColony/Meshes/Props/Factory` | Minimal machinery variation |
| Fab free VFX pack (non-Niagara heavy) | Communication pulse / state indicators | Stylized minimal FX | Fab Free Listing (curated) | https://www.fab.com/ | Fab Standard License (verify per listing) | Niagara/materials | FX assets | No | Keep FX radius <= 80cm around node | No collision | Unlit, additive with low intensity | Optional flipbook | Strip heavy emitters for performance/readability | `Content/NeuralColony/FX/Communication` | Text-only speech markers |
| Poly Haven HDRI (optional editor look-dev only) | Better neutral lighting while iterating | Neutral/studio | Poly Haven | https://polyhaven.com/hdris | CC0 | HDR | Lighting asset | N/A | Use only for dev profile | N/A | Keep gameplay PP independent | N/A | Create optional lighting map profile | `Content/NeuralColony/Lighting/DevOnly` | Default harsh placeholder lighting |

## C) Custom-needed assets (no strong free match)

| Asset Need | In-Game Purpose | Visual Brief | Why Custom Needed | Target Format | Content Path | Replaces |
|---|---|---|---|---|---|---|
| Baby-node growth stage mesh set (3 stages) | Make birth->juvenile->worker progression obvious | Rounded, readable silhouettes; shared rig; color-band sockets | Free packs rarely include staged lifecycle variants aligned to simulation states | Skeletal mesh + 3 meshes or morph targets | `Content/NeuralColony/Meshes/Characters/NodeLifecycle` | Single-size node placeholder |
| Build-site modular progress kit (foundation/frame/validation) | Project progress as physical construction phases | Three clean modules that stack visually with obvious progression | Existing free kits do not map directly to simulation stages without heavy kitbashing | Static modular set | `Content/NeuralColony/Meshes/ProjectSites` | Single build mass cube |
| Conversation bubble marker mesh + arrow beam | Clear source->target message passing | Cartoon speech bubble + subtle directional connector | Free UI packs offer icons, but not world-space 3D linked pair behavior | Static mesh + material | `Content/NeuralColony/Meshes/Communication` | Text-only bubble render |
| Family-lineage pedestal prop | Reinforce ancestry in world near nursery | Simple node-tree monument with emissive lines | Niche theme-specific prop unlikely in free libraries | Static mesh | `Content/NeuralColony/Meshes/Props/Nursery` | No ancestry world prop |

## Source policy and licensing guardrails
1. Prefer CC0 assets (Kenney, Quaternius, Poly Haven) first.
2. Use Fab only when listing explicitly grants free use under clear license terms.
3. Keep a per-asset attribution/license record in `Docs/AssetGapReport.md` and import manifest.
4. No runtime downloading; all assets are imported into project content locally.
