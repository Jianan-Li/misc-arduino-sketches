'''
Created on Feb 23, 2013

v1.0 Feb.24 
    Note: Not all trick moves are supported in this version. You are allowed to move the piece shortly
          after it has landed, but you can't rotate it after it has landed. I'll try to improve this in
          future updates.
          There might still be some bugs hidden here and there. I'll try my best to patch them in future
          updates.

Several great ideas used in creating this tetris:
    Different pieces are represented by different positive numbers (2,3,4,5,6...); after it's landed,
    the positive numbers are turned into negative values (-2,-3,-4,-5,-6...) so that it can be diffe-
    rentiated from the next piece generated;
    
    All pieces are actually a 3*3 matrix. It includes the actual blocks represented by the corresponding
    positive number, empty blocks represented by 0, and most importantly, the corner markers represented
    by -1 that is extremely crucial in the rotation process.
    
    Other great ideas can be discovered throughout this program. Just Read the whole thing.
    
    P.S. the deepcopy() function in the copy module is very handy...
    
@author: Jianan Li
'''
import random
import copy
from Tkinter import *

def keyPress(event):
    canvas=event.widget.canvas
    if (event.keysym == 'd'):
        canvas.data['debug']= not canvas.data['debug']
    elif (event.keysym == 'q'):
        gameOver(canvas)
    elif (event.keysym == 's'):
        canvas.data['start']= not canvas.data['start']
        canvas.data['gameStarted']=True
        timerFired(canvas)
    elif (event.keysym == 'r'):
        canvas.data['gameStarted']=False
        init(canvas)
    if canvas.data['start']==True and canvas.data["isGameOver"]== False:
        if event.keysym=='Up':
            checkoutofboundary(canvas)
            if checkoutofboundary(canvas)==False:   # if moveTetris is not allowed on the boundary, rotation is therefore not allowed as well
                return
            changeShape(canvas)
        elif event.keysym=='Left':
            moveTetris(canvas,0,-1)
        elif event.keysym=='Right':
            moveTetris(canvas,0,1)
        elif event.keysym=='Down':
            moveTetris(canvas,1,0)
        elif event.keysym=='space':
            dropPiece(canvas)
    redrawAll(canvas)

def dropPiece(canvas):  # move the piece down block by block (not called by timerfired, so no delay) until it's landed 
    while checkIfLanded(canvas)==False: # then the canvas is redrawn in keyPress(event)
        moveTetris(canvas,1,0)

def checkoutofboundary(canvas): #make rotating possible when the piece is right next to the walls
    cols=canvas.data['cols']    # when the 3*3 piece has it's -1 0 -1 row outside the boundary, we move the piece one block inward before rotating it
    rows=canvas.data['rows']
    tetrisBoard=canvas.data['tetrisBoard']
    oldtetrisBoard=copy.deepcopy(tetrisBoard)
    outofboundary1=[]
    for x in range(rows):
        outofboundary1+=[tetrisBoard[x][0]]
    outofboundary2=[]
    for x in range(rows):
        outofboundary2+=[tetrisBoard[x][cols-1]]
    outofboundary3=[]
    for x in range(rows):
        outofboundary3+=tetrisBoard[rows-1]
    if -1 in set(outofboundary1):
        moveTetris(canvas,0,1)      # here is the interesting part: if the moveTetris operation failed, signified by no change at all on the board
        canvas.data['cancelmove']=[0,1]
        if oldtetrisBoard==canvas.data['tetrisBoard']:      # the rotation would be suppressed as well
            return False
    elif -1 in set(outofboundary2):
        moveTetris(canvas,0,-1)
        canvas.data['cancelmove']=[0,-1]
        if oldtetrisBoard==canvas.data['tetrisBoard']:
            return False
    elif -1 in set(outofboundary3):
        moveTetris(canvas,-1,0)
        canvas.data['cancelmove']=[-1,0]
        if oldtetrisBoard==canvas.data['tetrisBoard']:
            return False
    return True
    
