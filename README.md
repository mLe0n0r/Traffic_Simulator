## 📁 Folder Structure

```
lab1/
 └─ Part1/
     ├─ include/        → header files (.h), e.g. linked-list.h
     ├─ src/            → source files (.c): Lab1a.c, Lab1b.c, linked-list.c
     └─ build/          → recommended folder to compile and store executables
                          (contains an optional Makefile)
 └─ Part2/
     ├─ include/        → header files (.h)
     ├─ src/            → source files (.c): Lab2a.c, Lab2b.c, Lab2c.c, linked-list.c, generate_events.c
     ├─ build/
     ├─ Results/        → results obtained during simulation (.txt)
     └─ plots/          → graphs created from the results obtained for a better visualization
```

---

## ▶️ How to Compile

```

cd Lab2/Part2/build

gcc -I../include ../src/Lab2c.c ../src/linked-list.c ../src/generate_events.c -o generalCase.exe

./generalCase.exe
```

---
## Credits
André Silva

Leonor Guedes

Rafael Parody
