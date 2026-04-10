#!/bin/bash

gcc teste.c -o teste
gcc teste_ilustrativo.c -o teste_ilustrativo

echo "teste ilustrativo diff"

./teste_ilustrativo > teste_ilustrativo_output.txt

diff teste_ilustrativo_output.txt output.txt > diferenca_teste_ilustrativo.txt

if [ ! -s "diferenca_teste_ilustrativo.txt" ]; then
    echo "não há diferença!"
else
    echo "tem diferença!"
fi