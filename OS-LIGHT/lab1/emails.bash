#!/bin/bash

    emails=$(grep -ERoh --binary-files=without-match "[a-zA-Z.+-]+@[a-zA-Z-]+\.[a-zA-Z.-]+" "/etc" | uniq | paste -sd, -)

    echo "${emails}," > etc_emails.lst
