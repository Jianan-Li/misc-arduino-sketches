'''
Created on Feb 25, 2013

@author: Victor
'''
from Tkinter import *
import random
import copy

def keyPress(event):
    canvas=event.widget.canvas
#    if event.keysym=='d':                                # uncomment these lines to help debugging
#        canvas.data['debug']= not canvas.data['debug']
#        if canvas.data['debug']==True:
#            canvas.config(width=canvas.data['debugWidth'],height=canvas.data['debugHeight'])
#        else:
#            canvas.config(width=canvas.data['canvasWidth'],height=canvas.data['canvasHeight'])
    if (event.keysym in ['q','Q']):
        gameOver(canvas)
    elif (event.keysym in ['Shift_R','Shift_L']):
        canvas.data['enhancedGraphics'] = not canvas.data['enhancedGraphics']
    elif (event.keysym in ['s','S']) and canvas.data["isGameOver"]== False:
        canvas.data['start/pause']= not canvas.data['start/pause']
        canvas.data['gameStarted']=True
        timerFired(canvas)
    elif (event.keysym in ['r','R']):
        canvas.data['gameStarted']=False
        init(canvas)
    if canvas.data['start/pause']==True and canvas.data["isGameOver"]== False:
        if event.keysym=='Up':
            rotatePiece(canvas,'Do')
        elif event.keysym=='Left':
            movePiece(canvas,0,-1,'Do')
        elif event.keysym=='Right':
            movePiece(canvas,0,1,'Do')
        elif event.keysym=='Down':
            movePiece(canvas,1,0,'Do')
        elif event.keysym=='space':
            dropPiece(canvas)
    redrawAll(canvas)
#    if checkIfLanded(canvas)==True:            # uncomment these two lines to help debugging
#        landed(canvas)

def dropPiece(canvas):  # move the piece down block by block (not called by timerfired, so no delay) until it's landed 
    while checkIfLanded(canvas)==False: # then the canvas is redrawn in keyPress(event)
        movePiece(canvas,1,0,'Do')
                
def rotatePiece(canvas,DoOrTry):
    pieceNum=canvas.data['currentPieceNum']
    if pieceNum==1:                             # if the current piece is the 2*2 block, no rotation
        return
    piece=canvas.data['currentPiece']
    pieceSize=canvas.data['currentPieceSize']
    '''create rotated piece start'''
    rotatedPiece=[]
    for i in range(pieceSize):
        rotatedPiece+=[[0]*pieceSize]
    if canvas.data['1*4']==-1 or pieceSize==5:      # clockwise
        for x in range(pieceSize):
            for y in range(pieceSize):
                rotatedPiece[x][y]=piece[(pieceSize-1)-y][x]
    elif canvas.data['1*4']==1:                      # counterclockwise
        for x in range(pieceSize):
            for y in range(pieceSize):
                rotatedPiece[x][y]=piece[y][(pieceSize-1)-x]
    '''create rotated piece end'''
    '''apply piece to experiment board start'''
    tlrow=canvas.data['tlrow']
    tlcol=canvas.data['tlcol']
    if DoOrTry=='Try':
        tlrow=canvas.data['tlrowOnExperimentBoard']
        tlcol=canvas.data['tlcolOnExperimentBoard']
        
    previousAllNegBoard=canvas.data['previousAllNegBoard']
    ExperimentBoard=copy.deepcopy(previousAllNegBoard)      # Make all changes on this experiment board
    for row in range(tlrow,tlrow+pieceSize):
        for col in range(tlcol,tlcol+pieceSize):
            if rotatedPiece[row-tlrow][col-tlcol]!=0:
                ExperimentBoard[row][col]=rotatedPiece[row-tlrow][col-tlcol]
    '''apply piece to experiment board end'''
    if checkOutOfBoundary(canvas,ExperimentBoard)!=False or checkOverlap(canvas,previousAllNegBoard, ExperimentBoard)==True:
        if DoOrTry=='Try':
            return 'fail'
        else:
            if movePiece(canvas,0,1,'Try')=='success':
                if rotatePiece(canvas,'Try')=='success':
                    movePiece(canvas,0,1,'Do')
                    rotatePiece(canvas,'Do')
                    return 'success'
                else:
                    if canvas.data['currentPieceNum']==2:
                        if movePiece(canvas,0,2,'Try')=='success':
                            if rotatePiece(canvas,'Try')=='success':
                                movePiece(canvas,0,2,'Do')
                                rotatePiece(canvas,'Do')
                                return 'success'
            if movePiece(canvas,0,-1,'Try')=='success':
                if rotatePiece(canvas,'Try')=='success':
                    movePiece(canvas,0,-1,'Do')
                    rotatePiece(canvas,'Do')
                    return 'success'
            if movePiece(canvas,-1,0,'Try')=='success':
                if rotatePiece(canvas,'Try')=='success':
                    movePiece(canvas,-1,0,'Do')
                    rotatePiece(canvas,'Do')
                    return 'success'
                else:
                    if canvas.data['currentPieceNum']==2:
                        if movePiece(canvas,-2,0,'Try')=='success':
                            if rotatePiece(canvas,'Try')=='success':
                                movePiece(canvas,-2,0,'Do')
                                rotatePiece(canvas,'Do')
                                return 'success'
        return 'fail'    
    else:
        if DoOrTry=='Do':
            canvas.data['1*4']=-canvas.data['1*4']      # change the rotation direction for 1*4 piece after a successful rotation
            canvas.data['currentPiece']=rotatedPiece
            canvas.data['tetrisBoard']=ExperimentBoard
    return 'success'    

