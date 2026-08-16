# ARM Session 01: Embedded C Essentials & Concepts Recap

---

### **1. Basic Pointers & Pass by Reference**
**Concept:** Passing a standard variable passes a **copy** (Pass by Value), leaving the caller's variable unchanged.
Passing an **address** (`&x`) to a pointer parameter (`int *ptr`) allows the function to dereference and modify the original variable directly in memory.

<img width="253" height="58" alt="image" src="https://github.com/user-attachments/assets/a523c88f-088f-4363-9cc1-ffcf571cfea0" />

```c
#include <stdio.h>

void increment(int *ptr)
{
    (*ptr)++; // Dereference pointer to modify caller's original value
}

int main(void)
{
    int x = 10;

    printf("Before = %d\n", x);
    increment(&x);
    printf("After  = %d\n", x);

    return 0;
}

```

---

### **2. Pointers & Array Arithmetic**

**Concept:** The array identifier (`arr`) decays into the address of its first element (`&arr[0]`). Array subscripting `arr[i]` and pointer arithmetic `*(ptr + i)` resolve to the exact same memory offset.

<img width="155" height="103" alt="image" src="https://github.com/user-attachments/assets/1a1f9e57-2af7-49da-b47e-d696f6373f8b" />

```c
#include <stdio.h>

int main(void)
{
    int arr[5] = {10, 20, 30, 40, 50};
    int *ptr = arr; // Points to arr[0]

    for(int i = 0; i < 5; i++)
    {
        printf("Index %d: %d \n", i, *(ptr + i));
    }

    return 0;
}

```

---

### **3. Function Pointers & Callback Mechanism**

**Concept:** Function names represent their starting memory addresses. Function pointers (`void (*callback)(int, int)`) decouple high-level logic from lower-level drivers, enabling flexible runtime event registration for ISRs, timers, ADC conversions, and communication buses.

<img width="227" height="56" alt="image" src="https://github.com/user-attachments/assets/75ccd647-ae50-43f8-87dd-5effb7c9a3bf" />

```c
#include <stdio.h>

void add(int a, int b)
{
    printf("Addition = %d\n", a + b);
}

void multiply(int a, int b)
{
    printf("Multiplication = %d\n", a * b);
}

void calculate(int a, int b, void (*callback)(int, int))
{
    callback(a, b); // Invoke the dynamically passed function
}

int main(void)
{
    calculate(5, 3, add);
    calculate(5, 3, multiply);

    return 0;
}

```


### **4. Structures (Grouping Related Data)**

**Concept:** Structures allow bundling heterogeneous data types (such as strings and numeric fields) representing a single entity under one cohesive user-defined data type.

<img width="196" height="50" alt="image" src="https://github.com/user-attachments/assets/4be8844a-d402-45e0-ab20-079c48c885ef" />

```c
#include <stdio.h>
#include <string.h>

struct Student
{
    char name[20];
    float grade;
};

int main(void)
{
    struct Student s1;

    strcpy(s1.name, "Hager");
    s1.grade = 95.5;

    printf("Name  : %s\n", s1.name);
    printf("Grade : %.2f\n", s1.grade);

    return 0;
}

```

---

### **5. Pointers to Structures (Arrow Operator `->`)**

**Concept:** Passing large structures by value wastes stack memory and prevents mutating the original instance. 
Passing a pointer to a struct (`struct Student *ptr`) allows direct modification using the arrow operator `ptr->member` (equivalent to `(*ptr).member`).

<img width="196" height="53" alt="image" src="https://github.com/user-attachments/assets/83607c45-3b56-4185-a4b3-068660404b64" />

```c
#include <stdio.h>

struct Student
{
    int age;
    float grade;
};

void updateStudent(struct Student *ptr)
{
    ptr->age = 24;
    ptr->grade = 98.0;
}

int main(void)
{
    struct Student s = {23, 90.0};

    updateStudent(&s);

    printf("Age   = %d\n", s.age);
    printf("Grade = %.2f\n", s.grade);

    return 0;
}

```

---
### **6. Const Variables**

**concept**: const is used to declare a variable whose value cannot be changed after initialization.

<img width="565" height="91" alt="image" src="https://github.com/user-attachments/assets/bcdd2da5-415c-4897-8b87-7ff62cde77d9" />

```c
#include <stdio.h>

int main(void)
{
    const int x = 10;

    printf("Before = %d\n", x);

    x = 20;   // Trying to modify a const variable

    printf("After = %d\n", x);

    return 0;
}

```

---

### **7. Static Local Variables**

**Concept:** Declaring a local variable `static` allocates it in the `.data` or `.bss` section rather than on the call stack.
It preserves its value between consecutive function invocations while remaining strictly scoped inside the function body.

<img width="255" height="67" alt="image" src="https://github.com/user-attachments/assets/561e4068-2f6b-466e-9d7a-69b780d189e4" />

```c
#include <stdio.h>

void counter(void)
{
// without static result will always be 1
    static int count = 0; // Initialized once; persists across function calls
    count++;
    printf("%d\n", count);
}

int main(void)
{
    counter(); // Prints 1
    counter(); // Prints 2
    counter(); // Prints 3

    return 0;
}

```

---
