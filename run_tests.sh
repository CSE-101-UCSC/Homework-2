#!/usr/bin/env bash
# Local test runner for CSE 101 Programming Assignment 2.
#
#   make                        compile first (rerun after code changes)
#   ./run_tests.sh              run every test with the existing executable
#   ./run_tests.sh 05           run only tests whose name contains "05"
#   ./run_tests.sh filter       run only tests whose name contains "filter"
#
# Each test is a folder in tests/ containing:
#   input.txt       the input file (left out when the test checks a missing file)
#   cmd             the arguments passed to ./FlightManager
#   expected.txt    the exact expected combined output
#   expected_exit   (optional) the expected exit status; 0 if missing
#   description     what the test checks
#
# Standard output and standard error are merged into tests/<test>/output.txt.
# These are the visible tests only. Gradescope also runs hidden tests.
#
# Works in a Codespace, on macOS (including its built-in bash 3.2), and on
# Windows in WSL or Git Bash.

cd "$(dirname "$0")" || exit 1

TIME_LIMIT_SECONDS=5
MAX_FEEDBACK_LINES=40

# Stop a test that runs too long. macOS has no `timeout` command, so fall back
# to gtimeout (Homebrew coreutils) or perl, which every Mac and Git Bash has.
if command -v timeout >/dev/null 2>&1; then
    TIMEOUT_COMMAND="timeout $TIME_LIMIT_SECONDS"
    TIMEOUT_EXIT_CODE=124
elif command -v gtimeout >/dev/null 2>&1; then
    TIMEOUT_COMMAND="gtimeout $TIME_LIMIT_SECONDS"
    TIMEOUT_EXIT_CODE=124
elif command -v perl >/dev/null 2>&1; then
    TIMEOUT_COMMAND="perl -e 'alarm shift; exec @ARGV' $TIME_LIMIT_SECONDS"
    TIMEOUT_EXIT_CODE=142
else
    TIMEOUT_COMMAND=""
    TIMEOUT_EXIT_CODE=124
    echo "Note: no timeout command found, so tests run without a time limit."
fi

if [ -t 1 ]; then
    RED=$'\033[31m'
    GREEN=$'\033[32m'
    YELLOW=$'\033[33m'
    BOLD=$'\033[1m'
    RESET=$'\033[0m'
else
    RED=""
    GREEN=""
    YELLOW=""
    BOLD=""
    RESET=""
fi

