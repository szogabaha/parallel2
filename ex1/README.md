# Numerical integration using trapezoids


The integral calculation is fair, so we do need to load balance the work of the threads since each of them compute the same value

```
$: ./numerical_integration -threads T -trapezoids N
```

/** Calculation process:
 * 1) Evaluate the value of the function in x1 to get b1. b1 = f(x1)
 * 2) Evaluate the value of the function in x2 to get b2. b2 = f(x2)
 * 3) h = x2 - x1
 */

/** Distribution of the work
 * 1) the main thread calculates how many trapezoids each thread need to calculate: n = N/T
 * 2) each thread has a local variable as a partial sum that will add to the entire sum (which is protected by a mutex)
 */

https://stackoverflow.com/questions/21323628/warning-cast-to-from-pointer-from-to-integer-of-different-size


## Debugger configs
launch.json
```json
"configurations": [
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/numerical_integration",
            "args": [
                "-t",
                "2",
                "-n",
                "2"
            ],
            "stopAtEntry": true,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                },
                {
                    "description": "Set Disassembly Flavor to Intel",
                    "text": "-gdb-set disassembly-flavor intel",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "build"
        }
    ]
```
tasks.json
```json
"tasks": [
        {
            "label": "build",
            "command": "make",
            "type": "shell",
            "args": [
            ],
            "group": "build",
            "presentation": {
                "reveal": "silent"
            },
            "problemMatcher": "$gcc"
        }
    ]
```