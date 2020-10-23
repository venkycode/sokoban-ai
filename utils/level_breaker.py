import os
pwd="/home/venkatesh/Desktop/work/aiproject/utils/"
f=open(pwd+'all_levels.txt',"r")

lines= f.readlines();
f.close()
cnt=0
wr_pwd="/home/venkatesh/Desktop/work/aiproject/standard_levels/"
os.mkdir(wr_pwd)
wr= open(wr_pwd+"lvl"+str(cnt)+".txt","w")

for line in lines:
    if(line[0]==';'):
        wr.close()
        cnt+=1
        wr= open(wr_pwd+"lvl"+str(cnt)+".txt","w")
        continue
    wr.write(line)