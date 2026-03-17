*This project has been created as part of the 42 curriculum by gostroum...*
# Philosophers

## Description
This project implements the classic Dining Philosophers concurrency problem.
The goal is to synchronize multiple philosopher threads so they can think, eat,
and sleep without starving or causing deadlocks, while respecting timing rules.

## Instructions
Build:
```bash
cd philo
make
```

Run:
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

Example:
```bash
./philo 5 800 200 200
```

## Resources
- The Dining Philosophers Problem (Wiki)
- pthreads documentation
- 42 School "philosophers" subject PDF
- Github (various approaches, tests)
- Leetcode (another approach)

## Tools
- Valgrind - nice tool for debug (helpfull options: --leak-check=full --fair-sched=yes --tool=helgrind)

## AI usage:
- Copilot autocomplete - (quick code rewriting)
- Codex 
  - context analysis, issues bullets list, (toying with fix all, but not using, just dump in ai slop branch for comparison)
  - Drafting a project Makefile
  - Updating README