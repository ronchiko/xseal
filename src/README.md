XSeal Project Architecture
===

The project can divided into 3 sub-categories: the core, frontends & backends.

Each compilation of the engine must have exactly one frontend, backend and core.

* Core: The core is responsible for running the actual logic of the engine.
* Frontends: Frontends are responsible for linking the engine with the player (showing a screen, reading inputs, etc...).
* Backends: The backends are resposible for managing the low level interaction against the graphics library. 

Currently we support 2 frontends: `glfw` (desktop) and `ems` (webgl - empscripten).
We also support a single backend: `gl` (Both openGL 4 and ES3)

The Core can interact with the backend and frontend by using the `seal::api` which is defined in `include/seal/api`.
