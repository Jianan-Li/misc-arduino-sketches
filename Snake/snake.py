import random
import sys
from Tkinter import *

def keyPress(event):
    canvas = event.widget.canvas
    if (event.char == "q"):
        canvas.data['quit']=True
    elif (event.char == "r"):
        canvas.data['start']=False
        init(canvas)
    elif (event.keysym == 'space'):
        canvas.data['start']= not canvas.data['start']
        timerFired(canvas)
    if (canvas.data["isGameOver"] == False) and (canvas.data['start']==True):
        if (event.keysym == "Up" or event.keysym =='w') and (canvas.data['snakeDrow']!=1):
            moveSnake(canvas, -1, 0)
        elif (event.keysym == "Down" or event.keysym =='s') and (canvas.data['snakeDrow']!=-1):
            moveSnake(canvas, +1, 0)
        elif (event.keysym == "Left" or event.keysym =='a') and (canvas.data['snakeDcol']!=1):
            moveSnake(canvas, 0,-1)
        elif (event.keysym == "Right" or event.keysym =='d') and (canvas.data['snakeDcol']!=-1):
            moveSnake(canvas, 0,+1)
    canvas.data['ignoreNextTimerEvent']=True
    redrawAll(canvas)

def gameOver(canvas):
    saveHighscore(canvas)
    canvas.data["isGameOver"] = True

def saveHighscore(canvas):
    f=open('snakesave.txt','r')
    previousscore=0
    for x in f:
        previousscore=int(x)
    f.close()
    if previousscore<=canvas.data['highscore']:
        f=open('snakesave.txt','w')
        f.write(str(canvas.data['highscore']))
        f.close()
    
def moveSnake(canvas, drow, dcol):
    snakeBoard = canvas.data["snakeBoard"]
    rows = len(snakeBoard)
    cols = len(snakeBoard[0])
    headRow= canvas.data["headRow"]
    headCol= canvas.data["headCol"]
    newHeadRow = headRow + drow
    newHeadCol = headCol + dcol
    canvas.data["snakeDrow"] = drow
    canvas.data["snakeDcol"] = dcol
    if newHeadRow<0 or newHeadCol<0 or newHeadRow>=rows or newHeadCol>=cols:
        gameOver(canvas)
    elif snakeBoard[newHeadRow][newHeadCol]>0:
        gameOver(canvas)
    elif snakeBoard[newHeadRow][newHeadCol]==-1:
        canvas.bell()
        canvas.data['snakelength']+=1
        canvas.data['snakecolor'][canvas.data['snakelength']]=canvas.data['lastfoodcolor']
        snakeBoard[newHeadRow][newHeadCol] = 1 + snakeBoard[headRow][headCol]
        canvas.data["headRow"] = newHeadRow
        canvas.data["headCol"] = newHeadCol
        placeFood(canvas)
    else:
        snakeBoard[newHeadRow][newHeadCol] = 1 + snakeBoard[headRow][headCol]
        canvas.data["headRow"] = newHeadRow
        canvas.data["headCol"] = newHeadCol
        removeTail(canvas)
    
def removeTail(canvas):
    snakeBoard=canvas.data['snakeBoard']
    rows = len(snakeBoard)
    cols = len(snakeBoard[0])
    for row in range(rows):
        for col in range(cols):
            if snakeBoard[row][col]>0:
                snakeBoard[row][col]-=1
    
def timerFired(canvas):
    if canvas.data['ignoreNextTimerEvent']==False and canvas.data["isGameOver"] == False:
        moveSnake(canvas, canvas.data["snakeDrow"], canvas.data["snakeDcol"])
    else:
        canvas.data['ignoreNextTimerEvent']=False        
    redrawAll(canvas)
    delay = 200-canvas.data['snakelength']*3 # ms
    if canvas.data['start']==True:
        canvas.after(delay, timerFired, canvas)


