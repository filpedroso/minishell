*This project has been created as part of the 42 curriculum by fpedroso, lcosta-a.*

# Minishell

## Description

Minishell is a simple shell implementation inspired by bash. The goal of this project is to learn about processes, file descriptors, lexing, tokenization, parsing, terminal control, execution pipelines and software architecture by creating a functional shell that can prompt the user for commands, parse them, and execute them.

This project covers a wide range of topics, including:
- **Lexical Analysis & Parsing:** Breaking down command lines into tokens and structuring them for execution.
- **Process Management:** Creating and managing child processes using `fork`, `execve`, and `waitpid`.
- **File Descriptors & Redirections:** Handling input/output redirection (`<`, `>`, `>>`) and pipes (`|`).
- **Signal Handling:** Managing signals like `SIGINT` (Ctrl+C) and `SIGEOF` (Ctrl+D).
- **Built-in Commands:** implementation of essential shell builtins like `cd`, `echo`, `pwd`, `export`, `unset`, `env`, and `exit`.
- **Environment Variables:** Managing and expanding environment variables (`$VAR`, `$?`).
- **Heredocs:** Support for `<<` redirections.

## Instructions

### Compilation

To compile the project, run the following command in the root of the project:

```bash
make
```

This will generate the `minishell` executable.

Other available `make` targets:
- `make clean`: Removes object files.
- `make fclean`: Removes object files and the executable.
- `make re`: Recompiles the project from scratch.
- `make all`: Same as `make clean`.

### Execution

Start the shell by running:

```bash
./minishell
```

Once inside, you can use it like a regular shell:
```bash
> ls -l | grep .c
> echo "Hello World"
> export MY_VAR=42
> echo $MY_VAR
```

## Resources

### References
- Crafting Interpreters - Book by Robert Nystrom
- Clean Code - Book by Robert Cecil Martin "Uncle Bob"
- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) - The official guide for Bash behavior.
- [42 Docs](https://harm-smits.github.io/42docs/projects/minishell)
- [42 Cursus git-book](https://42-cursus.gitbook.io/guide/3-rank-03/minishell)
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html) - Standard for shell behavior.
- [Write your own Shell](https://brennan.io/2015/01/16/write-a-shell-in-c/) - A helpful tutorial on shell basics.

### AI Usage

Artificial Intelligence tools were used during the development of this project for the following topics:

- **Design and Architectural consulting:** Long conversations about best design practices, ownership models and architecture were held with AI models, mostly Claude Sonnet 4.5
- **Test Generation:** Creation of test code for the lexer and parser, used during development
- **Debugging:** Analysis of valgrind and Asan reports for faster error identification, detection of bugs in the code

## Technical Choices

Clean architecture and its principles regarding variable and function naming, single resposability, clear and 
simple contracts
Canonical and idiomatic C for establishing ownership models and error handling
