#!/bin/bash

BASE_DIR="$HOME/.queue"

PIPE="$BASE_DIR/queue_pipe"
RUNNING_FILE="$BASE_DIR/running_tasks"
PENDING_FILE="$BASE_DIR/pending_tasks"
PID_FILE="$BASE_DIR/daemon.pid"
LOG_FILE="$BASE_DIR/queue.log"

MAX_CONCURRENT=2

initialize_environment() {
    if [ ! -d "$BASE_DIR" ]; then
        mkdir -p "$BASE_DIR"
        if [ $? -ne 0 ]; then
            echo "Error: Failed to create base directory at $BASE_DIR."
            exit 1
        fi
    fi

    if [[ ! -p "$PIPE" ]]; then
        mkfifo "$PIPE"
        if [ $? -ne 0 ]; then
            echo "Error: Failed to create named pipe at $PIPE."
            exit 1
        fi
    fi

    touch "$RUNNING_FILE" "$PENDING_FILE" "$LOG_FILE"
    if [ $? -ne 0 ]; then
        echo "Error: Failed to create necessary state files in $BASE_DIR."
        exit 1
    fi
}

start_daemon() {
    if [ -f "$PID_FILE" ] && ps -p "$(cat "$PID_FILE")" > /dev/null 2>&1; then
        echo "Daemon is already running with PID $(cat "$PID_FILE")."
        return
    fi

    (
        cd "$BASE_DIR" || exit

        while true; do
            if [ -s "$RUNNING_FILE" ]; then
                TEMP_RUNNING="$BASE_DIR/temp_running"
                > "$TEMP_RUNNING"

                while read -r line; do
                    script=$(echo "$line" | awk '{print $1}')
                    pid=$(echo "$line" | awk '{print $2}')
                    if kill -0 "$pid" 2>/dev/null; then
                        echo "$line" >> "$TEMP_RUNNING"
                    else
                        echo "$(date '+%Y-%m-%d %H:%M:%S') Completed: $script (PID: $pid)" >> "$LOG_FILE"
                    fi
                done < "$RUNNING_FILE"

                mv "$TEMP_RUNNING" "$RUNNING_FILE"
            fi

            current_running=$(wc -l < "$RUNNING_FILE" | tr -d ' ')
            while [ "$current_running" -lt "$MAX_CONCURRENT" ] && [ -s "$PENDING_FILE" ]; do
                read -r next_task < "$PENDING_FILE"

                sed -i '1d' "$PENDING_FILE"

                if [ ! -f "$next_task" ]; then
                    echo "$(date '+%Y-%m-%d %H:%M:%S') Error: Script not found - $next_task" >> "$LOG_FILE"
                    continue
                fi

                bash "$next_task" &
                pid=$!
                echo "$next_task $pid" >> "$RUNNING_FILE"
                echo "$(date '+%Y-%m-%d %H:%M:%S') Started: $next_task (PID: $pid)" >> "$LOG_FILE"

                ((current_running++))
            done

            if read -t 1 task < "$PIPE"; then
                if grep -Fxq "$task" "$PENDING_FILE" || grep -Fxq "$task" <(awk '{print $1}' "$RUNNING_FILE"); then
                    echo "$(date '+%Y-%m-%d %H:%M:%S') Warning: Task already in queue or running - $task" >> "$LOG_FILE"
                else
                    echo "$task" >> "$PENDING_FILE"
                    echo "$(date '+%Y-%m-%d %H:%M:%S') Added task: $task" >> "$LOG_FILE"
                fi
            fi

            sleep 0.1
        done
    ) >> "$LOG_FILE" 2>&1 &

    echo $! > "$PID_FILE"
    echo "Daemon started with PID $!"
    echo "$(date '+%Y-%m-%d %H:%M:%S') Daemon started with PID $!" >> "$LOG_FILE"
}

stop_daemon() {
    if [ -f "$PID_FILE" ]; then
        pid=$(cat "$PID_FILE")
        if ps -p "$pid" > /dev/null 2>&1; then
            kill "$pid"
            rm "$PID_FILE"
            echo "Daemon stopped."
            echo "$(date '+%Y-%m-%d %H:%M:%S') Daemon stopped." >> "$LOG_FILE"
        else
            echo "Daemon is not running, but PID file exists. Removing PID file."
            echo "$(date '+%Y-%m-%d %H:%M:%S') Daemon is not running, but PID file exists. Removing PID file." >> "$LOG_FILE"
            rm "$PID_FILE"
        fi
    else
        echo "Daemon is not running."
    fi
}

show_status() {
    echo "Currently running tasks:"
    if [ -s "$RUNNING_FILE" ]; then
        while read -r line; do
            script=$(echo "$line" | awk '{print $1}')
            pid=$(echo "$line" | awk '{print $2}')
            if kill -0 "$pid" 2>/dev/null; then
                echo " - $script (PID: $pid)"
            else
                echo " - $script (PID: $pid) [Completed]"
            fi
        done < "$RUNNING_FILE"
    else
        echo " - No running tasks."
    fi

    echo ""
    echo "Pending tasks in the queue:"
    if [ -s "$PENDING_FILE" ]; then
        nl -w2 -s'. ' "$PENDING_FILE"
    else
        echo " - Queue is empty."
    fi
}

show_logs() {
    if [ -f "$LOG_FILE" ]; then
        if command -v gnome-terminal > /dev/null 2>&1; then
            gnome-terminal -- bash -c "tail -f \"$LOG_FILE\"; exec bash"
        elif command -v konsole > /dev/null 2>&1; then
            konsole -e bash -c "tail -f \"$LOG_FILE\"; exec bash"
        else
            echo "Error: No supported terminal emulator found (gnome-terminal, konsole). Please install one to use the 'log' command."
            exit 1
        fi
    else
        echo "Log file does not exist."
    fi
}

reset_queue() {
    echo "Resetting the queue system..."

    if [ -f "$PID_FILE" ]; then
        stop_daemon
    fi

    rm -f "$RUNNING_FILE" "$PENDING_FILE" "$LOG_FILE" "$PIPE"

    echo "Cleared RUNNING_FILE, PENDING_FILE, LOG_FILE, and PIPE."

    initialize_environment

    echo "Environment re-initialized."

    start_daemon

    echo "Queue system has been reset."
}

initialize_environment

start_daemon

case "$1" in
    add)
        if [ -z "$2" ]; then
            echo "Usage: $0 add <script.sh>"
            exit 1
        fi
        script_to_add="$2"

        if [[ "$script_to_add" != /* ]]; then
            script_to_add="$PWD/$script_to_add"
        fi
        if [ ! -f "$script_to_add" ]; then
            echo "Error: Script $script_to_add not found."
            exit 1
        fi
        
        if grep -Fxq "$script_to_add" "$PENDING_FILE" || grep -Fxq "$script_to_add" <(awk '{print $1}' "$RUNNING_FILE"); then
            echo "Warning: Script is already in the queue or running."
            exit 1
        fi
        echo "$script_to_add" > "$PIPE"
        ;;
    status)
        show_status
        ;;
    stop)
        stop_daemon
        ;;
    log)
        show_logs
        ;;
    reset)
        reset_queue
        ;;
    *)
        echo "Usage: $0 {add <script.sh>|status|stop|log|reset}"
        exit 1
        ;;
esac