def changeShape(canvas):    #the magical rotation operation on the 3*3 pieces
    cols=canvas.data['cols']
    rows=canvas.data['rows']
    tetrisBoard=canvas.data['tetrisBoard']
    canbreak=False
    ltrow=canvas.data['ltrow']      # lefttop corner tracking
    ltcol=canvas.data['ltcol']
    #check surrounding blocks    # improvement needed! here it checks whether there is leftover in the 3*3 area
    for x in range(ltrow,ltrow+3):  # if there is, rotation is suppressed 
        for y in range(ltcol,ltcol+3):
            if tetrisBoard[x][y]<-1:
                if canvas.data['cancelmove']!=[0,0]:    # written specifically for the two 1*3 pieces, which, when adjacent to the wall, can be moved first in checkoutofboundary(canvas) but then fail the rotation; this cancels out the unnecessary move
                    moveTetris(canvas,-canvas.data['cancelmove'][0],-canvas.data['cancelmove'][1]) 
                    canvas.data['cancelmove']=[0,0]
                return
    lastshape=canvas.data['lastshape']
    newshape=[]
    for i in range(3):
        newshape+=[[0]*3]
    for x in range(3):
        for y in range(3):
            newshape[x][y]=lastshape[2-y][x]
    for row in range(ltrow,ltrow+3):
        for col in range(ltcol,ltcol+3):
            tetrisBoard[row][col]=newshape[row-ltrow][col-ltcol]
    canvas.data['lastshape']=newshape

def moveTetris(canvas,drow,dcol):
    cols=canvas.data['cols']
    rows=canvas.data['rows']
    tetrisBoard=canvas.data['tetrisBoard']
    changedict={}
    for row in range(rows):
        for col in range(cols):
            if tetrisBoard[row][col]>0 or tetrisBoard[row][col]==-1:    #Only the current piece has value greater than 0 and -1
                changedict[row,col]= tetrisBoard[row][col]  # Store all the changes needed in the dict
    outofboundary=[]
    newtetrisBoard=copy.deepcopy(tetrisBoard)
    for x in changedict.keys():
        newtetrisBoard[x[0]][x[1]]=0    # set all the block occupied by the piece at the previous position to 0
    for x in changedict.keys():     # If the block isn't occupied, 
        if changedict[x]!=-1 and newtetrisBoard[x[0]+drow][x[1]+dcol]==0:   #the -1 blocks may already be on the rightmost column; our of range
            # the -1 blocks won't affect the determining of whether the real blocks of the piece is out of boundary or not
            newtetrisBoard[x[0]+drow][x[1]+dcol]=changedict[x]
    for x in range(rows):   #col 0, col cols-1, row -1 are the boundaries
        outofboundary+=[newtetrisBoard[x][0],newtetrisBoard[x][cols-1]]
    outofboundary+=newtetrisBoard[-1]
    if (set(outofboundary)==set([-1,0]) or set(outofboundary)==set([0])) and checkSurrounding(canvas,changedict,drow,dcol)==True:
        for x in changedict.keys():     # update only if there are only -1 and 0 in the boundary set AND if there is indeed space to move if the movement is left/right 
            if newtetrisBoard[x[0]+drow][x[1]+dcol]==0: # only update the blocks that are still zero 
                newtetrisBoard[x[0]+drow][x[1]+dcol]=changedict[x]  
                # we can safely update the blocks because if any of them has something under them, 
                #it would be detected by checkIfLanded(canvas), which prevent the piece from moving downward
        canvas.data['ltrow'] +=drow    # left top corner tracking
        canvas.data['ltcol'] +=dcol
        tetrisBoard=newtetrisBoard
    canvas.data['tetrisBoard']=tetrisBoard

def checkSurrounding(canvas,changedict,drow,dcol): #suppress moving the piece when there are blocks around
    tetrisBoard=canvas.data['tetrisBoard']
    for x in changedict.keys():
        if (changedict[x]>0 and tetrisBoard[x[0]+1][x[1]]<-1 and drow==1) or (changedict[x]>0 and tetrisBoard[x[0]][x[1]-1]<-1 and dcol==-1) or (changedict[x]>0 and tetrisBoard[x[0]][x[1]+1]<-1 and dcol==1):
            return False
    return True
    
