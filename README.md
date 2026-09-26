# Assignment 1 - Data Structures and Algorithms

**Student:** Cristian Alexandru Catalin
**Group:** 312CC

---

## Assignment Description

The goal is to build a system that manages emergency calls (112). Each incident has an id, a priority, a description, and a status, and each unit has an id, a type, and an availability status. When a unit gets sent to an incident, the two are linked together into an intervention, which keeps its own status too.

The system consists of:
- intervention units and incidents
- interventions, which link a unit to an incident
- waiting queues, used to keep the incidents and the units in order
- a stack, used to keep the history of the interventions

The following operations are required:

| # | Operation |
|---|-----------|
| 1 | `ADD_INCIDENT` |
| 2 | `CHECK_UNITS` |
| 3 | `DISPATCH` |
| 4 | `UNDO_LAST_DISPATCH` |
| 5 | `SOLVED_INCIDENT` |
| 6 | `SHOW_UNIT` |
| 7 | `SHOW_INCIDENT` |
| 8 | `SHOW_INTERVENTION` |

---

## Files

| File | Description |
|------|-------------|
| `library.h` | Structure definitions and function declarations |
| `init_system.c` | Sets up the system |
| `incidents.c` | Functions for adding incidents |
| `check_availability.c` | Checks the availability of incidents and units |
| `queues_stack.c` | Functions for the stacks and queues |
| `tema1.c` | Main function, with the rest of the required operations |

---

## Structures

### `struct unit`
Represents an intervention unit.
Contains: `id`, `type`, and `availability` (whether it's free or not).

### `struct incident`
Represents an incident.
Contains: `id`, `priority`, `description`, `status` (its state: queued / intervened / solved), and `prev`/`next` pointers, since it's part of a circular doubly linked list.

### `struct intervention`
Represents an intervention.
Contains: a pointer to the `incident`, a pointer to the `unit`, and `prev`/`next` pointers.

### `node_unit` / `qlist_u`
`node_unit` is one node of the unit queue: a pointer to a unit, and a pointer to the next node.
`qlist_u` is the queue itself: just a `head` and a `tail` pointer.

### `node_incident` / `qlist_i`
`node_incident` is one node of the incident queue: a pointer to an incident, and a pointer to the next node.
`qlist_i` is the queue itself: just a `head` and a `tail` pointer.

### `node_stack` / `stack`
`node_stack` is one node of the history stack: a pointer to an intervention, and a pointer to the next node.
`stack` is the stack itself: just a `head` pointer (interventions are pushed and popped from here).

### `struct system`
Holds the whole system together.
Contains: `nr_unitati` (number of units) and the `units` array, the head pointers of the circular lists (`incidents`, `interventions`), the unit queue (`queue_available_units`), the 3 incident queues by priority (`queue_low`, `queue_medium`, `queue_high`), and the `history_stack`.

---

## Functions

### `struct system *init_sistem()`
- Reads the number and the data of the units from the file.
- Sets up:
  - the 4 waiting queues (3 for incidents, 1 for units).
  - the stack (history of interventions).
  - the circular doubly linked lists (incidents, interventions).

---

### `void push_incident_list()`
- Adds an incident at the end of the circular list.

### `void add_incident()`
- Reads an incident from the file and adds it to the list, and also to the right waiting queue, based on its `priority`.

---

### `int check_units_availability()`
- Counts how many units are free in `queue_available_units`.
  - `option == 0` just prints the count.
  - `option == 1` returns the count without printing (used inside `dispatch`).

### `int check_incident_availability()`
- Returns how many incidents are waiting in the incident queues.

---

### `qlist_u *init_unit_list()`
- Sets up the queue of units.

### `void push_units_queue()`
- Adds a unit at the end of the queue.

### `void pop_units_queue()`
- Removes the first node from the unit queue (does not return it).

### `qlist_i *init_incident_list()`
- Sets up the queue of incidents.

### `void push_incidents_queue()`
- Adds an incident at the end of the queue.

### `void push_incidents_queue_first()`
- Adds an incident at the start of the queue (used in `undo_last_dispatch`).

### `void pop_incidents_queue()`
- Removes the first node from the incident queue (does not return it).

### `stack *init_stack()`
- Sets up the stack, where the history of interventions is kept.

### `void push_intervention_stack()`
- Adds an intervention on top of the stack.

### `void pop_intervention_stack()`
- Removes the first node from the stack.

---

### `void push_intervention_list()`
- Creates a new intervention, links it to a unit and an incident, then adds it to the circular doubly linked list.
- Also adds the intervention to the history stack.

### `struct incident *extract_incident()`
- Returns the first incident from the queue.

### `struct unit *extract_unit()`
- Returns the first unit from the queue.

### `void dispatch()`
- If there are free units, takes the incident with the highest priority from the 3 waiting queues, and creates a new intervention that links a free unit (now marked unavailable) with that incident (whose status becomes "intervened").

### `void undo_last_dispatch()`
- Goes through the history stack from the top, looking for the first intervention whose incident has status "intervened" (interventions already "solved" get removed along the way).
- Sets the incident's status back to "queued" and puts it back in its priority's waiting queue; the unit is set back to "available" and put back in `queue_available_units`.
- Removes the intervention from the list.

### `void solved_incident()`
- Reads an id from the file and looks for the intervention whose incident has that id.
- If the incident is not "solved" yet, marks it as solved.
- Sets the unit back to available and puts it back in `queue_available_units`.

### `void show_unit()`
- Reads an id from the file and looks for the unit with that id in the unit array.
- Prints details like id, type, and availability.

### `void show_incident()`
- Reads an id from the file and looks for the incident with that id in the circular list.
- Prints details like id, priority, description, and status.

### `void show_intervention()`
- Goes through the circular list of interventions and prints their information.
- Prints details like intervention id, unit id, and status.

### `void free_all()`
- Frees all the parts of the system.
