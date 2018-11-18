#!/usr/bin/gnuplot -persist
set grid
set time
set ylabel "Energia(J)"
set xlabel "Num. Iteração"
set title "Gráfico de Energia"
plot "energias.dat" using ($1):($2) with lines
