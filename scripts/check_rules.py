# Basic file/interface checks for Homework 2.
# Run by make before compiling. This does not test AVL correctness.
# Gradescope always uses the teacher-provided fixed files and this script.
from pathlib import Path
import hashlib
import re
import sys


# The script lives in scripts/; go up twice to find the assignment directory.
# Resolve from the script location, independently of the current directory.
root = Path(__file__).resolve().parent.parent

# Collect problems so students can see all detected errors in one report.
errors = []


# 1. Check that teacher-provided source files and headers are unchanged.
# These SHA-256 fingerprints describe the original file contents.
# avl.cpp is intentionally omitted because students must edit it.
EXPECTED = {
    'main.cpp': 'cfd82f8bb12ddc88f8846414348ba3f894733bc6efba3f525f70fe9c35bb5f3b',
    'flights.cpp': 'b0871dd5feab817fd468a5b05f69469d9463c34ae74e1e1bcc81da9ec6258384',
    'flights.h': 'a5acded42f2e88c3fe9c8b13412c6f2bad0646de7258f70165e1d7369c711ef4',
    'avl.h': '8f8af0e805fc993f2171d64377855dfa6a06a55eb21dcfee54e610cec4f7e485',
    'types.h': '03e88dc76887066790f5338eb3c970ba1d86994eb8e560d8d7c43f5554e111db',
}

for (name, expected) in EXPECTED.items():
    p = root / name
    # Normalize CRLF to LF before hashing, so platform line endings are allowed.
    # Short-circuiting avoids reading a file when it does not exist.
    if (
        not p.exists()
        or hashlib.sha256(
            p.read_bytes().replace(b'\r\n', b'\n')
        ).hexdigest() != expected
    ):
        errors.append(name + ' changed or missing; restore the provided file.')


# 2. Reject additional source/header files in the assignment root.
# Ordinary files such as citations.md and output.txt are unaffected.
# Only avl.cpp is used as student source during grading.
allowed = {
    'main.cpp',
    'flights.cpp',
    'avl.cpp',
    'flights.h',
    'avl.h',
    'types.h',
}

for f in root.iterdir():
    if (
        f.suffix in {'.cpp', '.cc', '.cxx', '.c', '.h', '.hpp'}
        and f.name not in allowed
    ):
        errors.append('Extra source file: ' + f.name)


# 3. Require the student implementation file and check for a main() entry.
# The program entry point is already supplied in the fixed main.cpp.
p = root / 'avl.cpp'
if not p.exists():
    errors.append('avl.cpp missing')
else:
    raw = p.read_text()

    # Strip comments, then string contents, to reduce false matches in text.
    # These regular expressions are basic checks, not a complete C++ parser.
    code = re.sub('/\\*.*?\\*/|//[^\\n]*', '', raw, flags=re.S)
    code = re.sub('"(?:\\\\.|[^"\\\\])*"', '""', code)

    if re.search('\\bmain\\s*\\(', code):
        errors.append('Do not define main in avl.cpp.')


# 4. Report errors and return failure to make, or confirm that checks passed.
if errors:
    print('RULE CHECK FAILED:\n' + '\n'.join(errors))
    sys.exit(1)

print('Rule check passed.')
