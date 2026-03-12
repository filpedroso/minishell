# Minishell Test Cases

A comprehensive list of input cases and expected results for testing a 42 minishell implementation.
Each test shows the **input**, **expected stdout**, **expected stderr** (if any), and **exit code**.

> **Reference shell:** All expected outputs match `bash` behavior unless otherwise noted.
> Legend: `[empty]` = empty output/line, `$?` = last exit code, `$USER`/`$HOME`/`$PWD` = env values.

---

## 1. `echo`

### Basic

| Input | Expected Stdout | Exit Code |
|---|---|---|
| `echo` | `[empty line]` | 0 |
| `echo hello` | `hello` | 0 |
| `echo hello world` | `hello world` | 0 |
| `echo -n hello` | `hello` (no newline) | 0 |
| `echo -nn hello` | `hello` (no newline) | 0 |
| `echo -n -n -n hello` | `hello` (no newline) | 0 |
| `echo -nnnnn hello` | `hello` (no newline) | 0 |
| `echo -n` | `[empty, no newline]` | 0 |
| `echo -nnm hello` | `-nnm hello` (invalid flag, print as-is) | 0 |

### Quoting

| Input | Expected Stdout | Exit Code |
|---|---|---|
| `echo "hello"` | `hello` | 0 |
| `echo 'hello'` | `hello` | 0 |
| `echo ""` | `[empty line]` | 0 |
| `echo ''` | `[empty line]` | 0 |
| `echo "hello world"` | `hello world` | 0 |
| `echo 'hello world'` | `hello world` | 0 |
| `echo "it's a test"` | `it's a test` | 0 |
| `echo 'say "hello"'` | `say "hello"` | 0 |
| `echo "'hello'"` | `'hello'` | 0 |
| `echo '"hello"'` | `"hello"` | 0 |
| `echo "\n \n"` | `\n \n` (no escape interp) | 0 |

### Variable Expansion

| Input | Expected Stdout | Exit Code |
|---|---|---|
| `echo $USER` | `<username>` | 0 |
| `echo "$USER"` | `<username>` | 0 |
| `echo '$USER'` | `$USER` (literal) | 0 |
| `echo "$HOME"` | `<home path>` | 0 |
| `echo '$HOME'` | `$HOME` | 0 |
| `echo $PWD` | `<current dir>` | 0 |
| `echo $?` | `0` | 0 |
| `ls nonexistent; echo $?` | (stderr) then `2` | 0 |
| `echo $UNDEFINED_VAR` | `[empty line]` | 0 |
| `echo "$UNDEFINED_VAR"` | `[empty line]` | 0 |
| `echo $` | `$` | 0 |
| `echo $ $` | `$ $` | 0 |
| `echo $USER$USER` | `<user><user>` | 0 |
| `echo "$USER$USER"` | `<user><user>` | 0 |
| `echo text$USER` | `text<username>` | 0 |
| `echo $USER=test` | `<username>=test` | 0 |
| `echo "text $USER text"` | `text <username> text` | 0 |

### Special / Edge Cases

| Input | Expected Stdout | Exit Code |
|---|---|---|
| `echo "bip \| bip ; coyote > <"` | `bip \| bip ; coyote > <` | 0 |
| `echo hi";" hihi` | `hi; hihi` | 0 |
| `echo hi " ; " hihi` | `hi  ;  hihi` | 0 |
| `echo -n -n -nnnn -nnnnm` | `-nnnnm` (no newline) | 0 |
| `echo "\"` | `\"` | 0 |
| `echo $USER` | `<username>` | 0 |
| `echo """""$USER"""""` | `<username>` | 0 |

---

## 2. `cd`

| Input | Expected Stdout | Notes / Exit Code |
|---|---|---|
| `cd` | `[empty]` | Goes to `$HOME`, exit 0 |
| `cd .` | `[empty]` | Stay in place, exit 0 |
| `cd ..` | `[empty]` | Go up one level, exit 0 |
| `cd /` | `[empty]` | Go to root, exit 0 |
| `cd /tmp` | `[empty]` | Go to /tmp, exit 0 |
| `cd /nonexistent` | stderr: `cd: /nonexistent: No such file or directory` | exit 1 |
| `cd a b c` | stderr: `cd: too many arguments` | exit 1 |
| `cd $HOME` | `[empty]` | Goes to home, exit 0 |
| `cd $OLDPWD` | `[empty]` | Goes to previous dir, exit 0 |
| `cd ""` | stderr: `cd: : No such file or directory` | exit 1 (bash) |
| `cd; pwd` | `<home path>` | exit 0 |
| `cd /; pwd` | `/` | exit 0 |
| `cd ../../../..; pwd` | `<resolved path>` | exit 0 |
| `cd -` | `<previous dir>` (prints OLDPWD) | exit 0 |
| `unset HOME; cd` | stderr: `cd: HOME not set` | exit 1 |

