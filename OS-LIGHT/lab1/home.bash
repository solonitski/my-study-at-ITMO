#!/bin/bash

pwd | grep -q "^$HOME" && echo "Home directory path: $HOME" && exit 0 || echo "Error: Script is not run from the home directory" && exit 1