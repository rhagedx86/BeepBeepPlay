# BeepBeepPlay DLL

Minimal Windows DLL for asynchronous audio playback with adjustable volume.

This DLL exists to provide reliable per-sound volume control for applications
with limited native audio support.

---

## Function

```cpp
int BeepBeepPlay(const char* path, float volume);
```

### Parameters

- **path** — UTF-8 path to a `.wav` or `.mp3` file
- **volume** — float from `0.0` to `1.0`

The function returns immediately and plays audio on a detached thread.

---

## Requirements

- Windows
- DirectShow
- Visual Studio / MSVC
- Windows SDK
- Links against `strmiids.lib`

---

## Build

Build as a Win32 or x64 DLL using Visual Studio.  
No external dependencies required beyond the Windows SDK.

---

## Notes

- Uses DirectShow for playback
- Volume curve is UI-tuned (not physically linear)
- Designed for lightweight event sounds
- Playback is threaded and non-blocking

---

## License

GNU GPL v3 (or later).  
No warranty provided.

**Author:** Kyle Rhaged  
**GitHub:** [Rhagedx86]([https://github.com/rhagedx86])

If you enjoy this plugin and want to support my work, you can buy me a coffee here: [Ko-fi](https://ko-fi.com/rhaged)
