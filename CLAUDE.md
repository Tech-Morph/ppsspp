# CLAUDE.md — PPSSPP Developer Reference

## Project Purpose

PPSSPP is a cross-platform PSP (PlayStation Portable) emulator written in C++. It targets Android, Windows (Win32 + UWP), macOS, Linux, iOS, and Nintendo Switch. The goal is accurate, high-performance PSP emulation with hardware-accelerated rendering via Vulkan, OpenGL, and Direct3D 11/12.

---

## Tech Stack & Key Versions

| Technology | Role |
|---|---|
| **C++17** | Primary language for all emulation, GPU, UI, and platform code |
| **CMake** | Build system for all non-Android targets (`CMakeLists.txt` at root, ~100KB) |
| **Gradle / Kotlin DSL** | Android build system (`build.gradle.kts`, `settings.gradle.kts`) |
| **Vulkan** | Primary GPU rendering backend (preferred) |
| **OpenGL / GLES** | Fallback rendering backend (most platforms) |
| **Direct3D 11** | Windows-only rendering backend |
| **SDL2** | Linux/desktop windowing and input |
| **Qt** | Optional desktop frontend (Linux/macOS) |
| **FFmpeg** | Audio/video decoding (submodule at `ffmpeg/`) |
| **Python 3** | Test runner (`test.py`), tooling scripts |
| **GLSL / HLSL / MSL** | Shader languages per backend |

Build presets are defined in `CMakePresets.json`. The `b.sh` / `b-ios.sh` / `b-appstore.sh` shell scripts wrap common CMake invocations.

---

## Entry Points (Per Platform)

| Platform | Entry Point |
|---|---|
| **Android** | `android/src/…/NativeActivity` → JNI → `Core/` |
| **Windows (Win32)** | `Windows/main.cpp` |
| **Windows (UWP)** | `UWP/` |
| **Linux/macOS (SDL)** | `SDL/SDLMain.cpp` |
| **Linux/macOS (Qt)** | `Qt/main.cpp` |
| **iOS** | `ios/` Xcode project; build via `b-ios.sh` |
| **Headless (CI/test)** | `headless/Headless.cpp` — no display, used by `test.py` |
| **libretro core** | `libretro/libretro.cpp` |

---

## Directory Map

### Core Code (always relevant)

| Directory | Contents |
|---|---|
| `Core/` | PSP CPU emulation (MIPS dynarec/interpreter), memory, syscalls, HLE, savestate, loader |
| `GPU/` | GPU command processor, shader generation, all rendering backends (Vulkan/GL/D3D11) |
| `Common/` | Platform-agnostic utilities: threading, math, file I/O, logging, data structures, GPU abstractions |
| `UI/` | In-emulator UI (game browser, settings, overlays) — uses PPGE, the custom retained-mode UI framework |

### Platform Frontends

| Directory | Contents |
|---|---|
| `Windows/` | Win32 window management, D3D init, input, dialogs |
| `UWP/` | Universal Windows Platform variant |
| `SDL/` | SDL2 frontend for Linux and desktop macOS |
| `Qt/` | Qt-based frontend (optional, Linux/macOS) |
| `android/` | Android Studio project, JNI bridge, Java/Kotlin activity |
| `ios/` | Xcode project and iOS-specific shims |
| `macOS/` | macOS app bundle resources and native code |
| `libretro/` | RetroArch/libretro core interface |
| `headless/` | Headless runner for automated test execution |

### Supporting Directories

| Directory | Contents |
|---|---|
| `ext/` | Vendored third-party libraries (miniupnp, glslang, SPIRV-Cross, zstd, etc.) |
| `ffmpeg/` | Git submodule — custom ffmpeg build for PSP media decoding |
| `pspautotests/` | Git submodule — PSP hardware test ROMs used with headless runner |
| `assets/` | Shaders, fonts, lang files, UI atlases — **shipped with the binary** |
| `cmake/` | CMake helper modules |
| `scripts/` | Utility scripts (release packaging, symbol stripping, etc.) |
| `Tools/` | Developer tools (atlas generator, etc.) |
| `icons/` | App icons for all platforms |
| `source_assets/` | Raw/unprocessed assets (font sources, atlas inputs) |
| `ai_instructions/` | AI-assistant task context files (non-shipping) |
| `.github/` | CI workflow definitions (GitHub Actions) |

---

## Naming Conventions

- **Files**: `PascalCase.cpp` / `PascalCase.h` throughout all C++ directories. Some utility files use `snake_case.cpp`.
- **Classes/Structs**: `PascalCase` (e.g., `GPUCommon`, `MIPSState`, `VulkanRenderManager`)
- **Functions**: `PascalCase` for public methods; some internal helpers use `camelCase`
- **Macros/platform guards**: `PPSSPP_ARCH(X)`, `PPSSPP_PLATFORM(X)`, `PPSSPP_API(X)` — defined in `ppsspp_config.h`; **never** use raw `#ifdef __ANDROID__` etc. directly — always go through these macros
- **Shaders**: Live in `assets/shaders/`; GLSL files use `.fsh`/`.vsh`, HLSL use `.hlsl`

---

## Hard Constraints

1. **`ppsspp_config.h` is included by C, C++, and ASM** — it must never emit tokens or include other headers directly. Use only `#define` guards.
2. **All platform detection must go through `PPSSPP_PLATFORM()` / `PPSSPP_ARCH()` macros** from `ppsspp_config.h`. Do not use raw compiler-defined macros in feature code.
3. **`ext/` is vendored** — do not upgrade sublibraries independently without coordinating with CMakeLists.txt; many are patched.
4. **`ffmpeg/` and `pspautotests/` are git submodules** — always run `git submodule update --init --recursive` after cloning. Never edit files inside them directly.
5. **`assets/` is runtime-required** — it must be present next to the binary on all non-Android platforms. Android bundles it via the Gradle build.
6. **No STL exceptions in hot paths** — the codebase avoids exception-based error handling in emulation core for performance reasons.
7. **Android NDK builds use CMake via Gradle** — do not use ndk-build; the `android/` project drives CMake via the `externalNativeBuild` block.
8. **Shader changes require atlas rebuild** — UI texture atlases are pre-baked; use `build_fontatlas.sh` / `build_ppgeatlas.sh` when modifying UI assets in `source_assets/`.
9. **C++17 is the minimum standard** — do not use C++20 features without gating on compiler version checks.
10. **Windows ARM/ARM64 and UWP do not support OpenGL** — `PPSSPP_API_ANY_GL` is explicitly undefined for those targets; use `PPSSPP_API(ANY_GL)` to gate GL code.

---

## Build Quick Reference

```bash
# Linux/macOS (SDL)
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Or use the wrapper script
./b.sh

# Android (from repo root)
./gradlew assembleRelease

# iOS
./b-ios.sh

# Headless (for running test.py)
mkdir build-headless && cd build-headless
cmake .. -DHEADLESS=ON
make -j$(nproc)
cd ..
python3 test.py
```

---

## Key Files

| File | Purpose |
|---|---|
| `CMakeLists.txt` | Root build definition (~100KB, all targets) |
| `ppsspp_config.h` | Platform/arch/API detection macros — include everywhere |
| `Core/Config.cpp` | All user-configurable settings (g_Config) |
| `Core/System.cpp` | Top-level emulation lifecycle (init, shutdown, frame) |
| `GPU/GPUCommon.cpp` | Shared GPU command processor logic |
| `Common/GPU/thin3d.h` | Thin abstraction layer over Vulkan/GL/D3D11 |
| `history.md` | Full release changelog |
