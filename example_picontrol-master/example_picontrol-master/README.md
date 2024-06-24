# Example_PIControl

The PI Controller taken from the Polyspace training materials, slightly changed for use with polyspace-configure.

## Use with polyspace-configure
This project is changed so that is works with polyspace configure. There are currently two ways to automatically set up a Polspace analysis from the build system:
 1. sniffing the build process
 2. using the compilation database

Note that in both cases, polyspace-configure may still call the compiler to "query" some propertes, such as integer widths or defines. This step can however be prevented by using the "compiler info cache" (Polyspace remembers the result of these queries and stores them in a database residing in the default MathWorks data folder, e.g., `~/.matlab/R2020b/Polyspace/compiler-cache`), which is enabled by default. To query your compiler info cache:
```
polyspace-configure -dump-compiler-cache  # unofficial option
polyspace-configure -clear-compiler-cache  # forget all
polyspace-configure -compiler-cache-directory <path>  # change location of cache
```

### Sniffing the build process
For this way, use the Makefile, e.g.:
```
make clean  # important for polyspace to "see" all units
polyspace-configure <options> make
```
for your convenience, you may use the script `run-polyspace-configure-as-sniffer.sh`.

### Using the compilation database
This assumes that some other user has already compiled the program at some point, and left you the compilation database (JSON). This can, e.g., be done with cmake as follows:
```
mkdir -p build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .
make
```
For your convenience, simply call the script `run-cmake.sh` to execute these steps.

Now, Polyspace-configure can just read the produced JSON file and no longer needs to compile again to set up the analysis project. Use as follows:
```
polyspace-configure -compilation-database compile_commands.json
```
For your convenience, you may use the script `run-polyspace-configure-from-database.sh`.

## CI/CD pipeline in gitlab
Additionally, this gitlab project has a CI/CD pipeline configured, which automatically runs Polyspace on each merge request, and annotates the diffs with findings.
Towards that:
 * this gitlab project has a pipeline defined in the file .gitlab-ci.yml
 * that file contains the scripts to be executed on each merge request:
   * runs polyspace server
   * back-annotates the findings to the merge request via REST API

Technically, it works as follows:
 * a "gitlab runner" is waiting on a distant server (currently gnb-access-dtst.mathworks.com) for a trigger from gitlab
 * the runner launches a docker instance based on the image specified in the YAML file
 * towards that, the machine which hosts the runner must have a special polyspace docker image. See https://insidelabs-git.mathworks.com/mbecker/polyspace-dockerized for the Dockerfile to build the image