---

## 3. `pwd`

| Input | Expected Stdout | Exit Code |
|---|---|---|
| `pwd` | `<current directory path>` | 0 |
| `pwd extra_arg` | `<current directory path>` | 0 (bash ignores extra args) |
| `cd /tmp; pwd` | `/tmp` | 0 |
| `cd /; pwd` | `/` | 0 |

---

## 4. `export`

### Valid Cases

| Input | Expected Behavior | Exit Code |
|---|---|---|
| `export VAR=hello` | Sets VAR, no output | 0 |
| `export VAR=` | Sets VAR to empty string | 0 |
| `export VAR` | Marks VAR as exported (no value) | 0 |
| `export A=1 B=2 C=3` | Sets all three variables | 0 |
| `export VAR=hello; echo $VAR` | `hello` | 0 |
| `export _VAR=test` | Sets _VAR, valid | 0 |
| `export VAR123=ok` | Sets VAR123, valid | 0 |
| `export` | Lists all exported vars (like `env`) | 0 |

### Invalid Identifiers

| Input | Expected Stderr | Exit Code |
|---|---|---|
| `export 1VAR=test` | `export: '1VAR=test': not a valid identifier` | 1 |
| `export -TEST=100` | `export: '-TEST=100': not a valid identifier` | 1 |
| `export TE-ST=100` | `export: 'TE-ST=100': not a valid identifier` | 1 |
| `export =====` | `export: '=====': not a valid identifier` | 1 |
| `export "="="="` | `export: '===': not a valid identifier` | 1 |
| `export ""=""` | `export: '': not a valid identifier` | 1 |
| `export TE*ST=123` | `export: 'TE*ST=123': not a valid identifier` | 1 |
| `export TES.T=123` | `export: 'TES.T=123': not a valid identifier` | 1 |
| `export TES+T=123` | `export: 'TES+T=123': not a valid identifier` | 1 |

---

## 5. `unset`

| Input | Expected Behavior | Exit Code |
|---|---|---|
| `unset` | No-op | 0 |
| `export VAR=hi; unset VAR; echo $VAR` | `[empty line]` | 0 |
| `unset NONEXISTENT` | No-op, no output | 0 |
| `unset PATH` | PATH is unset (commands will fail) | 0 |
| `unset PATH; ls` | stderr: `ls: command not found` | 127 |
| `unset HOME; cd` | stderr: `cd: HOME not set` | 1 |
| `unset PWD` | Unsets PWD | 0 |
| `unset A B C` | Unsets all three | 0 |
| `unset 1VAR` | `unset: '1VAR': not a valid identifier` | 1 |
| `unset TES.T` | `unset: 'TES.T': not a valid identifier` | 1 |

---

## 6. `env`

| Input | Expected Behavior | Exit Code |
|---|---|---|
| `env` | Prints all environment variables | 0 |
| `export VAR=test; env \| grep VAR` | `VAR=test` | 0 |
| `unset PATH; env \| grep PATH` | `[empty]` (PATH is gone) | 0 |
| `env \| grep USER` | `USER=<username>` | 0 |
| `env \| grep SHLVL` | `SHLVL=<level>` | 0 |

> **Note:** `export` and `env` display variables differently — `export` prepends `declare -x`, `env` does not.

---

## 7. `exit`