def checkIfLanded(canvas):  # if any block below the current piece is less than -1, confirm the block is landed, return True
    cols=canvas.data['cols']    # (below the lowest block of the piece in each col)
    rows=canvas.data['rows']
    tetrisBoard=canvas.data['tetrisBoard']
    lowestblock={}
    for x in range(rows):   
        for y in range(cols):
            if tetrisBoard[x][y]>0: # find the lowest block of the piece in each col
                lowestblock[y]=x
    for x in lowestblock.items():
        if tetrisBoard[x[1]+1][x[0]]<-1: # if any <-1 return true
            return True
    if max(lowestblock.values())==rows-2:   # if reached bottom (row 11), return true
        return True
    return False

def landed(canvas):
    cols=canvas.data['cols']
    rows=canvas.data['rows']
    tetrisBoard=canvas.data['tetrisBoard']
    for x in range(rows):
        for y in range(cols):
            if tetrisBoard[x][y]==-1:
                tetrisBoard[x][y]=0
            if tetrisBoard[x][y]>0:
                tetrisBoard[x][y]=-tetrisBoard[x][y]
    checkIfClear(canvas)
    placeTetris(canvas)
    
def checkIfClear(canvas):
    cols=canvas.data['cols']
    rows=canvas.data['rows']
    tetrisBoard=canvas.data['tetrisBoard']
    newtetrisBoard=[]
    for x in range(3,rows-1):
        if 0 in tetrisBoard[x][1:-1]:
            newtetrisBoard+=[tetrisBoard[x]]
    newtetrisBoard +=[[0]*cols]
    newtetrisBoard=newtetrisBoard[-1::-1]
    i=0
    while len(newtetrisBoard)<len(tetrisBoard):
        newtetrisBoard+=[[0]*cols]
        i +=1
    canvas.data['linecleared'] += (i-3)
    newtetrisBoard=newtetrisBoard[-1::-1]
    canvas.data['tetrisBoard']=newtetrisBoard
    
'''
redraw start
'''
def redrawAll(canvas):
    canvas.delete(ALL)
    drawTetrisBoard(canvas)

def drawTetrisBoard(canvas):
    cols=range(canvas.data['cols'])
    rows=range(canvas.data['rows'])
    for row in rows:
        for col in cols:
            drawTetrisCell(canvas,row,col)
    drawCover(canvas)

def drawTetrisCell(canvas,row,col):
    cols=canvas.data['cols']
    rows=canvas.data['rows']
    tetrisBoard=canvas.data['tetrisBoard']
    margin=canvas.data['margin']
    cellsize=canvas.data['cellsize']
    left=margin+col*cellsize
    right=left+cellsize
    top=margin+row*cellsize
    bottom=top+cellsize
    if tetrisBoard[row][col]==0:
        if canvas.data['debug']==True:
            canvas.create_rectangle(left,top,right,bottom,fill='white')  
        else:
            canvas.create_rectangle(left,top,right,bottom,fill='black')  
    elif tetrisBoard[row][col]==-1:
        canvas.create_rectangle(left,top,right,bottom,fill='black')
    elif tetrisBoard[row][col]==2 or tetrisBoard[row][col]==-2:
        canvas.create_rectangle(left,top,right,bottom,fill='green')
    elif tetrisBoard[row][col]==3 or tetrisBoard[row][col]==-3:
        canvas.create_rectangle(left,top,right,bottom,fill='red')
    elif tetrisBoard[row][col]==4 or tetrisBoard[row][col]==-4:
        canvas.create_rectangle(left,top,right,bottom,fill='blue')
    elif tetrisBoard[row][col]==5 or tetrisBoard[row][col]==-5:
        canvas.create_rectangle(left,top,right,bottom,fill='magenta')
    elif tetrisBoard[row][col]==6 or tetrisBoard[row][col]==-6:
        canvas.create_rectangle(left,top,right,bottom,fill='orange')
    elif tetrisBoard[row][col]==7 or tetrisBoard[row][col]==-7:
        canvas.create_rectangle(left,top,right,bottom,fill='yellow')
    elif tetrisBoard[row][col]==8 or tetrisBoard[row][col]==-8:
        canvas.create_rectangle(left,top,right,bottom,fill='cyan')
    if row==0 or row==rows-1 or col==0 or col==cols-1:
        canvas.create_rectangle(left,top,right,bottom,fill='gray')
        