testNames=()
for testDirectory in tests/*/; do
    testNames+=("$(basename "$testDirectory")")
done

nameFilter="$1"

if [ -x ./FlightManager ]; then
    executablePath="../../FlightManager"
elif [ -x ./FlightManager.exe ]; then
    executablePath="../../FlightManager.exe"
else
    echo "Program not found or not executable. Run 'make' first, then './run_tests.sh'."
    exit 1
fi

passedCount=0
failedCount=0
executedCount=0
failedTestNames=()
# Per-part totals, kept in parallel arrays (bash 3.2 has no associative arrays).
groupNames=()
groupPassedCounts=()
groupTestCounts=()

for testName in "${testNames[@]}"; do
    if [ -n "$nameFilter" ] && [[ "$testName" != *"$nameFilter"* ]]; then
        continue
    fi
    testDirectory="tests/$testName"
    executedCount=$((executedCount + 1))

    groupName="$(tr -d '\r' < "$testDirectory/part" 2>/dev/null)"
    [ -n "$groupName" ] || groupName="Other"
    groupIndex=0
    while [ $groupIndex -lt ${#groupNames[@]} ] &&
          [ "${groupNames[$groupIndex]}" != "$groupName" ]; do
        groupIndex=$((groupIndex + 1))
    done
    if [ $groupIndex -eq ${#groupNames[@]} ]; then
        groupNames[$groupIndex]="$groupName"
        groupPassedCounts[$groupIndex]=0
        groupTestCounts[$groupIndex]=0
    fi
    groupTestCounts[$groupIndex]=$((groupTestCounts[$groupIndex] + 1))

    testArguments="$(tr -d '\r\n' < "$testDirectory/cmd")"
    expectedExitCode="$(cat "$testDirectory/expected_exit" 2>/dev/null || echo 0)"
    outputFile="$testDirectory/output.txt"

    # Run from inside the test folder so "input.txt" refers to that test's file.
    (cd "$testDirectory" && eval "$TIMEOUT_COMMAND $executablePath $testArguments") > "$outputFile" 2>&1
    exitCode=$?

    failureReason=""
    if [ -n "$TIMEOUT_COMMAND" ] && [ $exitCode -eq $TIMEOUT_EXIT_CODE ]; then
        failureReason="timed out after ${TIME_LIMIT_SECONDS}s (infinite loop?)"
    elif [ $exitCode -ge 128 ]; then
        failureReason="crashed (signal $((exitCode - 128)); often a null or deleted pointer)"
    elif [ "$exitCode" != "$expectedExitCode" ]; then
        failureReason="exit status $exitCode, expected $expectedExitCode"
    elif ! diff -q --strip-trailing-cr "$testDirectory/expected.txt" "$outputFile" >/dev/null; then
        failureReason="output differs from expected"
    fi

    if [ -z "$failureReason" ]; then
        passedCount=$((passedCount + 1))
        groupPassedCounts[$groupIndex]=$((groupPassedCounts[$groupIndex] + 1))
        echo "${GREEN}PASS${RESET}  $testName"
    else
        failedCount=$((failedCount + 1))
        failedTestNames+=("$testName")
        echo "${RED}FAIL${RESET}  $testName: $failureReason"
        echo "      $(cat "$testDirectory/description" 2>/dev/null)"
        echo "      command: ./FlightManager $testArguments"
        if [ "$failureReason" = "output differs from expected" ]; then
            echo "      ${YELLOW}--- lines starting with - are expected, + are your output${RESET}"
            diff -u --strip-trailing-cr --label "expected" --label "your output" \
                "$testDirectory/expected.txt" "$outputFile" | head -n $MAX_FEEDBACK_LINES | sed 's/^/      /'
        fi
        if [ "$exitCode" != "0" ] && [ -s "$outputFile" ]; then
            echo "      diagnostic (combined output; also saved in $outputFile):"
            head -n $MAX_FEEDBACK_LINES "$outputFile" | sed 's/^/      /'
        fi
        echo
    fi
done

if [ $executedCount -eq 0 ]; then
    echo "No tests match \"$nameFilter\". Check the tests/ directory for test names."
    exit 1
fi

echo
echo "${BOLD}Summary${RESET}"
groupIndex=0
while [ $groupIndex -lt ${#groupNames[@]} ]; do
    printf "  %-22s %d/%d passed\n" \
        "${groupNames[$groupIndex]}" \
        "${groupPassedCounts[$groupIndex]}" \
        "${groupTestCounts[$groupIndex]}"
    groupIndex=$((groupIndex + 1))
done
echo "  ----------------------------------"
printf "  %-22s %d/%d passed\n" "Total" "$passedCount" "$executedCount"
if [ -n "$nameFilter" ]; then
    printf "Selected score: %d/%d points\n" "$passedCount" "$executedCount"
else
    printf "Public score: %d/%d points\n" "$passedCount" "$executedCount"
fi
if [ $failedCount -gt 0 ]; then
    echo "Failed cases:"
    for failedTestName in "${failedTestNames[@]}"; do
        echo "  $failedTestName"
    done
else
    echo "Failed cases: none"
fi
echo
if [ $failedCount -eq 0 ]; then
    if [ -n "$nameFilter" ]; then
        echo "${GREEN}${BOLD}All $executedCount selected tests passed.${RESET} Run ./run_tests.sh with no arguments to run every test."
    else
        echo "${GREEN}${BOLD}All $executedCount tests passed!${RESET}"
    fi
else
    echo "${RED}${BOLD}$failedCount of $executedCount tests failed.${RESET} Scroll up to see the differences for each failed test."
fi
echo "Combined outputs: tests/<case>/output.txt."
if [ $failedCount -eq 0 ] && [ -z "$nameFilter" ]; then
    echo "Before submitting: fill in citations.md, run make clean, and push your work to your GitHub repository."
fi

[ $failedCount -eq 0 ]