| Input | Expected Exit Code | Notes |
|---|---|---|
| `exit` | last `$?` value | 0 if shell just started |
| `exit 0` | 0 | |
| `exit 1` | 1 | |
| `exit 42` | 42 | |
| `exit -1` | 255 | wraps: (-1 % 256) |
| `exit -42` | 214 | |
| `exit 259` | 3 | 259 % 256 |
| `exit 9223372036854775807` | 255 | max int64, overflow |
| `exit 9223372036854775808` | stderr: numeric arg required; exit 2 | |
| `exit azerty` | stderr: `exit: azerty: numeric argument required`; exit 2 | |
| `exit 42 53` | stderr: `exit: too many arguments`; stays in shell | 1 |
| `exit 0 0` | stderr: too many arguments | 1 |
| `exit "42"` | 42 | quotes stripped |
| `exit "+10"` | 10 | |
| `exit -- 0` | 0 (bash treats -- as terminator) | |

---

## 8. Pipes `\|`

| Input | Expected Stdout | Exit Code |
|---|---|---|
| `echo hello \| cat` | `hello` | 0 |
| `echo hello \| cat -e` | `hello$` | 0 |
| `echo oui \| echo non` | `non` (right side ignores stdin) | 0 |
| `ls \| grep ".c"` | `.c` files | 0 |
| `echo test \| cat -e \| cat -e` | `test$$` | 0 |
| `cat /etc/shells \| grep sh \| head -3` | first 3 sh entries | 0 |
| `echo hello \| rev` | `olleh` | 0 |
| `env \| grep USER` | `USER=<username>` | 0 |
| `ls -la \| grep "."` | all ls output | 0 |
| `echo hello \| cat \| cat \| cat \| cat \| cat` | `hello` | 0 |
| `\| ls` | stderr: syntax error near `\|` | 2 |
| `ls \|\| ls` | stderr: syntax error (bonus only) | 2 |
| `echo hi \|\|` | stderr: syntax error | 2 |
| `echo ; \|` | stderr: syntax error | 2 |
| `echo test \| cat \| \| cat` | stderr: syntax error | 2 |
| `exit 0 \| exit 1` | exit code = 1 (right pipe cmd) | 1 |
| `exit 1 \| exit 0` | exit code = 0 (right pipe cmd) | 0 |
| `sort \| ls` | stdout of `ls` to terminal; stdin ignored | 0 |

---

## 9. Redirections

### Output `>`

| Input | Expected Behavior | Exit Code |
|---|---|---|
| `echo hello > out.txt` | Creates out.txt with `hello` | 0 |
| `> out.txt` | Creates/truncates out.txt, empty | 0 |
| `> out.txt echo hello` | Creates out.txt with `hello` | 0 |
| `echo hi > out.txt; cat out.txt` | `hi` | 0 |
| `echo first > out; echo second > out; cat out` | `second` (truncates) | 0 |
| `echo hello > $UNDEFINED` | Creates file named empty string → error | 1 |
| `echo hello > out1 > out2` | out1 empty, out2 has `hello` | 0 |

### Append `>>`

| Input | Expected Behavior | Exit Code |
|---|---|---|
| `echo a >> out.txt; echo b >> out.txt; cat out.txt` | `a\nb` | 0 |
| `echo 2 >> out1 > out2` | out1 appended, out2 created | 0 |
| `echo 2 > out1 >> out2` | out1 created, out2 appended | 0 |

### Input `<`

| Input | Expected Behavior | Exit Code |
|---|---|---|
| `cat < Makefile` | Contents of Makefile | 0 |
| `wc -l < Makefile` | Line count | 0 |
| `cat < nonexistent.txt` | stderr: No such file or directory | 1 |
| `< Makefile cat` | Contents of Makefile | 0 |
| `< Makefile > out cat` | out contains Makefile content | 0 |
| `< in1 < in2 cat` | Contents of in2 (last one wins) | 0 |

### Combinations

| Input | Expected Behavior | Exit Code |
|---|---|---|
| `cat < Makefile \| grep FLAGS > out` | out has matching lines | 0 |
| `< Makefile cat \| grep FLAGS > out` | out has matching lines | 0 |
| `cat < Makefile > out; cat out` | out has Makefile content | 0 |
| `wc < Makefile -l \| cat -e > outfile` | Line count with `$` at end | 0 |
| `< in1 cat -e > out1 \| < in2 cat` | in1→out1, in2→stdout | 0 |

### Edge / Error Cases

