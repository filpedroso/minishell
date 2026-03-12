# Minishell — Essential Edge Case Tests

> Scoped to the **42 minishell subject** requirements.
> No `;`, no `\`, no `||`/`&&`, no bonus features.
> All expected outputs match **bash** behavior.
> Legend: `[newline]` = empty line printed, `[none]` = no stdout, `$USER`/`$HOME`/`$PWD` = live env values.

---

## 1. `echo`

| # | Input | Expected Stdout | Exit |
|---|---|---|---|
| 1 | `echo` | `[newline]` | 0 |
| 2 | `echo hello world` | `hello world` | 0 |
| 3 | `echo -n hello` | `hello` (no trailing newline) | 0 |
| 4 | `echo -n` | `[none]` | 0 |
| 5 | `echo -nnnnn hello` | `hello` (no newline) | 0 |
| 6 | `echo -nnm hello` | `-nnm hello` (invalid flag, printed as-is) | 0 |
| 7 | `echo "-n" hello` | `-n hello` (quoted flag = not a flag) | 0 |
| 8 | `echo hello     world` | `hello     world` (inner spaces preserved) | 0 |

---

## 2. Quoting

Single quotes block **everything**. Double quotes block everything **except `$`**.

| # | Input | Expected Stdout | Note |
|---|---|---|---|
| 1 | `echo 'hello $USER'` | `hello $USER` | `$` not expanded in `''` |
| 2 | `echo "hello $USER"` | `hello <username>` | `$` expanded in `""` |
| 3 | `echo '$USER'` | `$USER` | literal |
| 4 | `echo "$USER"` | `<username>` | expanded |
| 5 | `echo 'say "hi"'` | `say "hi"` | `""` inside `''` are literal |
| 6 | `echo "it's fine"` | `it's fine` | `''` inside `""` are literal |
| 7 | `echo ""` | `[newline]` | empty double quote |
| 8 | `echo ''` | `[newline]` | empty single quote |
| 9 | `echo "hello""world"` | `helloworld` | adjacent quoted tokens join |
| 10 | `echo 'a''b''c'` | `abc` | adjacent single-quoted tokens join |
| 11 | `echo "'$USER'"` | `'<username>'` | `$` expands inside outer `""` |
| 12 | `echo '"$USER"'` | `"$USER"` | nothing expands inside outer `''` |

---

## 3. Variable Expansion

| # | Input | Expected Stdout | Note |
|---|---|---|---|
| 1 | `echo $USER` | `<username>` | basic expansion |
| 2 | `echo $NOPE` | `[newline]` | undefined var = empty |
| 3 | `echo "$NOPE"` | `[newline]` | undefined inside `""` = empty |
| 4 | `echo $` | `$` | lone `$` = literal |
| 5 | `echo "$"` | `$` | same inside `""` |
| 6 | `echo $?` | `0` | exit status of last command |
| 7 | `ls /nonexistent_dir ; echo $?` | (stderr) then `2` | non-zero exit captured (use pipe if no `;`) |
| 8 | `false; echo $?` | `1` | ← use `/usr/bin/false` if no `;` |
| 9 | `echo $?$?` | `00` | two consecutive `$?` |
| 10 | `export A=foo; echo $A$A` | `foofoo` | adjacent expansions |
| 11 | `export A=foo; echo ${A}` | depends on impl (bash: `foo`) | `{}` is optional/bonus |
| 12 | `echo $USER=check` | `<username>=check` | expansion stops at `=` |

---

## 4. `cd`

Subject specifies **relative or absolute path only** (no `cd -`).

| # | Input | Expected Stdout | Exit | Note |
|---|---|---|---|---|
| 1 | `cd /tmp; pwd` | `/tmp` | 0 | absolute |
| 2 | `cd ..; pwd` | `<parent dir>` | 0 | relative |
| 3 | `cd; pwd` | `$HOME` | 0 | no args → HOME |
| 4 | `cd /nonexistent` | `[none]` | 1 | stderr: No such file |
| 5 | `cd a b c` | `[none]` | 1 | stderr: too many arguments |
| 6 | `unset HOME; cd` | `[none]` | 1 | stderr: HOME not set |
| 7 | `cd /tmp; cd -` | depends | — | `cd -` not required by subject |

