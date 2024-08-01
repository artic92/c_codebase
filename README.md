# C Codebase

This repository contains all the C code I used while learning the C programming language. The code examples range from basic syntax to more complex projects, covering various aspects of C programming.

## Compiling C Code

To compile and run the C code, you can use a Docker container with the GCC compiler installed. This setup ensures a consistent development environment, avoiding any issues with local compiler versions or configurations.

### With Docker Container

1. **Prerequisites**:
   - Install Docker on your system.

2. **Building and Running Docker Container**:
   - Ensure Docker is installed and running on your system.
   - Open a terminal or command prompt.
   - Build the Docker image using the provided Dockerfile:
     ```bash
     docker build . --file .devcontainer/Dockerfile --tag c-compiler
     ```
   - Run the Docker container to compile the code (Linux):
     ```bash
     docker run --rm \
       --volume $(pwd):/workspace \
       c-compiler /bin/bash -c \
       "cd /workspace/template && make clean && make"
     ```
   - Run the Docker container to compile the code (Windows):
     ```bash
      docker run --rm `
        --volume ${PWD}:/workspace `
        c-compiler /bin/bash -c `
        "cd /workspace/template && make clean && make"
     ```

You can execute the container and do the compilation steps manually.

To execute the container (Linux):
```bash
    docker run --rm -it -v $(PWD):/workspace c-compiler
```

To execute the container (Windows):
```bash
    docker run --rm -it -v ${PWD}:/workspace c-compiler
```

When you first run Docker with the provided configuration, it may take more time as the Docker engine builds the container based on the Dockerfile in the `.devcontainer` directory. Subsequent runs should be faster as Docker caches the built container.

### Without Docker Container

1. **Prerequisites**:
   - Install `gcc` and `make` utilities on your system (Debian):
     ```bash
     sudo apt-get -y install gcc make
     ```

2. **Compilation**:
   - Open a terminal or command prompt.
   - Navigate to the project directory you want to compile
     ```bash
     cd template/
     ```
   - Run the following command to compile the code:
     ```bash
     make
     ```
   - Execute the code:
     ```bash
     ./hellomake
     ```