# PROJECT BRIEF

## Project status
Start from scratch.

Do not assume previous experiments, prototypes, tools, or backend systems are usable as production foundations.

This is a new clean project.

## Primary goal
The **primary goal** of this project is the **demonstration layer**.

This game exists to help people intuitively understand how an autonomous AI system works.

The game should make it easy to understand:
- how nodes communicate
- how nodes specialize
- how tasks are delegated
- how projects are built
- how systems fail
- how systems repair themselves
- how new nodes are created and evolve

## Zero-budget rule
This project must work with:
- $0 budget
- no API keys
- no paid backend services
- no cloud dependency
- no required internet connection

If there is any conflict, choose the fully local simulation-first path.

## Core idea
Build a RimWorld-style top-down colony simulation / management game that represents a living AI system.

This is not meant to be a literal clone of RimWorld.
It should capture:
- top-down colony readability
- visible work assignment
- specialist roles
- family-tree growth
- alerts and logs
- progress and failure states
- player oversight

## Core translation layer
AI concepts should be mapped into things people naturally understand.

Examples:
- node / agent -> NPC / colony member
- node creation -> birth
- lineage -> family tree
- communication -> dialogue / speech bubbles / message passing
- project building -> construction / crafting / progress bars
- verification -> inspectors / testing stations
- repair loop -> healers / engineers / fixers
- specialization -> job class / profession

## Current intended system capabilities
The eventual real system is meant to support:
1. Goal-driven orchestration
2. Multiple node roles
3. Node spawning
4. Specialization growth
5. Task routing
6. Build / verify / repair loop
7. Project tracking
8. Action layer
9. Family tree / ancestry
10. Demonstration-first observability

## Important implementation rule
The first versions should be **simulation first**.

Do not require any real backend AI system for Milestone 1.

Build the demonstration layer so that real local functionality could be attached later, but is never required for the base experience.

## Required feel
The player should quickly understand:
- who is doing what
- why they are doing it
- who created whom
- what the colony is building
- what failed
- what is being repaired
- how the system is evolving

## First build target
A playable simulation-first vertical slice with:
- top-down map
- node NPCs
- baby-to-worker lifecycle
- family tree support
- visible conversations
- visible work/project construction
- completion progress
- alerts/logs
- inspector
- editable goal input
- time controls
- simulated colony logic only
