# Hephaestus

Hephaestus is a simple, cross platform game engine written in C.

## Features

- Cross platform (Windows, OSX, Linux, FreeBSD).
- Lua scripts for control over the engine.

## Installation

Currently, the only way to install Hephaestus is by building it from source, but
binary releases will be made available in the future once the project reaches a
stable phase.

### Dependencies

Make sure you have the `REQUIRED` dependencies installed first before proceeding.

- C Compiler (REQUIRED)
- Meson (REQUIRED)
- Git (REQUIRED)
- raylib (recommended)
- Lua 5.4.6 (recommended)
- libarchive (recommended)
- jansson (recommended)

If the `recommended` dependencies are not installed, the Meson Wrap system will automatically download the sources for them.

### Building from source

Run `./build.sh` to automatically build the sources.

You can manually build/configure through the following steps:

`meson setup builddir`  
`meson configure builddir` (optional)  
`meson compile -C builddir`  

> Remember to update your submodules before building!

### Binary Releases

Binary releases will soon be available.

## Usage/Examples

Controls:

- Escape: Closes window
- F5: Unload currently loaded file.

To load a script, just drag and drop onto the engine's window.

Here is the format of a loadable script:

```
function init() -- Called once loaded.
    print("Hello from Lua!")
    return
end

function loop() -- Called every frame.
    return
end

function exit() -- Called on cleanup.
    print("Goodbye!")
    return
end
```

## Support

For support, email <mailto:project.gunix@skiff.com>.

## Roadmap

- Create a custom logging suite.
- Create asset management utilities (textures, audio, etc).
- Combine archive utils, Lua, assets, and JSON/YAML configs to create "game packs".
- Eventually remove raylib as a dependency:
  1. Add support for Nuklear/cimgui.
  2. Add support for alternative model loading libraries.
  3. Use libsndfile with OpenAL or PortAudio to create a custom audio engine.
  4. Create a custom OpenGL 4 instance through GLFW + GLAD.
  5. Vulkan??

## Contributing

Contributions are always welcome!  
See `CONTRIBUTING.md` for ways to get started.  
Please adhere to this project's `code of conduct`.

## License

[BSD 3-Clause](https://choosealicense.com/licenses/bsd-3-clause/)

