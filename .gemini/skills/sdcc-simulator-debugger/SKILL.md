---
name: sdcc-simulator-debugger
description: Guidance for using and extending the SDCC simulator wrapper (scripts/simulator-sdcc). Use when you need to run 8051 or STM8 simulations, set multiple breakpoints, execute custom debugger commands, or troubleshoot simulator interaction issues.
---

# SDCC Simulator Debugger Skill

This skill provides specialized knowledge for interacting with the `scripts/simulator-sdcc` wrapper, which orchestrates various `ucsim_*` debuggers for SDCC targets.

## Core Features

- **Multi-Target Support**: Supports 8051 (`z51`), DS390 (`--ds390`), and STM8 (`--stm8`).
- **Multi-Breakpoint Support**: Set multiple breakpoints using `-b <name|addr>`.
- **Command Association**: Associate semicolon-separated debugger commands with each breakpoint using `-c "cmd1; cmd2"`.
- **Clean Output**: Automatically strips debugger noise using a callback system while preserving program stdout.
- **Timeout Management**: Configurable execution timeout via `-t <seconds>`.

## Common Workflows

### 1. Simple Trace (8051)
Set a breakpoint at a function and print a variable:
```bash
./scripts/simulator-sdcc -b my_func -c "print my_var" my_program.ihx
```

### 2. STM8 Simulation
Run a simulation for STM8 target:
```bash
./scripts/simulator-sdcc --stm8 my_program.ihx
```

### 3. Multi-point Inspection
Inspect state at different stages:
```bash
./scripts/simulator-sdcc \
    -b init -c "print config_reg" \
    -b main_loop -c "print loop_count; step; print loop_count" \
    my_program.ihx
```

### 4. Memory Dump at Breakpoint
Inspect memory contents when a specific function is reached:
```bash
./scripts/simulator-sdcc -v -b yio_printctx_raw_write -c "dump iram 0x80 0xff" my_program.ihx
```

### 5. Verbose Troubleshooting
If interaction fails, use `-v` to see raw debugger traffic:
```bash
./scripts/simulator-sdcc -v -b critical_section my_program.ihx
```

## Developer Notes

- **SimulatorState.print**: Always use `state.print(cmd)` to send commands to the debugger. It handles logging and flushing correctly.
- **Callback System**: Output filtering is handled by `DebuggerCallback` subclasses. To ignore new debugger noise, add a pattern to `IgnoreCallback`.
- **Breakpoint Detection**: The `BreakpointDispatcher` uses regex to catch debugger "Stop at" messages and routes them to specific handlers.
