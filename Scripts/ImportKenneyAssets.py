"""
Run inside Unreal Editor Python console:
    exec(open(r"<Project>/Scripts/ImportKenneyAssets.py", "r", encoding="utf-8").read())

This script imports staged raw Kenney GLB files into /Game/NeuralColony/Art/* folders.
It is editor-only and does not add any runtime internet dependency.
"""

import os
import unreal

PROJECT_DIR = unreal.Paths.project_dir()
RAW_ROOT = os.path.join(PROJECT_DIR, "Content", "NeuralColony", "RawAssets", "Kenney")

IMPORTS = [
    # Environment / zones
    ("StarterKitCityBuilder/models/grass.glb", "/Game/NeuralColony/Art/Environment"),
    ("StarterKitCityBuilder/models/pavement.glb", "/Game/NeuralColony/Art/Zones"),
    ("StarterKitCityBuilder/models/pavement-fountain.glb", "/Game/NeuralColony/Art/Zones"),
    ("StarterKitCityBuilder/models/road-straight.glb", "/Game/NeuralColony/Art/Environment"),
    ("StarterKitCityBuilder/models/road-corner.glb", "/Game/NeuralColony/Art/Environment"),
    ("StarterKitCityBuilder/models/road-intersection.glb", "/Game/NeuralColony/Art/Environment"),
    # Props / workshop
    ("StarterKitCityBuilder/models/building-garage.glb", "/Game/NeuralColony/Art/Props"),
    ("StarterKitCityBuilder/models/building-small-a.glb", "/Game/NeuralColony/Art/Props"),
    ("StarterKitCityBuilder/models/building-small-b.glb", "/Game/NeuralColony/Art/Props"),
    ("StarterKitCityBuilder/models/building-small-c.glb", "/Game/NeuralColony/Art/Props"),
    ("StarterKitCityBuilder/models/building-small-d.glb", "/Game/NeuralColony/Art/Props"),
    # Character + station prop
    ("StarterKitBasicScene/models/character-soldier.glb", "/Game/NeuralColony/Art/Characters"),
    ("StarterKitBasicScene/models/weapon-rack.glb", "/Game/NeuralColony/Art/Props"),
]


# Import settings
ui = unreal.FbxImportUI()
ui.import_mesh = True
ui.import_as_skeletal = False
ui.import_materials = False
ui.import_textures = False
ui.static_mesh_import_data.auto_generate_collision = True
ui.static_mesh_import_data.generate_lightmap_u_vs = True
ui.static_mesh_import_data.import_uniform_scale = 1.0

asset_tools = unreal.AssetToolsHelpers.get_asset_tools()

imported = []
missing = []

for rel_path, dest_path in IMPORTS:
    source_file = os.path.join(RAW_ROOT, rel_path)
    if not os.path.exists(source_file):
        missing.append(source_file)
        continue

    task = unreal.AssetImportTask()
    task.automated = True
    task.destination_path = dest_path
    task.filename = source_file
    task.replace_existing = True
    task.save = True
    task.options = ui

    asset_tools.import_asset_tasks([task])
    imported.extend(task.imported_object_paths)

print("Imported assets:", len(imported))
for p in imported:
    print("  ", p)

if missing:
    print("Missing raw files:")
    for p in missing:
        print("  ", p)
