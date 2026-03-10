# SYSTEM SIMULATION SPEC

## Purpose
Define the mock colony behavior for Milestone 1.

## Node lifecycle
1. born
2. juvenile
3. assigned simple work
4. specialized
5. active contributor
6. possible mentor / parent later

## Communication
When nodes pass information:
- move toward each other or visibly interact
- display short readable dialogue
- update downstream task state

## Job flow
Player sets goal
-> architect interprets goal
-> jobs created
-> nodes assigned
-> construction / work progresses
-> verification may fail
-> repair nodes may respond
-> project completes

## Specialization examples
- architect
- planner
- builder
- verifier
- repairer
- researcher
- ui specialist
- backend specialist

## Failure examples
- stalled project
- invalid build
- overload
- conflict
- broken structure

## Visual rules
All important logic should be visible through:
- movement
- speech
- progress bars
- alerts
- inspector panels
