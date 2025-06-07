# Priority Inversion Demo

This project demonstrates the priority inversion problem and its solution using priority inheritance in C with POSIX threads.

## How to Create This Codespace (No Extensions Required)

1. **Fork or Clone the Repository**
   - Fork this repository to your own GitHub account or clone it directly:
     ```bash
     git clone <your-repo-url>
     cd topic-in-computer-science-projec-t
     ```

2. **Open in GitHub Codespaces**
   - On GitHub, click the green `<> Code` button and select `Open with Codespaces` > `New codespace`.
   - Or, from the command line (with [GitHub CLI](https://cli.github.com/) installed):
     ```bash
     gh codespace create --repo <your-repo-url>
     gh codespace code
     ```

## How to Run the `original_system.c` Demo

1. **Compile the Code**
   - Open the integrated terminal in Codespaces.
   - Run:
     ```bash
     gcc original_system.c -o original_system -lpthread
     ```
   - Becasue I have done this step and push the file on the github, you can skip this step

2. **Run the Program**
   - In the terminal, execute:
     ```bash
     ./original_system
     ```

3. **Expected Output**
   - The program will print logs showing the behavior of low, medium, and high priority tasks, demonstrating the priority inversion problem.

## How to Run the `priority_inheritance.c` Demo

1. **Compile the Code**
   - Open the integrated terminal in Codespaces.
   - Run:
     ```bash
     gcc priority_inheritance.c -o priority_inheritance -lpthread
     ```
   - Becasue I have done this step and push the file on the github, you can skip this step

2. **Run the Program with Real-Time Scheduling**
   - You need to use `sudo` to enable real-time scheduling and priority inheritance:
     ```bash
     sudo ./priority_inheritance
     ```

3. **Expected Output**
   - The program will print logs showing the behavior of low, medium, and high priority tasks, demonstrating how priority inheritance solves the priority inversion problem.

---

**Note:**  
If you want to experiment with priority inheritance, see `priority_inheritance.c` and run it with `sudo` for real-time scheduling support.
