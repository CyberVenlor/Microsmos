#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=["src/"])

# Function to recursively collect all .cpp files in a directory
def collect_sources(root_dir):
    sources = []
    for dirpath, dirnames, filenames in os.walk(root_dir):
        for filename in filenames:
            if filename.endswith(".cpp"):
                sources.append(os.path.join(dirpath, filename))
    return sources

# Collect all .cpp files in src/ and its subdirectories
sources = collect_sources("src/")

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "bin/Microsmos{}.{}.framework/{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "bin/Microsmos{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)