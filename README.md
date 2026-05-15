# Traffic Simulator

This repository contains a C-based discrete-event traffic simulator developed for the Telecommunications Networks course. The project progressively models Poisson arrivals, packet-oriented queueing systems, and a call-centre scenario, estimating performance metrics such as blocking probability, delay probability, average waiting time, loss probability, and prediction error.

## Overview

The work is divided into three parts:

### Part 1 — Poisson Arrival Process

Simulates a traffic source with Poisson arrivals using two approaches:

- Event-driven simulation, where the clock advances directly to the next arrival;
- Time-stepped simulation, where arrivals are generated over fixed time intervals using λΔt.

The simulated inter-arrival times are compared with the expected exponential distribution.

### Part 2 — Packet-Oriented Traffic Systems

Extends the simulator to packet-oriented systems with multiple service channels, covering:

- Erlang-B loss system;
- Erlang-C waiting system with an infinite FIFO queue;
- Finite-queue waiting system.

The simulator estimates blocking probability, delay probability, average delay, delay distributions, and queue length requirements for a target loss probability.

### Part 3 — Call-Centre System

Simulates a telecom operator call centre during peak-hour traffic, with calls arriving according to a Poisson process at 80 calls/hour.

The model includes two interacting subsystems:

- A general-purpose answering system with a finite FIFO queue;
- An area-specific answering system with an infinite FIFO queue.

Calls may be handled entirely by a general operator or transferred to a specialised operator. Different numbers of operators and queue lengths can be tested to evaluate delay probability, blocking probability, average delay, total system time, and waiting-time prediction accuracy.

For queued calls, the simulator estimates the expected waiting time using previously observed waiting times and compares the prediction with the actual waiting time to compute absolute and relative prediction errors.


## 📁 Folder Structure

```
lab1/
 └─ Part1/
     ├─ build/          → recommended folder to compile and store executables
     ├─ include/        → header files (.h), e.g. linked-list.h
     └─ src/            → source files (.c): Lab1a.c, Lab1b.c, linked-list.c

 └─ Part2/
     ├─ Results/        → results obtained during simulation (.txt)
     ├─ build/    
     ├─ include/        → header files (.h)
     ├─ plots/          → graphs created from the results obtained for a better visualization
     └─ src/            → source files (.c): Lab2a.c, Lab2b.c, Lab2c.c, linked-list.c, generate_events.c

 └─ Part3/              → final implementation of the system
     ├─ build/
     ├─ diagrams/       → flow diagrams
     ├─ include/        → header files (.h)
     ├─ plots/          → graphs created from the results obtained 
     ├─ Results/        → results obtained during simulation (.txt)
     └─ src/            → source files (.c): system.c, duration_of_calls.c, linked_list.c, generate_arrivals.c    
```

---

## ▶️ How to Compile

```
cd Lab1/Part3/build

gcc -I../include ../src/system.c ../src/linked_list.c ../src/generate_arrivals.c ../src/duration_of_calls.c -o system.exe -lm

./system.exe
```