| Input | Expected Behavior | Exit Code |
|---|---|---|
| `echo >` | stderr: syntax error near newline | 2 |
| `echo bonjour >>> out` | stderr: syntax error near `>` | 2 |
| `echo bonjour > > out` | stderr: syntax error | 2 |
| `cat < >` | stderr: syntax error | 2 |
| `cat < <` | stderr: syntax error | 2 |
| `cat > >` | stderr: syntax error | 2 |
| `> a ls > b < Makefile` | a empty, b has ls output | 0 |
| `not_cmd > outfile` | stderr: command not found; outfile created (empty) | 127 |

---

## 10. Heredoc `<<`

| Input | Expected Behavior | Exit Code |
|---|---|---|
| `cat << EOF\nhello\nworld\nEOF` | `hello\nworld` | 0 |
| `cat << EOF\n$USER\nEOF` | `<username>` (expanded) | 0 |
| `cat << 'EOF'\n$USER\nEOF` | `$USER` (not expanded, quoted delimiter) | 0 |
| `cat << EOF << LIMITER\nhello\nEOF\nworld\nLIMITER` | `world` (last heredoc wins) | 0 |
| `cat << fake < Makefile << "limiter"\n42\nlimiter` | `42` (last heredoc, quoted) | 0 |
| `cat << EOF > outfile\nhello\nEOF; cat outfile` | `hello` in outfile | 0 |

---

## 11. Variable Expansion Edge Cases

| Input | Expected Stdout | Exit Code |
|---|---|---|
| `echo $?` | `0` | 0 |
| `ls /nonexistent; echo $?` | (stderr then) `2` | 0 |
| `echo $UNDEFINED` | `[empty line]` | 0 |
| `echo "$UNDEFINED"` | `[empty line]` | 0 |
| `echo $` | `$` | 0 |
| `echo "$"` | `$` | 0 |
| `echo $?$?` | `00` | 0 |
| `export VAR=hello; echo $VARworld` | `[empty]` (`$VARworld` undefined) | 0 |
| `export VAR=hello; echo ${VAR}` | Depends on implementation (bonus) | 0 |
| `echo $1` | `[empty]` (positional params not set) | 0 |
| `export A=foo; echo $A$A` | `foofoo` | 0 |
| `echo $USER=4` | `<username>=4` | 0 |

---

## 12. Quoting Edge Cases

| Input | Expected Stdout | Exit Code |
|---|---|---|
| `echo "hello''world"` | `hello''world` | 0 |
| `echo 'hello""world'` | `hello""world` | 0 |
| `echo "it'"'"'s"` | `it's` | 0 |
| `echo ok"hello"ok1"mfg"` | `okhellook1mfg` | 0 |
| `echo ok'hello'ok1'mfg'` | `okhellook1mfg` | 0 |
| `echo "o""k "hellook1` | `ok hellook1` | 0 |
| `echo '"***hello***"'` | `"***hello***"` | 0 |
| `echo "'***hello***'"` | `'***hello***'` | 0 |
| `echo "-n" bonjour` | `-n bonjour` (not a flag) | 0 |
| `echo "-nnnn" bonjour` | `-nnnn bonjour` (not all-n) | 0 |
| `echo "-nnnn" -n bonjour` | `-nnnn bonjour` (no newline) | 0 |

---

## 13. Command Execution

| Input | Expected Behavior | Exit Code |
|---|---|---|
| `/bin/ls` | lists current dir | 0 |
| `/bin/echo hello` | `hello` | 0 |
| `ls` | lists current dir | 0 |
| `ls -la` | detailed listing | 0 |
| `/nonexistent_cmd` | stderr: No such file or directory | 126 or 127 |
| `nonexistent_cmd` | stderr: command not found | 127 |
| `not_cmd hello > outfile` | stderr: not found; outfile created empty | 127 |
| `./minishell` | new shell instance (or not found if not in path) | varies |
| `""` | stderr: command not found or empty | 127 |
| `/bin/ls nonexistentdir` | stderr: ls error | 2 |
| `cat nonexistentfile` | stderr: No such file or directory | 1 |

---

## 14. Signal Handling

