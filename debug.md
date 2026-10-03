# Debugging in CLion with BlastEm

Source-level debugging of a project ROM using BlastEm's GDB stub, `gdb-multiarch` and CLion's Remote Debug.

## One-time setup

```sh
sudo apt install blastem socat gdb-multiarch
```

CLion toolchain (Settings → Build, Execution, Deployment → Toolchains → + → System):
- Name: `m68k-gdb`
- Debugger: Custom GDB executable → `/usr/bin/gdb-multiarch`
- Ignore compiler/CMake warnings, only the debugger matters.

## Per project

Run/Debug Configurations → + → **Remote Debug**:

| Field         | Value                                                 |
|---------------|-------------------------------------------------------|
| Toolchain     | `m68k-gdb`                                            |
| Connection    | `localhost:1234`                                      |
| Symbol file   | `$PROJECT_DIR$/projects/<project>/out/debug/rom.out`  |
| Sysroot       | empty                                                 |
| Path mappings | empty                                                 |

## Debug session

1. Build debug ROM (from `projects/<project>`):
   ```sh
   make debug CONVSYM=true
   ```
   `CONVSYM=true` skips symbol injection. marsdev has no convsym, so without it `make debug` fails before writing `out/<project>.bin`.
   Release and debug both write `out/<project>.bin`, so rebuild debug if you ran a release build since.

2. Start the GDB bridge (from `projects/<project>`, leave it running):
   ```sh
   socat TCP-LISTEN:1234,bind=127.0.0.1,reuseaddr,fork EXEC:"blastem -D out/<project>.bin"
   ```
   BlastEm's stub talks over stdin/stdout. socat exposes it on a TCP port. `fork` spawns a fresh BlastEm per connection.

3. Set breakpoints in **this project's** sources, then Debug the Remote Debug config.

4. Execution halts at reset (`_Entry_Point`). Press Resume to run to your breakpoint.

5. Stopping the session kills BlastEm (socat logs `vKill ... not implemented`, harmless). Ctrl+C socat when done.

## Troubleshooting

- **Connection timed out**: socat isn't running or port is taken. Check `ss -ltnp | grep 1234` and `pgrep -a 'socat|blastem'`, kill leftovers.
- **Breakpoints never hit**: ROM is stale (release build) or breakpoints are in another project's `main.c`.
- **`<optimized out>` locals**: debug builds use `-O1`. Mark vars `volatile` or use `-O0` in `makefile.gen`.
- **Sanity check without CLion**:
  ```sh
  gdb-multiarch out/debug/rom.out -ex "target remote | blastem -D out/<project>.bin" -ex "break main" -ex "continue"
  ```
