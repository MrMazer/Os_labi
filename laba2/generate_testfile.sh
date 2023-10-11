#!/bin/bash

# Генерируем случайные целые числа и записываем их в файл testfile.txt
for i in {1..20000000}; do
  echo $((RANDOM % 1000)) >> testfile.txt
done

echo "Сгенерирован файл testfile.txt с 20000000 случайными числами."
