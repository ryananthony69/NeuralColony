# TECH ARCHITECTURE

## Preferred stack
Unreal Engine, Windows-first.

## Core rule
This project must function entirely as a local simulation with no cloud services and no API keys.

## Recommended split
### Frontend / presentation
Unreal:
- top-down map
- UI panels
- selection / inspection
- alerts
- time controls
- simulation view

### Runtime
Phase 1:
- mock / simulated in-engine colony behavior

Phase 2:
- optional local-only backend bridge

## Core data model

### Node
- id
- role
- specialization
- status
- activity
- parents
- generation
- load
- health
- current task
- linked project
- last output summary

### Project
- id
- name
- status
- health
- current stage
- build progress
- failure reason
- assigned nodes

### Job
- id
- type
- priority
- assigned node
- project id
- status

## Runtime modes
- Mock Mode
- Optional Local Backend Mode

## Requirement
UI must work in mock mode first.
Architecture must allow local extension later.
No cloud dependency.
