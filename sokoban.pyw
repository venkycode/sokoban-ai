try:
    from tkinter import *
except ImportError:
    from Tkinter import *
try:
    from tkinter.filedialog import askopenfilename
except ImportError:
    from tkFileDialog import askopenfilename
try:
    from tkinter.messagebox import *
except ImportError:
    from tkMessageBox import *
import sys
from time import sleep
"""
    O ... igralec
    # ... zid
    * ... blokec (ki se ga lahko premika)
    X ... tocka za zmago
    A ... igralec na tocki za zmago
    R ... blokec na tocki za zmago
    
"""


def makeLevel(string):
    p = []
    with open(string, 'r') as f:
        for line in f:
            line = line.strip()
            foo = []
            for i in line:
               foo.append(i)
            p.append(foo)
    return p

def findPlayer():
    print(w,h)
    for j in range(h):
        for i in range(len(p[j])):
            if(p[j][i] == '@'):
                return(i,j)

def kvadrat(x, y, barva,flag=0):
    if flag: canvas.create_rectangle(x+wid*0.2, y+wid*0.2, x+wid*0.8, y+hei*0.8, fill=barva)
    else:canvas.create_rectangle(x, y, x+wid, y+hei, fill=barva)

def krog(x, y, barva):
    canvas.create_oval(x, y, x+wid, y+hei, fill=barva)

def draw():
    canvas.delete("all")
    for i in range(wid, width, wid):
        canvas.create_line(i, 0, i, height)

    for i in range(hei, height, hei):
        canvas.create_line(0, i, width, i)

    for j in range(h):
        for i in range(len(p[j])):
            if(p[j][i] == '#'):
                kvadrat(i*wid, j*hei, "#252525")
            if(p[j][i] == '@'):
                krog(i*wid, j*hei, "red")
            if(p[j][i] == '$'):
                kvadrat(i*wid, j*hei, "blue",1)
            if(p[j][i] == '.'):
                kvadrat(i*wid, j*hei, "yellow")
            if(p[j][i] == '+'):
                kvadrat(i*wid, j*hei, "yellow")
                krog(i*wid, j*hei, "red")
            if(p[j][i] == '*'):
                kvadrat(i*wid, j*hei, "green")
                kvadrat(i*wid, j*hei, "pink",1)
            
            
def kill(event):
    root.destroy()

def movePlayerFrom(x, y):
    #global ply_y, ply_x
    if(p[y][x] == '@'):
        p[y][x] = ' '
    if(p[y][x] == '+'):
        p[y][x] = '.'

def movePlayerTo(x, y):
    if(p[y][x] == '.'):
        p[y][x] = '+'
    if(p[y][x] == ' '):
        p[y][x] = '@'

def moveBoxFrom(x, y):
    if(p[y][x] == '$'):
        p[y][x] = ' '
    if(p[y][x] == '*'):
        p[y][x] = '.'
        
def moveBoxTo(x, y):
    if(p[y][x] == ' '):
        p[y][x] = '$'
    if(p[y][x] == '.'):
        p[y][x] = '*'


def premikanje(x, y):
#    print("Hej")
    global ply_y, ply_x
    if(p[ply_y + y][ply_x + x] in dovoljeni):
        movePlayerFrom(ply_x, ply_y)
        ply_y += y
        ply_x += x
        movePlayerTo(ply_x, ply_y)

    elif(p[ply_y + y][ply_x + x] in premikajoci):
        if(p[ply_y + 2*y][ply_x + 2*x] in dovoljeni):
            moveBoxFrom(ply_x + x, ply_y + y)
            moveBoxTo(ply_x + 2*x, ply_y + 2*y)
            movePlayerFrom(ply_x, ply_y)
            ply_y += y
            ply_x += x
            movePlayerTo(ply_x, ply_y)
            

def isWin():
    for i in p:
        for j in i:
            if(j == '.'):
                return False
            elif(j == '+'):
                return False
    return True

def movement(n):
    global ply_x, ply_y

    if(n == 'Up'):
        premikanje(0, -1)

    elif(n == 'Down'):
        premikanje(0, 1)

    elif(n == 'Left'):
        premikanje(-1, 0)

    elif(n == 'Right'):
        premikanje(1, 0)

    draw()
    if(isWin()):
        showinfo("You won!", "Congratulations, you won!")
        root.destroy()
    
    

def restart():
    global p
    global ply_x, ply_y
    p = makeLevel(level)
    ply_x, ply_y = findPlayer()
    draw()
    
    


def keyHandler(event):
    if not solutionGiven:
        foo = event.keysym
        movement(foo)
    if(event.char == 'r'):
        restart()


def askLevel():
    top = Tk()
    top.withdraw()
    level = askopenfilename(initialdir = "", filetypes = [('Txt files','.txt'),('Level files', '.lvl'), ('All files', '.*')], title = "Choose the level you want to play")
    top.destroy()

    try:
        return(makeLevel(level))
    except IOError:
        top = Tk()
        top.withdraw()
        if(askretrycancel("Error!", "There was an error trying to open your level. Do you want to try again?")):           
            try:
                return(askLevel())
            finally:
                top.destroy()
        else:
            top.destroy()
            return(False)


solutionGiven=0

if(sys.argv.count("-runWithSolution")) : solutionGiven=1

#print(solutionGiven)
p = askLevel()
if(not p):
    pass
else:
    w = len(p[0])
    for x in p: w=max(w,len(x))
    h = len(p)

    dovoljeni = [' ', '.']
    premikajoci = ['$', '*']


    max_width = 1000
    max_height = 1000


    wid = hei = 50

    if(wid*w > max_width or hei*h > max_height):
        wid = hei = min(max_width//w, max_height//h)
    width = wid * w
    height = hei * h



    ply_x, ply_y = findPlayer()





    root = Tk()
    root.title("Simple Sokoban clone")
    root.focus_force()

    canvas = Canvas(root, width=width, height=height)
    canvas.pack()
    draw()

    root.bind_all("<Escape>", kill)
    root.bind_all("<Key>", keyHandler)
    if not solutionGiven: root.mainloop()
    st= input()

    print(st,type(st))
    

    if solutionGiven:
        firstmove=1
        for mv in st:
            root.update()
            mv1='X'
            if mv=='U': mv1='Up'
            elif mv=='R': mv1='Right'
            elif mv=='L': mv1='Left'
            elif mv=='D': mv1='Down'
            print(mv)
            movement(mv1)
            if not firstmove:sleep(0.3)
            else:
                sleep(3)
                firstmove=0
                root.update()

            root.update()
        root.mainloop()
        

                

    
