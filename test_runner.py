import glob
import os
ll=glob.glob('./standard_levels/*')
lx= sorted(ll)
for i in lx:
    os.system('./main1.out -nameOfFile '+ i[1:]+' '+'<'+i+' >>standard_level_results.txt')
