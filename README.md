# pipex

Recreation of a Unix pipe mechanism that allows output from one command to be passed as input to another. The program mimics the behavior of `< file1 cmd1 | cmd2 > file2` with proper input/output redirection.

## What Was Done:

- Implemented process management using system calls like `fork`, `execve`, and `pipe`.
- Managed input/output stream redirection with `dup` and `dup2`.
- Implemented thorough error handling with system-level functions (`perror`, `strerror`).
- Supported multiple pipelines and handled complex command sequences.
- Created a structured `Makefile` for compilation and dependency management.
- Handling multiple pipes in command sequences.
- Supporting `here_doc` functionality (`<<`) for input redirection.