def redrawAll(canvas):
    canvas.delete(ALL)
    drawSnakeBoard(canvas)
    if (canvas.data["isGameOver"] == True):
        canvas.create_text(canvas.data['canvaswidth']/2, canvas.data['canvasheight']/2, text="Game Over", font=("Helvetica", 32, "bold"),fill='white')

def drawSnakeBoard(canvas):
    #First the status bar
    snakeBoard = canvas.data["snakeBoard"]
    margin=canvas.data["margin"]
    sleft=0+margin
    sright=canvas.data['canvaswidth']-margin
    stop=canvas.data['canvasheight']-canvas.data['statusbarSize']-margin
    sbottom=canvas.data['canvasheight']-margin
    canvas.create_rectangle(sleft,stop,sright,sbottom, fill="gray")
    #canvas.create_rectangle()
    score=(canvas.data['snakelength']-1)*2*(canvas.data['snakelength']<=11)+(20+5*(canvas.data['snakelength']-11))*(canvas.data['snakelength']>11)
    canvas.data['score']=score
    if canvas.data['highscore']<canvas.data['score']:
        canvas.data['highscore']=canvas.data['score']
    highscore=canvas.data['highscore']
    canvas.create_text((sleft+sright+206)/2, (stop+sbottom-20)/2, text="Sore:%d" % (score), font=("Helvetica", 18, "bold"),fill='black')
    canvas.create_text((sleft+sright+160)/2, (stop+sbottom+20)/2, text="High Sore:%d" % (highscore), font=("Helvetica", 18, "bold"),fill='black')
    canvas.create_text((sleft+sright-100)/2, (stop+sbottom-30)/2, text='Arrow Keys - move the snake', font=("Helvetica", 13, "bold"),fill='black')
    canvas.create_text((sleft+sright-141)/2, (stop+sbottom)/2, text='Spacebar - start/pause', font=("Helvetica", 13, "bold"),fill='black')    
    canvas.create_text((sleft+sright-230)/2, (stop+sbottom+30)/2, text='R - reset', font=("Helvetica", 13, "bold"),fill='black')
    #Then the cells
    rows = len(snakeBoard)
    cols = len(snakeBoard[0])
    for row in range(rows):
        for col in range(cols):
            drawSnakeCell(canvas, snakeBoard, row, col)

def drawSnakeCell(canvas, snakeBoard, row, col):
    margin = canvas.data['margin']
    cellSize = canvas.data['cellSize']
    left = margin + col * cellSize
    right = left + cellSize
    top = margin + row * cellSize
    bottom = top + cellSize
    canvas.create_rectangle(left, top, right, bottom, fill="black")
    A=[left*3/4+right*1/4, top]
    B=[left*1/4+right*3/4, top]
    C=[right, top*3/4+bottom*1/4]
    D=[right, top*1/4+bottom*3/4]
    E=[left*1/4+right*3/4, bottom]
    F=[left*3/4+right*1/4, bottom]
    G=[left, top*1/4+bottom*3/4]
    H=[left, top*3/4+bottom*1/4]    
    if (snakeBoard[row][col] > 0):
        canvas.create_polygon([A,B,C,D,E,F,G,H], fill=canvas.data['snakecolor'][canvas.data['snakelength']])
    elif (snakeBoard[row][col] == -1):
        if [row,col]==canvas.data['foodposition']:
            canvas.create_polygon([A,B,C,D,E,F,G,H], fill=canvas.data['lastfoodcolor'])
        else:
            while True:
                foodcolor=random.choice(['lavender','LemonChiffon','coral','DarkBlue','ForestGreen','gold','GreenYellow','HotPink','BlueViolet','beige','azure','brown','red','green','purple','cyan','magenta','orange'])
                if foodcolor!=canvas.data['lastfoodcolor']:
                    break
            canvas.data['lastfoodcolor']=foodcolor
            canvas.create_polygon([A,B,C,D,E,F,G,H], fill=foodcolor)
            canvas.data['foodposition']=[row,col]
    return