def drawCover(canvas):
    cols=canvas.data['cols']
    margin=canvas.data['margin']
    cellsize=canvas.data['cellsize']
    left=margin
    right=left+cols*cellsize
    top=margin
    bottom=top+3*cellsize
    canvas.create_rectangle(left,top,right,bottom,fill='gray')
    canvas.create_text((left+right-140)/2, (top+bottom-50)/2, text="Move - Left, Right, Down", font=("Helvetica", 13, "bold"),fill='Black')
    canvas.create_text((left+right-223)/2, (top+bottom-23)/2, text="Rotate - Up", font=("Helvetica", 13, "bold"),fill='Black')
    canvas.create_text((left+right-192)/2, (top+bottom)/2, text="Drop - Spacebar", font=("Helvetica", 13, "bold"),fill='Black')
    canvas.create_text((left+right-237)/2, (top+bottom+27)/2, text="Reset - R", font=("Helvetica", 13, "bold"),fill='Black')
    canvas.create_text((left+right-40)/2, (top+bottom+50)/2, text="Quit - Q", font=("Helvetica", 13, "bold"),fill='Black')
    canvas.create_text((left+right-55)/2, (top+bottom+27)/2, text="Debug - D", font=("Helvetica", 13, "bold"),fill='Black')
    canvas.create_text((left+right-199)/2, (top+bottom+50)/2, text="Start/Pause - S", font=("Helvetica", 13, "bold"),fill='Black')
    #score
    score=canvas.data['linecleared']*10
    canvas.data['score']=score
    if canvas.data['highscore']<canvas.data['score']:
        canvas.data['highscore']=canvas.data['score']
    highscore=canvas.data['highscore']
    canvas.create_text((left+right+130)/2, (top+bottom-30)/2, text="High Score:", font=("Helvetica", 18, "bold"),fill='Black')
    canvas.create_text((left+right+270)/2, (top+bottom-30)/2, text="%d" % (highscore), font=("Helvetica", 18, "bold"),fill='Black')
    canvas.create_text((left+right+177)/2, (top+bottom+30)/2, text="Score:", font=("Helvetica", 18, "bold"),fill='Black')
    canvas.create_text((left+right+270)/2, (top+bottom+30)/2, text="%d" % (score), font=("Helvetica", 18, "bold"),fill='Black')
    if canvas.data['start']==False and canvas.data["isGameOver"] == False and canvas.data['gameStarted']==True:
        if canvas.data['debug']==True:
            canvas.create_text((left+right)/2, (top+bottom+300)/2, text="||", font=("Helvetica", 52, "bold"),fill='black')
        else:
            canvas.create_text((left+right)/2, (top+bottom+300)/2, text="||", font=("Helvetica", 52, "bold"),fill='white')
    if canvas.data["isGameOver"] == True:
        drawGameOver(canvas)
        
def drawGameOver(canvas):
    cols=canvas.data['cols']
    margin=canvas.data['margin']
    cellsize=canvas.data['cellsize']
    left=margin
    right=left+cols*cellsize
    top=margin
    bottom=top+3*cellsize
    position=canvas.data['initGameOverPos']
    if position<canvas.data['finaGameOverPos']:
        position+=1
        canvas.data['initGameOverPos']+=1
    canvas.create_text((left+right)/2, (top+bottom+200)/2+position, text="Game Over", font=("Helvetica", 42, "bold"),fill='white')
    delay=10
    canvas.after(delay,redrawAll,canvas)
    
'''
redraw end
'''    

def storeShapes(canvas):
    canvas.data['shape'][1]=[[-1,0,-1],[0,2,0],[2,2,2]]
    canvas.data['shape'][2]=[[-1,0,-1],[3,0,0],[3,3,3]]
    canvas.data['shape'][3]=[[-1,0,-1],[0,0,4],[4,4,4]]
    canvas.data['shape'][4]=[[-1,0,-1],[5,5,0],[-1,5,5]]
    canvas.data['shape'][5]=[[-1,0,-1],[0,6,6],[6,6,-1]]
    canvas.data['shape'][6]=[[-1,0,-1],[7,7,7],[-1,0,-1]]
    canvas.data['shape'][7]=[[-1,8,-1],[0,8,0],[-1,8,-1]]

