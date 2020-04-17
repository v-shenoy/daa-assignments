#!/bin/bash

if [ $# -lt 2 ]; then
	echo "Expect algorithm, and results-file name."
	exit 1
fi

function exit_if_error() {
	if [ $? -ne 0 ]; then
		exit 1
	fi
}

make getScc
exit_if_error

for graph in $(ls graphs)
do
    echo "Running graph - $graph"
    ./bin/getScc "./graphs/$graph" $1 $2 >/dev/null
    exit_if_error
done
