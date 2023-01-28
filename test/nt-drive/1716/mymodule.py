#!/usr/bin/env python

class TestClass:
    def __init__(self):
        pass

    def print_string(self, string):
        print(string)


if __name__ == "__main__":
    # We're a module, never run anything here
    pass
else:
    # Run things on import here
    print("module Hello world")
    pass
