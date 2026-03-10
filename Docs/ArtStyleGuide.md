# ArtStyleGuide

## Style target for Milestone 1
Readable, stylized, top-down management-sim visuals.

Not photoreal. Not gritty. Not noisy.

## Core principles
1. **Readability first**
   - Player should identify role/state/zone within 1-2 seconds.
2. **Strong silhouettes**
   - Each role class should be distinguishable from above.
3. **Color hierarchy**
   - State color = strongest semantic signal.
   - Zone color = broad context signal.
   - Prop color = muted support signal.
4. **Low clutter**
   - Avoid dense prop spam that hides movement/task flow.
5. **Consistent scale**
   - Human/node scale and furniture scale must remain believable.

## Recommended palette behavior
- Base environment: desaturated cool neutrals.
- Role/state accents: saturated but limited.
- Error/repair: warm warning colors.
- Verification/completion: cooler blue/cyan tones.
- Nursery: warm yellow/orange highlights.

## Material rules
- Prefer flat or lightly shaded stylized materials.
- Avoid high-frequency normal detail that hurts top-down readability.
- Keep roughness broad and simple.
- Use emissive sparingly for important interactive points only.

## Zone visual language
- Architect Core: clean, ordered, cool accent.
- Nursery: softer, warm accent, rounded forms.
- Build Workshop: practical tools, medium clutter, active work feel.
- Verification: diagnostic/console look, precise layout.
- Repair/Debug: hazard markers and maintenance props.

## Character/node visual language
- Babies: smaller scale + rounded forms + warm tint.
- Active workers: medium scale + role icon + clear state color.
- Error state: high-contrast red indicator visible from zoomed-out view.

## UI-facing world readability props
- Role icon billboards above node heads.
- Zone signs readable from camera height.
- Optional lane markers from workshop to project site.
- Optional communication pulse markers for source->target exchanges.