---

## 5. `pwd`

| # | Input | Expected Stdout | Exit |
|---|---|---|---|
| 1 | `pwd` | `<current dir>` | 0 |
| 2 | `cd /tmp; pwd` | `/tmp` | 0 |
| 3 | `cd /; pwd` | `/` | 0 |

---

## 6. `export`

| # | Input | Expected Behavior | Exit |
|---|---|---|---|
| 1 | `export VAR=hello; echo $VAR` | `hello` | 0 |
| 2 | `export VAR=` | sets VAR to empty string | 0 |
| 3 | `export VAR` | marks VAR as exported (no value set) | 0 |
| 4 | `export A=1 B=2; echo $A $B` | `1 2` | 0 |
| 5 | `export` | lists all exported vars (like `declare -x`) | 0 |
| 6 | `export 1BAD=x` | stderr: not a valid identifier | 1 |
| 7 | `export -BAD=x` | stderr: not a valid identifier | 1 |
| 8 | `export BA.D=x` | stderr: not a valid identifier | 1 |
| 9 | `export =value` | stderr: not a valid identifier | 1 |
| 10 | `export VAR=a; export VAR=b; echo $VAR` | `b` | 0 |
| 11 | `export VAR=hello; unset VAR; export` | VAR no longer in export list | 0 |

---

## 7. `unset`

| # | Input | Expected Behavior | Exit |
|---|---|---|---|
| 1 | `export VAR=hi; unset VAR; echo $VAR` | `[newline]` | 0 |
| 2 | `unset NONEXISTENT` | no output, no error | 0 |
| 3 | `unset A B C` | unsets all three | 0 |
| 4 | `unset PATH; ls` | stderr: command not found | 127 |
| 5 | `unset HOME; cd; pwd` | error on cd, pwd unchanged | 1 |
| 6 | `unset 1INVALID` | stderr: not a valid identifier | 1 |

---

## 8. `env`

| # | Input | Expected Behavior | Exit |
|---|---|---|---|
| 1 | `env` | prints all env vars | 0 |
| 2 | `export TESTVAR=42; env \| grep TESTVAR` | `TESTVAR=42` | 0 |
| 3 | `unset TESTVAR; env \| grep TESTVAR` | no output (grep finds nothing) | 1 |
| 4 | `unset PATH; env \| grep PATH` | no output | 0 |

---

## 9. `exit`

| # | Input | Shell Exit Code | Note |
|---|---|---|---|
| 1 | `exit` | last `$?` | clean exit |
| 2 | `exit 0` | 0 | |
| 3 | `exit 42` | 42 | |
| 4 | `exit 259` | 3 | 259 % 256 |
| 5 | `exit -1` | 255 | wraps around |
| 6 | `exit abc` | 2 | stderr: numeric argument required |
| 7 | `exit 1 2` | stays in shell | stderr: too many arguments; `$?` = 1 |
| 8 | `exit 9999999999999999999` | 2 | overflow → error |
| 9 | `exit "42"` | 42 | quotes stripped first |

---

## 10. Pipes `\|`

| # | Input | Expected Stdout | Exit |
|---|---|---|---|
| 1 | `echo hello \| cat` | `hello` | 0 |
| 2 | `echo hello \| cat -e` | `hello$` | 0 |
| 3 | `echo oui \| echo non` | `non` (right side ignores stdin) | 0 |
| 4 | `echo test \| cat \| cat \| cat` | `test` | 0 |
| 5 | `ls /tmp \| grep "."` | files in /tmp | 0 |
| 6 | `echo hello \| rev` | `olleh` | 0 |
| 7 | `echo test \| cat -e \| cat -e` | `test$$` | 0 |
| 8 | `exit 1 \| exit 0` | — | 0 (rightmost exit code wins) |
| 9 | `exit 0 \| exit 1` | — | 1 |
| 10 | `\| ls` | stderr: syntax error near `\|` | 2 |
| 11 | `echo hi \|` | stderr: syntax error near newline | 2 |

---

## 11. Redirections

### Output `>`

