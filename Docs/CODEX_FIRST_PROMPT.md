Read these files first:
- Docs/PROJECT_BRIEF.md
- Docs/MILESTONES.md
- Docs/UI_SPEC.md
- Docs/TECH_ARCHITECTURE.md
- Docs/SYSTEM_SIMULATION_SPEC.md
- AGENTS.md

Project rules:
- This project must work with $0 budget
- No API keys
- No cloud dependency
- No paid backend services
- No OpenAI API integration
- No required online services
- The game must function entirely as a local simulation

Primary goal:
Build a RimWorld-style top-down colony simulation game called NeuralColony that acts as a demonstration layer for how an autonomous AI system works.

Important:
The primary goal is the demonstration layer, not real backend AI execution.
The game should make AI concepts intuitive through simulation and visual metaphor.

Core system concepts to demonstrate:
- nodes as colony members / NPCs
- node birth as baby nodes
- family tree / ancestry
- information passing as visible conversations
- task delegation as work orders
- specialization growth over time
- visible project construction with progress
- failure states
- repair behavior
- colony expansion

Milestone scope:
Work only on Milestone 1.

Milestone 1 requirements:
- top-down camera with pan and zoom
- node NPC actors
- node lifecycle including baby nodes
- family tree support in data model
- visible conversations / message passing
- visible project build sites with completion progress
- left panel for My Goal / Neural Network Goal
- right panel persistent inspector
- top status bar
- event / alert feed
- time controls: pause, 1x, 2x, 3x
- simulated colony runtime only
- Windows-first local execution
- no backend integration yet

Implementation requirements:
- Use simulation-first architecture
- Do not require internet
- Do not implement cloud AI
- Do not implement API calls
- Build the colony logic using local game systems only
- Design the architecture so a local-only backend could be attached later, but do not require it for Milestone 1

Design intent:
This should feel like a colony sim people can watch and immediately understand.
The point is to show how AI systems behave using game language that people innately understand.

Examples of visual translation:
- AI node creation -> baby NPC birth
- information transfer -> dialogue bubbles / meetings
- model specialization -> professions/jobs
- project generation -> construction sites
- system failure -> visible breakdowns / alerts
- repair loop -> fixer / engineer behavior
- expanding system -> family tree and colony growth

Technical guidance:
- Prioritize readability and clarity
- Prioritize modular architecture
- Keep simulation data separate from presentation where possible
- Support future expansion, but do not build beyond Milestone 1
- Prefer simple, stable local implementations over ambitious unfinished systems

Deliverables:
- Milestone 1 implementation inside this Unreal project
- clear local run/build instructions
- DEV_NOTES.md summarizing architecture, major files, and extension points
- any setup notes needed for continuing Milestone 2 later

Acceptance criteria:
- app launches locally on Windows
- top-down camera works
- nodes visibly update state over time
- baby nodes can exist in the simulation
- conversations are visible
- projects/build sites visibly progress
- selection inspector works
- goal text is editable and persisted
- time controls affect simulation speed
- no internet or API keys required
