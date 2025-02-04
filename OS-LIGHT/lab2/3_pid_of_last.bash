#!/bin/bash

ps aux | tail -n 1 | awk '{print $2 " " $9 " " $11}'
