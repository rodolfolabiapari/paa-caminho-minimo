#!/bin/bash

echo "Quantas iteracoes?"
read quantidade_iteracoes;
echo

eval "rm saida*"
eval "rm tempo*"
eval "gcc bellman/bellman.c -o bellman/bellman"
eval "gcc floyd/floyd.c -o floyd/floyd"
eval "gcc dijkstra/dijkstra.c -o dijkstra/dijkstra"

#instancias=( rome99.gr rg300_4730.gr rg300_768_floyd.gr )
#instancias=( rg300_768_floyd.gr )
instancias=( rg300_768_floyd-n.gr )
#instancias=( rome99.gr )
#instancias=( cormen.txt )
#instancias=( comp-2007-2-22c-negative.gr )

#algoritmos=( bellman dijkstra floyd )
algoritmos=( bellman floyd )

for algoritmo in "${algoritmos[@]}"
do
	echo $algoritmo

  for instancia in "${instancias[@]}"
  do
  	echo $instancia

    for (( i = 0; i < "$quantidade_iteracoes"; i++ )); do
      echo "$i"

      cmd="./$algoritmo/$algoritmo $instancia"
      date
      echo $cmd
      $cmd
    done
    echo

  done
  echo

done


#if ["$i" -ge "$quantidade_iteracoes"]
