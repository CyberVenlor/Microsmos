#!/usr/bin/env python
import os
import sys

# Define the environment for SCons
env = SConscript("godot-cpp/SConstruct")

# Function to recursively collect all .cpp files and include directories in a directory
def collect_sources_and_includes(root_dir):
    sources = []
    includes = set()
    
    for dirpath, dirnames, filenames in os.walk(root_dir):
        # Collect .cpp files
        for filename in filenames:
            if filename.endswith(".cpp"):
                sources.append(os.path.join(dirpath, filename))
        
        # Collect include directories
        includes.add(os.path.abspath(dirpath))
    
    return sources, includes

# Collect all .cpp files and include directories from src/
sources, include_dirs = collect_sources_and_includes("src/")

# Append all collected include directories to CPPPATH
env.Append(CPPPATH=list(include_dirs))

# Create the shared library based on the platform
if env["platform"] == "macos":
    library_path = "bin/Microsmos{}.{}.framework/{}.{}".format(
        env["platform"], env["target"], env["platform"], env["target"]
    )
else:
    library_path = "bin/Microsmos{}{}".format(env["suffix"], env["SHLIBSUFFIX"])

# Build the shared library with the collected sources
library = env.SharedLibrary(library_path, source=sources)

# Set the default target
Default(library)