def movePiece(canvas,drow,dcol,DoOrTry):
    cols=canvas.data['cols']
    rows=canvas.data['rows']
    tetrisBoard=canvas.data['tetrisBoard']
    previousAllNegBoard=canvas.data['previousAllNegBoard']
    ExperimentBoard=copy.deepcopy(previousAllNegBoard)     # Make all the changes on this experiment board
    for row in range(rows):
        for col in range(cols):
            if tetrisBoard[row][col]>0:
                ExperimentBoard[row+drow][col+dcol]=tetrisBoard[row][col]
    if checkOutOfBoundary(canvas,ExperimentBoard)!=False or checkOverlap(canvas,previousAllNegBoard, ExperimentBoard)==True:
        return 'fail'
    else:
        if DoOrTry=='Do':
            canvas.data['tetrisBoard']=ExperimentBoard
            canvas.data['tlrow'] += drow
            canvas.data['tlcol'] += dcol    
        elif DoOrTry=='Try':
            canvas.data['tlrowOnExperimentBoard']=copy.deepcopy(canvas.data['tlrow'])
            canvas.data['tlcolOnExperimentBoard']=copy.deepcopy(canvas.data['tlcol'])
            canvas.data['tlrowOnExperimentBoard'] += drow
            canvas.data['tlcolOnExperimentBoard'] += dcol
    return 'success'

def checkOutOfBoundary(canvas,board):       # checks any OutOfBoundary; returns the corresponding string if indeed out of boundary
    rows=canvas.data['rows']
    for row in range(rows):
        if set(board[row][0:3])!=set([0]):
            return 'left'
        elif set(board[row][-1:-4:-1])!=set([0]):
            return 'right'
    if set(board[rows-3])!=set([0]):
        return 'bottom'
    return False

def checkOverlap(canvas, previousAllNegBoard, experimentBoard):          # checks any overlap between a previousAllNegBoard and the current experiment board
    cols=canvas.data['cols']
    rows=canvas.data['rows']
    for row in range(rows):
        for col in range(cols):
            if experimentBoard[row][col]*previousAllNegBoard[row][col]<0: # can be greater than 0 if the negative blocks don't change
                return True      
    return False
    
def checkIfLanded(canvas):
    cols=canvas.data['cols']
    rows=canvas.data['rows']
    tetrisBoard=canvas.data['tetrisBoard']
    lowestblock={}
    for x in range(rows):   
        for y in range(cols):
            if tetrisBoard[x][y]>0:         # find the lowest block of the piece in each col
                lowestblock[y]=x
    for x in lowestblock.items():
        if tetrisBoard[x[1]+1][x[0]]<0:     # if any <0 return true
            return True
    if max(lowestblock.values())==rows-4:   # if reached bottom row, return true
        return True
    return False

def landed(canvas):
    cols=canvas.data['cols']
    rows=canvas.data['rows']
    tetrisBoard=canvas.data['tetrisBoard']
    for x in range(rows):
        for y in range(cols):
            if tetrisBoard[x][y]>0:
                tetrisBoard[x][y]=-tetrisBoard[x][y]        # turn the leftovers into negative values
    checkIfClear(canvas)
    canvas.data['totalPieces'] += 1
    placePiece(canvas)
    
