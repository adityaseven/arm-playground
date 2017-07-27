load-cmsis-svd MKL46Z4.svd
target remote localhost:3333
set mem inaccessible-by-default off
monitor reset halt
load
monitor reset init
break main
continue
clear
