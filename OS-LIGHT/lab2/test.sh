#!/bin/bash

function run_test() {
    local description="$1"
    local command="$2"
    local expected_code="$3"

    echo "Running test: $description"
    eval "$command"
    local exit_code=$?

    if [ "$exit_code" -eq "$expected_code" ]; then
        echo "[PASS] $description"
    else
        echo "[FAIL] $description (Expected $expected_code, got $exit_code)"
    fi
    echo
}

mock_scripts_dir="mock_scripts"
mkdir -p "$mock_scripts_dir"

cat <<'EOF' > "$mock_scripts_dir/low_read.sh"
#!/bin/bash
sleep 2
count=0
while [ $count -lt 500 ]; do
    head -c 100 < /dev/urandom > /dev/null
    count=$((count + 100))
    sleep 0.05
done
EOF
chmod +x "$mock_scripts_dir/low_read.sh"

cat <<'EOF' > "$mock_scripts_dir/high_read.sh"
#!/bin/bash
count=0
while [ $count -lt 2000000 ]; do
    head -c 100000 < /dev/urandom > /dev/null
    count=$((count + 100000))
    sleep 0.05
done
EOF
chmod +x "$mock_scripts_dir/high_read.sh"

cat <<'EOF' > "$mock_scripts_dir/quick_finish.sh"
#!/bin/bash
sleep 2
EOF
chmod +x "$mock_scripts_dir/quick_finish.sh"

cat <<'EOF' > "$mock_scripts_dir/infinite.sh"
#!/bin/bash
while true; do :; done
EOF
chmod +x "$mock_scripts_dir/infinite.sh"

run_test "Script reads below limit" "./mod.sh 1000000 $mock_scripts_dir/low_read.sh" 0
run_test "Script reads above limit" "./mod.sh 1000000 $mock_scripts_dir/high_read.sh" 2
run_test "Script finishes quickly without exceeding limit" "./mod.sh 1000000 $mock_scripts_dir/quick_finish.sh" 0

echo "Running test: Script exceeds time limit but no read limit"
./mod.sh 1000000 "$mock_scripts_dir/infinite.sh" &
MOD_PID=$!
sleep 65
if kill -0 "$MOD_PID" 2>/dev/null; then
    echo "Script did not terminate as expected."
    kill -9 "$MOD_PID" 2>/dev/null
    wait "$MOD_PID" 2>/dev/null
    exit_code=1
else
    wait "$MOD_PID" 2>/dev/null
    exit_code=$?
fi

if [ "$exit_code" -eq 2 ]; then
    echo "[PASS] Script exceeds time limit but no read limit"
else
    echo "[FAIL] Script exceeds time limit but no read limit (Expected 2, got $exit_code)"
fi
echo

rm -rf "$mock_scripts_dir"

