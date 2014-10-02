#!/bin/bash


# "grep vischia" selects only the lines corresponding to a job. It might as well be "grep job-" or stuff like that.
while true ;
 do
  echo "running: " `qstat -u viesturs | grep viesturs | grep " r " | wc -l` "    total: " `qstat -u viesturs | grep viesturs | wc -l` 
done