| # | Input | Expected Behavior | Exit |
|---|---|---|---|
| 1 | `echo hello > out.txt; cat out.txt` | `hello` | 0 |
| 2 | `> out.txt` | creates/truncates out.txt, no output | 0 |
| 3 | `echo a > out; echo b > out; cat out` | `b` (file truncated) | 0 |
| 4 | `echo a > out1 > out2; cat out1; cat out2` | out1 empty, out2=`a` | 0 |
| 5 | `notacmd > out.txt` | out.txt created (empty); stderr: not found | 127 |

### Append `>>`

| # | Input | Expected Behavior | Exit |
|---|---|---|---|
| 6 | `echo a >> f; echo b >> f; cat f` | `a` then `b` | 0 |
| 7 | `echo a > f; echo b >> f; cat f` | `a` then `b` | 0 |

### Input `<`

| # | Input | Expected Behavior | Exit |
|---|---|---|---|
| 8 | `cat < Makefile` | contents of Makefile | 0 |
| 9 | `wc -l < Makefile` | line count | 0 |
| 10 | `cat < nofile` | stderr: No such file or directory | 1 |
| 11 | `< in1 < in2 cat` | reads in2 (last `<` wins) | 0 |
| 12 | `< Makefile > out cat; cat out` | out has Makefile content | 0 |

### Syntax Errors

| # | Input | Expected Behavior | Exit |
|---|---|---|---|
| 13 | `echo >` | stderr: syntax error near newline | 2 |
| 14 | `cat < >` | stderr: syntax error | 2 |
| 15 | `echo >>> out` | stderr: syntax error near `>` | 2 |

---

## 12. Heredoc `<<`

| # | Input | Expected Stdout | Note |
|---|---|---|---|
| 1 | `cat << EOF` then `hello` then `EOF` | `hello` | basic heredoc |
| 2 | `cat << EOF` then `$USER` then `EOF` | `<username>` | `$` expands by default |
| 3 | `cat << 'EOF'` then `$USER` then `EOF` | `$USER` | quoted delimiter = no expansion |
| 4 | `cat << "EOF"` then `$USER` then `EOF` | `$USER` | double-quoted delimiter = no expansion |
| 5 | `cat << EOF > out` then `hello` then `EOF` | `[none]` (stdout → out) | redir applies to heredoc |
| 6 | `cat << A << B` then `first` then `A` then `second` then `B` | `second` | last heredoc wins |

---

## 13. Signals (Interactive)

| # | Action | Expected Behavior | `$?` after |
|---|---|---|---|
| 1 | `Ctrl+C` on empty prompt | new line + new prompt | 130 |
| 2 | `Ctrl+C` during `sleep 10` | interrupts sleep, new prompt | 130 |
| 3 | `Ctrl+D` on empty prompt | exits the shell | — |
| 4 | `Ctrl+\` on empty prompt | **nothing** (no-op in interactive) | unchanged |
| 5 | `Ctrl+\` during `sleep 10` | quits sleep | 131 |
| 6 | Type `abc` then `Ctrl+C` | clears line, new prompt | 130 |

---

## 14. Command Execution & PATH

| # | Input | Expected Behavior | Exit |
|---|---|---|---|
| 1 | `ls` | lists current dir via PATH | 0 |
| 2 | `/bin/ls` | lists current dir (absolute path) | 0 |
| 3 | `./minishell` | launches minishell (if exists) | 0 |
| 4 | `notacmd` | stderr: command not found | 127 |
| 5 | `/nonexistent` | stderr: No such file or directory | 127 |
| 6 | `export PATH=; ls` | stderr: command not found | 127 |
| 7 | `export PATH=; /bin/ls` | works (absolute path bypasses PATH) | 0 |
| 8 | `unset PATH; ls` | stderr: command not found | 127 |
| 9 | `unset PATH; /bin/ls` | works | 0 |

---

## 15. Empty / Whitespace Input

| # | Input | Expected Behavior | Exit |
|---|---|---|---|
| 1 | `[enter]` (empty) | new prompt | 0 |
| 2 | `   ` (spaces only) | new prompt | 0 |
| 3 | `echo   hello   world` | `hello   world` | 0 |
| 4 | `   echo hello` | `hello` (leading spaces OK) | 0 |