def checkIfClear(canvas):
    cols=canvas.data['cols']
    rows=canvas.data['rows']
    tetrisBoard=canvas.data['tetrisBoard']
    newtetrisBoard=[]
    for x in range(4,rows):
        if 0 in tetrisBoard[x][3:-3]:
            newtetrisBoard+=[tetrisBoard[x]]
    newtetrisBoard=newtetrisBoard[-1::-1]
    i=0
    while len(newtetrisBoard)<len(tetrisBoard):
        newtetrisBoard+=[[0]*cols]
        i +=1
    canvas.data['linecleared'] += (i-4)
    newtetrisBoard=newtetrisBoard[-1::-1]
    canvas.data['tetrisBoard']=newtetrisBoard
    
def placePiece(canvas):
    tetrisBoard=canvas.data['tetrisBoard']
    cols=canvas.data['cols']
    previousAllNegBoard=copy.deepcopy(tetrisBoard)
    canvas.data['previousAllNegBoard']=previousAllNegBoard           # store the all negative board for checking
    pcol=(cols-1)/2-2                                                # find the coordinates of the top left corner to place the piece
    prow=0
    if set(tetrisBoard[4][pcol:pcol+3])!=set([0]):
        gameOver(canvas)
        return
    pieceNum=random.choice(canvas.data['pieces'].keys())
    canvas.data['currentPieceNum']=pieceNum
    canvas.data['currentPiece']=canvas.data['pieces'][pieceNum]
    canvas.data['currentPieceSize']=len(canvas.data['currentPiece'])
    for row in range(prow, prow+canvas.data['currentPieceSize']):
        for col in range(pcol, pcol+canvas.data['currentPieceSize']):
            tetrisBoard[row][col]=canvas.data['currentPiece'][row-prow][col-pcol]
    canvas.data['tlrow']=prow                                        # top left corner tracking
    canvas.data['tlcol']=pcol
    
def loadBoard(canvas):
    cols=canvas.data['cols']
    rows=canvas.data['rows']
    tetrisBoard=[]
    for i in range(rows):
        tetrisBoard += [[0]*cols]
    canvas.data['tetrisBoard']=tetrisBoard
    placePiece(canvas)

'''redraw start'''
    
def redrawAll(canvas):
    canvas.delete(ALL)
    drawBoard(canvas)

def drawBoard(canvas):
    cols=range(3, canvas.data['cols']-3)    # Leave out the hidden cols and rows
    rows=range(4, canvas.data['rows']-3)
    if canvas.data['debug']==True:
        cols=range(canvas.data['cols'])
        rows=range(canvas.data['rows'])
    for row in rows:
        for col in cols:
            drawCell(canvas,row,col)
    drawCover(canvas)

def drawCell(canvas,row,col):
    tetrisBoard=canvas.data['tetrisBoard']
    margin=canvas.data['margin']
    cellSize=canvas.data['cellSize']
    left=margin+(col-3)*cellSize
    top=margin+(row-4)*cellSize
    if canvas.data['debug']==True:
        left=margin+col*cellSize
        top=margin+row*cellSize
    right=left+cellSize
    bottom=top+cellSize
    color=[]
    if tetrisBoard[row][col]==0:                # Lightred: 255,130,130   red:190,0,0   Darkred: 121,0,0 
        color='black'
        canvas.create_rectangle(left,top,right,bottom,fill=color)
    else:
        color=canvas.data['color'][tetrisBoard[row][col]]
        if canvas.data["isGameOver"]==False:
            if canvas.data['enhancedGraphics']==False:
                canvas.create_rectangle(left,top,right,bottom,fill=color[0])
            else:
                canvas.create_rectangle(left,top,right,bottom,fill=color[1])
                canvas.create_polygon([left,top,left,bottom,right,top,right,bottom],fill=color[2]) 
                canvas.create_polygon([left,top,(left+right)/2,(top+bottom)/2,right,top],fill=color[3]) 
        else:
            if canvas.data['enhancedGraphics']==False:
                canvas.create_rectangle(left,top,right,bottom,fill=random.choice(['blue','violet','red','orange','yellow','cyan','green']))
            else:
                color=random.choice(canvas.data['color'].values())
                canvas.create_rectangle(left,top,right,bottom,fill=color[1])
                canvas.create_polygon([left,top,left,bottom,right,top,right,bottom],fill=color[2]) 
                canvas.create_polygon([left,top,(left+right)/2,(top+bottom)/2,right,top],fill=color[3])                 
                
