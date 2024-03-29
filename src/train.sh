#!/bin/bash

echo "Path to the direcory with training files ('.' for current):"
read the_path

ls -x $the_path | xargs ./zipme -r $the_path -t
