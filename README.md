# Practical3
# 🏐 City Netball Championship - EventFlow

## Event Concept

The City Netball Championship is a multi-day competitive netball tournament held at the Tembisa Sports Complex. The tournament brings together netball teams from different regions and age divisions (U12, U14, U16, U18, and Open) to compete for championship titles.

The tournament features:
- Multiple age divisions with group-stage matches
- Knockout rounds (quarter-finals, semi-finals, and final)
- Simultaneous matches across multiple courts and zones
- Operational support units (medical teams, scoreboards, information desks, spectator gates)

The system uses the **Composite Pattern** to model the tournament hierarchy and the **Observer Pattern** for event notifications throughout the tournament.

## Team Members

| Name                 | Student Number |
|----------------------|----------------|
| Lisakhanya Tantane   | u25514424      |
| Lethukuthula Ngwenya | u24986535      |
| Tendai Jonasi        | u25010230      |

## Architecture Overview

### Composite Pattern Structure
EventComponent (Component)
├── EventGroup (Composite)
│ ├── Tournament
│ ├── TournamentDay
│ ├── Division
│ ├── Zone
│ └── Court
└── EventUnit (Leaf)
├── NetballMatch
├── TeamWarmUp
├── UmpireBriefing
├── ScoreBoard
├── MedicalTeam
├── SpectatorGate
└── InformationDesk

### Observer Pattern Structure
Subject (Abstract)
├── EventGroup (Concrete Subject)
├── EventUnit (Concrete Subject)
└── NetballMatch (Concrete Subject)

Observer (Abstract)
├── EventGroup (Concrete Observer)
├── ScoreBoard (Concrete Observer)
├── MedicalTeam (Concrete Observer)
├── SpectatorGate (Concrete Observer)
├── InformationDesk (Concrete Observer)
├── TeamWarmUp (Concrete Observer)
└── UmpireBriefing (Concrete Observer)


### Key Design Decisions

1.  **Non-owning Observer Pointers**: Subjects store observers as raw pointers but do not manage their lifetime. Observers must detach themselves before destruction to prevent dangling pointers.
2.  **Push Notification Model**: The system uses a push approach where Subjects send complete `Notice` objects to observers, providing all relevant event information at once. This simplifies observer implementation.
3.  **Ownership Hierarchy**: Composite objects own their children. When a parent is destroyed, it deletes all its children, maintaining a clear ownership chain.
4.  **Cascading Notifications**: `EventGroup`s can act as both Subjects and Observers, allowing notifications to propagate through the hierarchy.

## Building and Running
### Prerequisites

- C++ compiler with C++11 support (g++ )
- `make`
- Doxygen (for documentation generation)

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/u24986535-ship-it/Practicatl3.git
cd Practicatl3

# Build the project
make

# Run the program
./eventflow

# Clean build files
make clean

# Generate Doxygen documentation
doxygen Doxyfile

# View documentation (open in browser)
# On Linux/Mac:
xdg-open docs/html/index.html
# On Windows:
start docs/html/index.html

GitHub Workflow Reflection
Work Division
Our team of three divided the work as follows:

Lisakhanya Tantane (Tasks 1, 4, 8): Designed the Composite pattern hierarchy (EventComponent, EventGroup, EventUnit), implemented the tournament structure (Tournament, TournamentDay, Division, Zone, Court), and created the memory management strategy. Also set up the initial repository structure and build system, and integrated the final UML diagrams and design rationale.

Tendai Jonasi (Tasks 2, 5): Implemented the Observer pattern (Subject, Observer, Notice), created concrete event units (NetballMatch, TeamWarmUp, UmpireBriefing), and developed the push notification model. Tendai also built the sample event hierarchy and object diagrams and was responsible for the sequence diagrams portfolio.

Lethukuthula Ngwenya (Tasks 3, 6, 7): Wrote comprehensive tests in main.cpp to validate the functionality of both patterns, created Doxygen documentation for all classes, and documented design decisions. Lethukuthula also helped with the README.md setup and coordinated the GitHub workflow.

Integration Process
Changes were integrated through a structured process that ensured code quality and team collaboration:

Feature Branches: Each member worked on their own feature branch:

feature/composite-pattern (Lisakhanya)

feature/observer-pattern (Tendai)

feature/testing-documentation (Lethukuthula)

Pull Requests: Each feature was merged through pull requests after:

Code review by at least one other team member.

Successful compilation and test execution.

Resolution of merge conflicts.

Main Branch Integration: The main branch was protected and required:

All tests to pass before merging.

No merge conflicts.

Code style consistency.

Example of Meaningful Collaboration
A key example of our team's collaboration is the integration of the Observer pattern with the existing Composite structure. Tendai implemented the Subject and Observer classes on his feature/observer-pattern branch (Commit: 87b4e75). When he opened a pull request to merge these changes, Lisakhanya reviewed the code and requested a change to ensure EventGroup could act as a concrete subject. This resulted in a collaborative commit (Commit: add3bd3) where we updated the EventGroup class to properly propagate notifications and manage its observer list, which was crucial for achieving the required cascading notifications.

Contact
Course: COS 214 - Programming and Algorithms

Repository: https://github.com/u24986535-ship-it/Practicatl3