def drawCover(canvas):
    margin=canvas.data['margin']
    canvasWidth=canvas.data['canvasWidth']
    left=margin
    top=canvas.data['canvasHeight']-margin
    right=canvasWidth-margin
    bottom=top+canvas.data['statusbarSize']+margin
    canvas.create_rectangle(left,top,right,bottom,fill='gray')
    canvas.create_text(65,top+10, text="Move - Arrow keys", font=("Helvetica", 13, "bold"),fill='black')
    canvas.create_text(62,top+24, text="Rotate - Up Arrow", font=("Helvetica", 13, "bold"),fill='black')
    canvas.create_text(58,top+38, text="Drop - Spacebar", font=("Helvetica", 13, "bold"),fill='black')
    canvas.create_text(54,top+52, text="Start/Pause - S", font=("Helvetica", 13, "bold"),fill='black')
    canvas.create_text(35,top+66, text="Reset - R", font=("Helvetica", 13, "bold"),fill='black')
    canvas.create_text(95,top+66, text="Quit - Q", font=("Helvetica", 13, "bold"),fill='black')
    # score
    score=canvas.data['linecleared']*10 + canvas.data['totalPieces']*1      
    canvas.data['score']=score
    if canvas.data['highscore']<canvas.data['score']:
        canvas.data['highscore']=canvas.data['score']
    highscore=canvas.data['highscore']
    canvas.create_text(right-67,top+24, text="Score:", font=("Helvetica", 18, "bold"),fill='black')
    canvas.create_text(right-19,top+24, text="%d" % (score), font=("Helvetica", 18, "bold"),fill='Black')
    canvas.create_text(right-90,top+54, text="High Score:", font=("Helvetica", 18, "bold"),fill='black')
    canvas.create_text(right-19,top+54, text="%d" % (highscore), font=("Helvetica", 18, "bold"),fill='Black')
    if canvas.data["isGameOver"] == True:
        drawGameOver(canvas)
    # the pause sign
    if canvas.data['start/pause']==False and canvas.data["isGameOver"] == False and canvas.data['gameStarted']==True:
        canvas.create_text((left+right)/2, top-400, text="||", font=("Helvetica", 72, "bold"),fill='white')

def drawGameOver(canvas):
    cols=canvas.data['cols']
    margin=canvas.data['margin']
    cellsize=canvas.data['cellSize']
    left=margin
    right=left+(cols-6)*cellsize
    top=margin
    bottom=top+3*cellsize
    position=canvas.data['initGameOverPos']
    if position<canvas.data['finaGameOverPos']:
        position+=3
        canvas.data['initGameOverPos']+=3
    canvas.create_text((left+right)/2, position, text="Game Over", font=("Helvetica", 31, "bold"),fill='white')
    delay=30
    canvas.after(delay,redrawAll,canvas)
    
'''redraw end'''

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
    if canvas.data['start/pause']==True and canvas.data["isGameOver"]==False:
        movePiece(canvas,downOrpause,0,'Do')
        redrawAll(canvas)
        linecleared=canvas.data['linecleared']
        delay=400-10*linecleared     # change difficulty here~~~~~~~~~
        if canvas.data['downOrpause']==0:
            delay=350
        canvas.after(delay, timerFired, canvas)
        
def gameOver(canvas):
    canvas.data["isGameOver"] = True
    saveHighscore(canvas)
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
    canvas.data['totalPieces'] =0 
    canvas.data['linecleared']=0
    canvas.data['initGameOverPos']=0
    canvas.data["isGameOver"]=False
    canvas.data['start/pause']=False
    loadBoard(canvas)
    redrawAll(canvas)