| Action | Expected Behavior |
|---|---|
| `Ctrl+C` on empty prompt | New line with prompt, exit code = 130 |
| `Ctrl+C` during `sleep 10` | Interrupts sleep, new prompt, exit code = 130 |
| `Ctrl+D` on empty prompt | Exit shell (like `exit`) |
| `Ctrl+\` on empty prompt | No-op (bash ignores SIGQUIT on prompt) |
| `Ctrl+\` during `sleep 10` | Quits process, exit code = 131 |
| Type something then `Ctrl+C` | Clears line, new prompt, exit code = 130 |

---

## 15. Syntax / Parse Errors

| Input | Expected Behavior | Exit Code |
|---|---|---|
| `;` | stderr: syntax error near `;` | 2 |
| `;;` | stderr: syntax error | 2 |
| `\|` | stderr: syntax error near `\|` | 2 |
| `echo \|` | stderr: syntax error near newline | 2 |
| `echo hi ; \|` | stderr: syntax error near `\|` | 2 |
| `echo bonjour \| ;` | stderr: syntax error near `;` | 2 |
| `echo bonjour ; \|` | stderr: syntax error | 2 |
| `echo \|\|\| echo` | stderr: syntax error | 2 |
| `'\''\'` | pipes with empty strings | depends |
| `echo hi\` | stderr: syntax error (unclosed backslash) | 2 |

---

## 16. Miscellaneous

| Input | Expected Behavior | Exit Code |
|---|---|---|
| `export var; export var=test; echo $var` | `test` | 0 |
| `env; export; env` | env vars not sorted the same as export | 0 |
| `export LOL=lala ROR=rara; env \| grep -E 'LOL\|ROR'` | `LOL=lala` and `ROR=rara` | 0 |
| `unset LOL ROR; echo $LOL $ROR` | `[empty line]` | 0 |
| `$bla` (undefined) | stderr: command not found | 127 |
| `export var="cat Makefile \| grep >"` | Sets var without executing | 0 |
| `cat /dev/urandom \| head -n 1 \| cat -e` | Random binary output + `$` | 0 |
| `sleep 5 \| exit` | exit runs in subshell, sleep continues briefly | 0 |
| `echo $SHLVL` | `1` (or incremented in nested shell) | 0 |
| `cat \| cat \| cat` | Waits for stdin (Ctrl+D to end) | 0 |
| `ls -la \| grep "." \| wc -l` | line count of ls output | 0 |
| `echo test \| cat -e \| cat -e \| cat -e \| cat -e` | `test$$$$` | 0 |

---

## 17. `$PATH` Manipulation

| Input | Expected Behavior | Exit Code |
|---|---|---|
| `echo $PATH` | Current PATH value | 0 |
| `export PATH=""; ls` | stderr: ls not found | 127 |
| `export PATH=/bin; ls` | Works (ls is in /bin) | 0 |
| `unset PATH; ls` | stderr: ls not found | 127 |
| `unset PATH; /bin/ls` | Works (absolute path) | 0 |
| `export PATH=/bin:/usr/bin; echo hello` | Works | 0 |

---

## 18. Multiple Redirections / Ambiguous Cases

| Input | Expected Behavior | Exit Code |
|---|---|---|
| `echo hello > out1 > out2; cat out1; cat out2` | out1 empty, out2=hello | 0 |
| `echo 2 >> out1 > out2` | out1 appended with 2, out2 empty | 0 |
| `< in1 < in2 cat` | reads from in2 (last wins) | 0 |
| `< in1 < in2 < in3 < Makefile rev` | reads Makefile | 0 |
| `wc < Makefile -l \| cat -e > out \| echo hello \| rev > out2` | complex multi-pipe+redir | 0 |

---

## 19. Builtins in Pipes

> In bash, builtins in a pipe run in a subshell, so changes to env do NOT persist.

| Input | Expected Behavior |
|---|---|
| `export TEST=1 \| env \| grep TEST` | TEST may not appear (subshell) |
| `cd /tmp \| pwd` | pwd shows original dir (cd in subshell) |
| `unset TEST \| cat -e` | no output; TEST still set after pipe |
| `export A=1; echo test \| export B=2; echo $B` | `[empty]` (B not set outside pipe) |

---

## 20. Empty / Whitespace Inputs

| Input | Expected Behavior | Exit Code |
|---|---|---|
| `   ` (spaces only) | New prompt, no error | 0 |
| `\t\t` (tabs only) | New prompt, no error | 0 |
| `` (empty enter) | New prompt | 0 |
| `   echo hello   ` | `hello` (trim leading/trailing spaces) | 0 |
| `echo   hello   world` | `hello   world` (internal spaces preserved) | 0 |
