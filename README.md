## Miva Student Record System

This is a simple terminal-based student record system written in C. It lets you add, edit, remove, search, sort, and save student records.

### Files

- `main.c` – program entry point and menu loop  
- `input.c` / `input.h` – helpers for reading and validating user input  
- `student_db.c` / `student_db.h` – in-memory student database and file I/O  
- `menu.c` / `menu.h` – menu display

### Build

From this folder, compile with:

```bash
cc -Wall -Wextra -o mivadb main.c input.c student_db.c menu.c
```

If you use `clang`, you can also write:

```bash
clang -Wall -Wextra -o mivadb main.c input.c student_db.c menu.c
```

### Run

After a successful build:

```bash
./mivadb
```

Follow the on-screen prompts to enter your name and interact with the menu. Records are saved to and loaded from the `students.txt` file in this directory.

