# UML_Playground

## Finite State Machines

### Mealy Machine
1. Output is produced by `state machine` depends on **input events** fed to `state machine` && the `active state` of the `state machine`.
1. Output (also known as `input action`) is produced during `transition`; not inside the state itself.
1. Output is represented along each input separated by a `/` on transition arrow.

### Moore Machine
1. Output is determined ONLY by the `active state` of the `state machine` and NOT by any **input events**.
1. Output (also known as `entry action`) is produced inside the state itself; not during `transition`.
1. Output is represented inside state.

##  Implementation of State Machine
1. Nested Switch Approach.
1. State Table Approach.
1. State Handler Approach.


