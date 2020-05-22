#!/bin/bash
set -xeuo pipefail

curl 'https://cdash.karta.dyzio.pl/api/v1/index.php?project=Yio' |
jq '.buildgroups[] | select(.id == 2).builds[] | select(.buildname == "Linux-x86-64-gcc") |
	{ "pass": .test.pass, "fail": .test.fail, "time": .test.time }
'

