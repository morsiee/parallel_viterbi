#gnuplot
#set terminal postscript eps color colortext 'Helvetica'17
#set terminal pngcairo enhanced font 'Verdana,10'
#set termoption dashed
set terminal png enhanced font 'Verdana,11' butt dashlength 1.0 
set boxwidth 0.9 absolute
set style fill   solid 1.00 border lt -1
set key inside right top vertical Right noreverse noenhanced autotitle nobox
set style histogram clustered gap 1 title textcolor lt -1
set datafile missing '-'
set style data histograms
set xtics border in scale 0,0 nomirror rotate by -45  autojustify
set xtics  norangelimit
set xtics   ()
set datafile separator ','
set grid

set output 'Running time.png'
set ylabel 'Running time [sec]'
set title 'Optimised Viterbi vs. Maps implementation vs. highly optimised GHMM'
set yrange [ 0.00 : 17 ] noreverse nowriteback
x = 0.0
plot 'Fri Sep 11, 2015.csv' using 2:xtic(1) ti col, '' u 3 ti col, '' u 4 ti col, '' u 5 ti col


set key inside left top vertical Left reverse noenhanced autotitle nobox
set output 'speedup.png'
set xlabel 'Sparsity'
set ylabel 'Speedup'
set title "Viterbi algorithm sparsity optimisation\n Array implementation speedup vs.\n Maps implementation and a highly optimised GHMM"
set label "{/:Bold 10x}" at 5.7,18.5
set yrange [ -10.0 : 20 ] noreverse nowriteback
plot 'Fri Sep 11, 2015 - speed up.csv' using 2:xtic(1) ti col, '' u 3 ti col, '' u 4 ti col


