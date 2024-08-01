# Use the GCC image as the base image
FROM gcc:latest

# Create a volume at /workspace
VOLUME [ "/workspace" ]

# Create a user named 'dev' with a home directory
RUN useradd -m dev

# Set the working directory to /workspace
WORKDIR /workspace

# Switch to the 'dev' user
USER dev

# By default, the container will just open a shell as the 'dev' user in /workspace
CMD [ "bash" ]