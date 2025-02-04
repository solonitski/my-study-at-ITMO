#!/bin/bash

at now +1 minutes -f ./1_datetime.bash

tail -f ~/report &