def placeTetris(canvas):
    tetrisBoard=canvas.data['tetrisBoard']
    shapeNum=random.choice(canvas.data['shape'].keys())
    shape=canvas.data['shape'][shapeNum]
    canvas.data['lastshape']=shape      # stored for rotation
    if len(canvas.data['shape'][shapeNum])==3:
        for row in range(3):
            for col in range(3):
                tetrisBoard[row][col+5] += shape[row][col]      # Need to be modified with different table size
    canvas.data['tetrisBoard']=tetrisBoard
    canvas.data['ltrow']=0     # left top corner tracking
    canvas.data['ltcol']=5     
    if tetrisBoard[3][5]!=0 or tetrisBoard[3][6]!=0 or tetrisBoard[3][7]!=0:
        gameOver(canvas)
         
def loadTetrisBoard(canvas):
    cols=canvas.data['cols']
    rows=canvas.data['rows']
    tetrisBoard=[]
    for i in range(rows):
        tetrisBoard += [[0]*cols]
    canvas.data['tetrisBoard']=tetrisBoard
    placeTetris(canvas)

def timerFired(canvas):         # Confirm the block is landed only after checkIfLanded(canvas)==True twice in a row
    if checkIfLanded(canvas)==True:     # which means that you have one delay interval (<350ms) to move the block 
        canvas.data['downOrpause']=0    # left or right before the landing is confirmed and a new piece is generated
        canvas.data['landedtwiceinarow'] +=1
    else:
        canvas.data['downOrpause']=1
        canvas.data['landedtwiceinarow']=0
    downOrpause=canvas.data['downOrpause']
    if canvas.data['landedtwiceinarow']>=2:     # landing is confirmed here
        canvas.data['landedtwiceinarow']=0
        landed(canvas)
    if canvas.data['start']==True and canvas.data["isGameOver"]==False:
        moveTetris(canvas,downOrpause,0)
        redrawAll(canvas)
        linecleared=canvas.data['linecleared']
        delay=350-10*linecleared     # change difficulty here~~~~~~~~~
        canvas.after(delay, timerFired, canvas)
    
def gameOver(canvas):
    saveHighscore(canvas)
    canvas.data["isGameOver"] = True
    canvas.data['gameStarted']=False
    redrawAll(canvas)

def saveHighscore(canvas):
    f=open('tetrissave.txt','r')
    previousscore=0
    for x in f:
        previousscore=int(x)
    f.close()
    if previousscore<=canvas.data['highscore']:
        f=open('tetrissave.txt','w')
        f.write(str(canvas.data['highscore']))
        f.close()
    
def init(canvas):
    canvas.data['initGameOverPos']=0
    canvas.data['score']=0
    canvas.data['linecleared']=0
    canvas.data['start']=False
    canvas.data["isGameOver"] = False
    loadTetrisBoard(canvas)
    redrawAll(canvas)
    
def run():
    #create root
    root=Tk()
    root.title('Tetris')
    #create canvas
    margin=4
    cellsize=24
    #statusbar=
    cols=13
    rows=27
    canvasWidth=2*margin+cols*cellsize
    canvasHeight=2*margin+rows*cellsize
    canvas=Canvas(root, width=canvasWidth, height=canvasHeight)
    canvas.pack()
    #store canvas in root for callbacks
    root.canvas=canvas.canvas=canvas
    #create a dictionary in canvas
    canvas.data={}
    canvas.data['highscore']=0
    f=open('tetrissave.txt','r')
    for x in f:
        canvas.data['highscore']=int(x)
    f.close()
    canvas.data['initGameOverPos']=0
    canvas.data['finaGameOverPos']=200
    canvas.data['cancelmove']=[0,0]     # specifically for the two 1*3 pieces
    canvas.data['debug']=False
    canvas.data['score']=0
    canvas.data['linecleared']=0
    canvas.data['gameStarted']=False
    canvas.data['landedtwiceinarow']=0      # confirm the block is landed after checkIfLanded(canvas)==True twice 
    canvas.data['downOrpause']=1    # determine whether the piece move downward or left/right in timerFired
    canvas.data["isGameOver"] = False
    canvas.data['start']=False
    canvas.data['cols']=cols
    canvas.data['rows']=rows
    canvas.data['margin']=margin
    canvas.data['cellsize']=cellsize
    canvas.data['shape']={}
    storeShapes(canvas)
    #key event
    root.bind('<Key>', keyPress)
    #initialize
    init(canvas)
    root.mainloop()
    
run()