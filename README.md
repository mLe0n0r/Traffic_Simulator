## Traffic Simulator

This repository provides a discrete-event simulation of a call-centre system developed in the context of the Telecommunications Networks course. The system models two interacting subsystems: a general-purpose answering subsystem, with a finite waiting queue, and an area-specific answering subsystem, which operates with an infinite queue. The simulation allows the configuration of the number of operators, both general-purpose and area-specific, as well as the length of the general-purpose queue.


## 📁 Folder Structure

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

 └─ Part3/              → 
     ├─ build/
     ├─ diagrams/       → flow diagrams
     ├─ include/        → header files (.h)
     ├─ plots/          → graphs created from the results obtained 
     ├─ Results/        → results obtained during simulation (.txt)
     └─ src/            → source files (.c): system.c, duration_of_calls.c, linked_list.c, generate_arrivals.c    

---

## ▶️ How to Compile

cd Lab2/Part3/build

gcc -I../include ../src/system.c ../src/linked_list.c ../src/generate_arrivals.c ../src/duration_of_calls.c -o system.exe -lm

./system.exe


