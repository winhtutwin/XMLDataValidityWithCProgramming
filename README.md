# XML Validity Checker (C Programming Assignment 2)

This repository contains a structured C program designed to validate the structural syntax of an XML file (`note.xml`). The application parses through the tags to ensure every opening tag matches its respective closing tag sequentially.

## Project Constraints Followed
* **No Pointers:** Implemented completely using static data referencing and array boundaries.
* **No Dynamic Memory Allocation:** Avoids using `malloc()`, `calloc()`, or `free()`. Memory constraints are pre-defined statically at compile time.
* **Structures & Arrays:** Utilizes standard structural design (`struct`) to implement a static array-based collection architecture.

---

## How the Validation Logic Works

The application implements a **Stack Data Structure** behavior using a static array framework inside a `struct XmlValidator`. 

### Step-by-Step Execution:
1. **File Reading:** The program reads the full content of `note.xml` into a static character buffer array.
2. **Parsing Strings:** It iterates character by character looking for `<` and `>` bounds:
   * **Opening Tags (e.g., `<note>`):** When an opening tag is extracted, the tag name is **pushed** onto the stack array.
   * **Closing Tags (e.g., `</note>`):** When a closing tag is found, the program **pops** the top item off the stack array and compares the two strings using `strcmp()`.
3. **Evaluation:**
   * If an duplicate structural anomaly occurs (e.g., mismatched tag names or an unexpected closing tag before an opening one), the balance sequence breaks instantly.
   * At the end of parsing, if the stack tracker index returns to empty (`top == -1`), it proves all tags are balanced.

---

## Expected Terminal Output

When compiled and executed as shown in the demonstration instructions, the application verifies the source data file structure and prints out either of the following states:

### Case 1: Valid XML Format
```bash
PS C:\Users\N1C\Desktop\DSAAssignments\Assignment2> gcc -o main.exe main.c
PS C:\Users\N1C\Desktop\DSAAssignments\Assignment2> .\main.exe
XML is valid
