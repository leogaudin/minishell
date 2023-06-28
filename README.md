# minishell
Write your own Shell.

## TODO
### Tokenizer
- We need to define structures for every type of token (pipes, redirections, etc.)
- We need to define a structure for the command line (a list of tokens)
* Video of a guy who makes a C tokenizer: [https://www.youtube.com/watch?v=AqyZztKlSGQ](https://www.youtube.com/watch?v=AqyZztKlSGQ)

---
## Requirements
### Mandatory
The shell should:
* Display a **prompt** when waiting for a new command.
* Have a working **history**.
* Search and launch the right executable (based on the `PATH` variable or using a relative or an absolute path).
* Not use more than **one global variable**. Think about it. You will have to explain its purpose.
* Not interpret unclosed quotes or special characters which are not required by the subject such as `\` (backslash) or `;` (semicolon).
* Handle `’` (single quote) which should prevent the shell from interpreting the meta- characters in the quoted sequence.
* Handle `"` (double quote) which should prevent the shell from interpreting the meta- characters in the quoted sequence except for $ (dollar sign).

* Implement redirections:
  * `<` should redirect input.
  * `>` should redirect output.
  * `<<` should be given a delimiter, then read the input until a line containing the delimiter is seen. However, it doesn’t have to update the history!
  * `>>` should redirect output in append mode.
* Implement **pipes** (`|` character). The output of each command in the pipeline is
connected to the input of the next command via a pipe.
* Handle **environment variables** (`$` followed by a sequence of characters) which should expand to their values.
* Handle $? which should expand to the exit status of the most recently executed foreground pipeline.
* Handle `ctrl-C`, `ctrl-D` and `ctrl-\` which should behave like in bash.

* In interactive mode:
  * `ctrl-C` displays a new prompt on a new line.
  * `ctrl-D` exits the shell.
  * `ctrl-\` does nothing.

* Your shell must implement the following **builtins**:
  * `echo` with option `-n`
  * `cd` with only a relative or absolute path
  * `pwd` with no options
  * `export` with no options
  * `unset` with no options
  * `env` with no options or arguments
  * `exit` with no options

The `readline()` function can cause memory leaks. You don’t have to fix them. But that **doesn’t mean your own code, yes the code you wrote, can have memory leaks**.

> You should limit yourself to the subject description.  Anything that is not asked is not required.
>
>If you have any doubt about a requirement, take bash as a reference.

### Bonus
Your program has to implement:
* `&&` and `||` with parenthesis for priorities.
* Wildcards `*` should work for the current working directory.

## Resources
1.   https://www.geeksforgeeks.org/fork-system-call/
2.   https://gist.github.com/iomonad/a66f6e9cfb935dc12c0244c1e48db5c8
3.   https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html#Redirections
4.   http://sdz.tdct.org/sdz/la-programmation-systeme-en-c-sous-unix.html
5.   http://www.zeitoun.net/articles/communication-par-tuyau/start
6.   https://www.geeksforgeeks.org/dup-dup2-linux-system-call/
7.   https://sites.uclouvain.be/SystInfo/notes/Theorie/html/Fichiers/fichiers-signaux.html
8.   http://patatos.over-blog.com/article-liste-des-signaux-unix-47601760.html
9.   http://www-igm.univ-mlv.fr/~dr/NCSPDF/chapitre11.pdf
10.  https://www.geeksforgeeks.org/signals-c-language/
11.  https://brennan.io/2015/01/16/write-a-shell-in-c/
12.  https://www.geeksforgeeks.org/pipe-system-call/
13.  https://shapeshed.com/unix-exit-codes/
14.  https://stackoverflow.com/questions/13098620/using-stat-to-check-if-a-file-is-executable-in-c
15.  https://c-for-dummies.com/blog/?p=3246
16.  https://tomassetti.me/guide-parsing-algorithms-terminology/
17.  https://pastebin.com/K1YZchMK
18.  https://files.gogaz.org/42sh-appsinges.html
19.  https://ruslanspivak.com/lsbasi-part1/
20.  https://ruslanspivak.com/lsbasi-part2/
21.  https://ruslanspivak.com/lsbasi-part3/
22.  https://ruslanspivak.com/lsbasi-part4/
23.  https://ruslanspivak.com/lsbasi-part5/
24.  https://ruslanspivak.com/lsbasi-part6/
25.  https://ruslanspivak.com/lsbasi-part7/
26.  https://www.strchr.com/expression_evaluator
