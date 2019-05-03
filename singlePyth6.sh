starver SL18b
line="$*"
argv=`sed 's/ /:/g' <<< "$line"`
root4star -q -b pyth6.C\(\"$argv\"\);