def storePieces(canvas):
    canvas.data['pieces']={}
    canvas.data['pieces'][1]=[[0]*4,[0]*4,[0,0,1,1],[0,0,1,1]]              # Square 4*4
    canvas.data['pieces'][2]=[[0,0,2,0],[0,0,2,0],[0,0,2,0],[0,0,2,0]]      # I      4*4
    canvas.data['pieces'][3]=[[0]*5,[0]*5,[0,0,3,0,0],[0,3,3,3,0],[0]*5]    # All the followings are 5*5
    canvas.data['pieces'][4]=[[0]*5,[0]*5,[0,4,0,0,0],[0,4,4,4,0],[0]*5]
    canvas.data['pieces'][5]=[[0]*5,[0]*5,[0,0,0,5,0],[0,5,5,5,0],[0]*5]
    canvas.data['pieces'][6]=[[0]*5,[0]*5,[0,6,6,0,0],[0,0,6,6,0],[0]*5]
    canvas.data['pieces'][7]=[[0]*5,[0]*5,[0,0,7,7,0],[0,7,7,0,0],[0]*5]

def makeMenus(root):
    menubar = Menu(root)
    filemenu = Menu(menubar)
    root.config(menu=menubar)
    
def run(cols, rows):
    root=Tk()
    root.title('Tetris')
    makeMenus(root)
    margin=4
    cellSize=24
    statusbarSize=70
    canvasWidth=2*margin+cols*cellSize
    canvasHeight=2*margin+rows*cellSize
    canvas=Canvas(root, width=canvasWidth, height=canvasHeight+statusbarSize)
    canvas.pack()
    ws = root.winfo_screenwidth()
    x = (ws/2) - (canvasWidth/2) 
    root.geometry('+%d+%d' % (0, 22))
    root.canvas=canvas.canvas=canvas    #!! for callbacks
    canvas.data={}
    canvas.data['highscore']=0
    f=open('tetrissave.txt','r')
    for x in f:
        canvas.data['highscore']=int(x)
    f.close()
    
    #size info
    canvas.data['margin']=margin
    canvas.data['cellSize']=cellSize
    canvas.data['cols']=cols+6          # IMPORTANT: 6 hidden cols; 3 on each side
    canvas.data['rows']=rows+7          # IMPORTANT: 7 hidden rows; 4 on top, 3 on bottom
    canvas.data['canvasWidth']=canvasWidth
    canvas.data['canvasHeight']=canvasHeight        # not including the status bar height
    canvas.data['statusbarSize']=statusbarSize
    canvas.data['debugWidth']=canvasWidth+6*cellSize
    canvas.data['debugHeight']=canvasHeight+7*cellSize
    canvas.data['initGameOverPos']=0
    canvas.data['finaGameOverPos']=cellSize*rows/2
    
    #various counters
    canvas.data['totalPieces'] =0 
    canvas.data['linecleared']=0
    canvas.data['score']=0
    
    # indicators
    canvas.data['downOrpause']=1
    canvas.data['landedtwiceinarow']=0  
    canvas.data['1*4']=1                # track the rotation of the 1*4 piece    
    
    # True/False
    canvas.data['enhancedGraphics']=False
    canvas.data['gameStarted']=False     # only used for not showing the 'pause' sign after the game has been reset
    canvas.data['start/pause']=False            
    canvas.data["isGameOver"]=False
    canvas.data['debug']=False
    #color dictionary
    canvas.data['color']={}
    canvas.data['color'][1]=['blue','#000079','#0000BF','#8383FF']
    canvas.data['color'][2]=['violet','#790079','#BF00BF','#FF83FF']
    canvas.data['color'][3]=['red','#790000','#BF0000','#FF8383']   
    canvas.data['color'][4]=['orange','#793D00','#BF5F00','#FFC183']
    canvas.data['color'][5]=['yellow','#797900','#BFBF00','#FFFF83']
    canvas.data['color'][6]=['green','#007900','#00BF00','#83FF83']
    canvas.data['color'][7]=['cyan','#007979','#00BFBF','#83FFFF']
    canvas.data['color'][-1]=canvas.data['color'][1]
    canvas.data['color'][-2]=canvas.data['color'][2]
    canvas.data['color'][-3]=canvas.data['color'][3]
    canvas.data['color'][-4]=canvas.data['color'][4]
    canvas.data['color'][-5]=canvas.data['color'][5]
    canvas.data['color'][-6]=canvas.data['color'][6]
    canvas.data['color'][-7]=canvas.data['color'][7]

    storePieces(canvas)
    #key event
    root.bind('<Key>', keyPress)
    init(canvas)
    root.mainloop()
    
run(11,22)