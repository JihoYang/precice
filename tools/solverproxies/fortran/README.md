#Compilation

Simply type `scons`. Assumption: preCICE is available as a shared library in your `LD_LIBRARY_PATH`.

#Run

You can test the proxy by coupling two instances with each other. Open two terminals and run
 * `./SolverProxy precice-config.xml SolverOne MeshOne`
 * `./SolverProxy precice-config.xml SolverTwo MeshTwo`

#Next Steps

If you want to couple any other solver against the proxy be sure to adjust the preCICE configuration (participant names, mesh names, data names etc.) to the needs of your solver, compare our [step-by-step guide for new adapters](https://github.com/precice/precice/wiki/Adapter-Example).
 
