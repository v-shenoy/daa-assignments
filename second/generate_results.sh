#!/bin/bash

if [ $# -lt 2 ]; then
	echo "Expect problem, and results-file name."
	exit 1
fi

function exit_if_error() {
	if [ $? -ne 0 ]; then
		exit 1
	fi
}

make ff
exit_if_error

prefix="ff"
if [[ $1 = "bipartite_matching" ]]
then
	prefix="bm"
fi

for graph in $(ls graphs)
do
	if [[ $graph =~ ^$prefix ]]
	then
		echo "Running graph - $graph"
		./bin/ff $1 "./graphs/$graph" $2 >/dev/null
		exit_if_error
	fi
done