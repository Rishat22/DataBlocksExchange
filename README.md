# Batch command handler
* Commands are read line by line from the standard input and processed in blocks of N commands.
One command is one line, the specific value does not matter. If the data has run out , the block is forcibly terminated.
The N parameter is passed as a single command-line parameter as an integer.

* Together with the output to the console, the blocks should be saved in separate files


### The logic of operation for the block size (in the example, N == 3)

Input | Output | Comment
| ----- | ------ | ------------- |
| `cmd1` | | |
| `cmd2` | | |
| `{` | bulk: cmd1, cmd2 | The beginning of a dynamic block - we output the\n previous static block ahead of schedule |
| `cmd3` | | |
| `cmd4` | | |
| `}` | bulk: cmd3, cmd4 | The end of the dynamic block - output it. |
| `{` | | |
| `cmd5` | | |
| `cmd6` | | |
| `{` | | Ignoring nested commands |
| `cmd7` | | |
| `cmd8` | | |
| `}` | | Ignoring nested commands |
| `cmd9` | | |
| `}` | bulk: cmd5, cmd6, cmd7, cmd8, cmd9 | The end of the dynamic block - output it. |
| `{` | | |
| `cmd10` | | |
| `cmd11` | | |
| `EOF` | | End of input – the dynamic block is ignored, we do not output it. |


