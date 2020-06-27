#!/bin/bash
set -euo pipefail

fatal() {
	echo "$(basename "$0"): ERROR: $*" >&2
	exit 1
}

if ! cd "$(dirname "$0")/../public"; then
	fatal "public dir doesn't exists"
fi

# Create files.txt
{
	# all files except doxygen
	# only html in doxygen folder
	find . -mindepth 1 -type f '(' '!' -path './doxygen/*' -o \
		-path './doxygen/index.html' ')' |
	sed 's@./@@' | sort > files.txt
	if sed -n '/[^[:alnum:][:punct:]]/{p;q};$q1' files.txt; then
		fatal "there is a unprintable filename in files.txt!"
	fi
}

# Generate index.html
(
	tmp=$(mktemp)
	trap 'rm -r "$tmp"' EXIT
	< files.txt awk '{
		printf "<tr><td>"
		printf "<a href=\"" $1 "\">" $1 "</a>"
		printf "</td></tr>\n"
	}' > "$tmp"
	sed -e '/@@FILES@@/{ s///; r '"$tmp" -e '}' > index.html <<'EOF'
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>yio gitlab public pages</title>
  <meta name="description" content="kamcuk archlinux repository">
  <meta name="author" content="Kamil Cukrowski">
  <link rel="stylesheet" href="css/styles.css?v=1.0">
</head>
<body>

<br>
Files in this repo:<br>
<p>
<table>
<tr><th>File</th><tr>
  @@FILES@@
</table>
</p>

<p>
Written by Kamil Cukrowski<br>
</p>

</body>
</html>

EOF
)

echo "Generated index.html from $(wc -l files.txt) files"