def placeFood(canvas):
    snakeBoard=canvas.data['snakeBoard']
    rows = canvas.data["rows"]
    cols = canvas.data["cols"]
    while True:
        randrow=random.randint(0,rows-1)
        randcol=random.randint(0,cols-1)
        if snakeBoard[randrow][randcol]==0:
            snakeBoard[randrow][randcol]=-1
            break
    
def loadSnakeBoard(canvas):
    rows = canvas.data["rows"]
    cols = canvas.data["cols"]
    snakeBoard = [ ]
    for row in range(rows): 
        snakeBoard += [[0] * cols]
    snakeBoard[rows/2][cols/2]=1
    canvas.data["snakeBoard"] = snakeBoard
    findSnakeHead(canvas)
    placeFood(canvas)

def findSnakeHead(canvas):
    snakeBoard = canvas.data["snakeBoard"]
    rows = len(snakeBoard)
    cols = len(snakeBoard[0])
    headRow = 0
    headCol = 0
    for row in range(rows):
        for col in range(cols):
            if (snakeBoard[row][col] > snakeBoard[headRow][headCol]):
                headRow = row
                headCol = col
    canvas.data["headRow"] = headRow
    canvas.data["headCol"] = headCol
    
def init(canvas):
    loadSnakeBoard(canvas)
    canvas.data["isGameOver"] = False
    canvas.data["ignoreNextTimerEvent"] = False
    canvas.data['snakelength']=1
    initialDirection=random.randint(1,4)
    if initialDirection==1:
        canvas.data["snakeDrow"]=0
        canvas.data["snakeDcol"]=1
    elif initialDirection==2:
        canvas.data["snakeDrow"]=0
        canvas.data["snakeDcol"]=-1
    elif initialDirection==3:
        canvas.data["snakeDrow"]=1
        canvas.data["snakeDcol"]=0
    else:
        canvas.data["snakeDrow"]=-1
        canvas.data["snakeDcol"]=0
    redrawAll(canvas)

def run(rows,cols):
    root = Tk()
    root.title('This is not Snake!')
    margin = 8
    cellSize = 20
    statusbarSize=50
    canvasWidth = 2*margin + cols*cellSize
    canvasHeight = 2*margin + rows*cellSize+statusbarSize
    canvas = Canvas(root, width=canvasWidth, height=canvasHeight)
    canvas.pack()
    # Store canvas in root and in canvas itself for callbacks
    root.canvas = canvas.canvas = canvas
    canvas.data = { }
    canvas.data['highscore']=0
    f=open('snakesave.txt','r')
    for x in f:
        canvas.data['highscore']=int(x)
    f.close()
    canvas.data['score']=0
    canvas.data['snakecolor']={}
    canvas.data['lastfoodcolor']='blue'
    canvas.data['foodposition']=[0,0]
    canvas.data['snakelength']=1
    canvas.data['snakecolor'][1]='blue'    
    canvas.data['canvaswidth']=canvasWidth
    canvas.data['canvasheight']=canvasHeight
    canvas.data['quit']=False
    canvas.data['start']=False
    canvas.data['ignoreNextTimerEvent']=False
    canvas.data["isGameOver"] = False
    canvas.data["margin"] = margin
    canvas.data["cellSize"] = cellSize
    canvas.data['statusbarSize']=statusbarSize
    canvas.data["rows"] = rows
    canvas.data["cols"] = cols
    init(canvas)
    canvas.create_text(canvas.data['canvaswidth']/2, canvas.data['canvasheight']/2+20, text="Press Spacebar to Start", font=("Helvetica", 20, "bold"),fill='white')
    # set up events
    #root.bind("<Button-1>", mousePress)
    root.bind("<Key>", keyPress)
    # and launch the app
    root.mainloop()  # This call BLOCKS (so your program waits until you close the window!)

run(20,16)