grep -o "mul([0-9][0-9]*,[0-9][0-9]*)" input.txt | tr -d 'mul()' | tr ',' '*' | paste -sd+ - | bc

