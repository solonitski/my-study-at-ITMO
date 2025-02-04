#!/bin/bash

[[ ! -p my_pipe ]] && mkfifo my_pipe

bash ./5_handler.bash & bash ./5_producer.bash

