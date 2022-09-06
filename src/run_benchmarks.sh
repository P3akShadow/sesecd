#rm benchmarks/*.dump
for file in benchmarks/*; do
  echo $file
  cat "$file" | ./compiler 
done
