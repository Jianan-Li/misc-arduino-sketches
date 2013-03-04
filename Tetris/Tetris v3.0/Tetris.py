'''
Made by Jianan Li
Duke Class of 2016
Email: jianan.li@duke.edu
It's will be very much appreciated if you can report the bugs you find in this program.
'''
from Tkinter import *
import random
import copy
from pygame import mixer

def keyPress(event):
    canvas=event.widget.canvas
    bgm=event.widget.bgm
    if event.keysym=='0':                                
        canvas.data['debug']= not canvas.data['debug']
        if canvas.data['debug']==True:
            canvas.config(width=canvas.data['debugWidth'],height=canvas.data['debugHeight'], background='white')
        else:
            canvas.config(width=canvas.data['canvasWidth']+canvas.data['sidebarSize'],height=canvas.data['canvasHeight']+canvas.data['statusbarSize'],background='black')
    elif (event.keysym in ['q','Q']):
        gameOver(canvas)
    elif (event.keysym in ['Shift_R','Shift_L']):
        canvas.data['enhancedGraphics'] = not canvas.data['enhancedGraphics']
    elif (event.keysym in ['s','S']) and canvas.data["isGameOver"]== False:        
        canvas.data['start/pause']= not canvas.data['start/pause']
        canvas.data['gameStarted']=True
        if canvas.data['start/pause']==True and canvas.data["isGameOver"]==False:
            bgm.unpause()
            timerFired(canvas)
        else:
            bgm.pause()
    elif (event.keysym in ['r','R']):
        bgm.stop()
        canvas.data['gameStarted']=False
        init(canvas,bgm)
    if canvas.data['start/pause']==True and canvas.data["isGameOver"]== False and canvas.data['generatingBoss']==False:
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
    ExperimentBoard=copy.deepcopy(previousAllNegBoard)          # Make all changes on this experiment board
    for row in range(tlrow,tlrow+pieceSize):
        for col in range(tlcol,tlcol+pieceSize):
            if rotatedPiece[row-tlrow][col-tlcol]!=0:
                ExperimentBoard[row][col]=rotatedPiece[row-tlrow][col-tlcol]
    '''apply piece to experiment board end'''
    if checkOutOfBoundary(canvas,ExperimentBoard)==True or checkOverlap(canvas,previousAllNegBoard, ExperimentBoard)==True:
        if DoOrTry=='Try':
            return 'fail'
        else:
            for i in [[0,1],[0,-1],[-1,0]]:
                if movePiece(canvas,i[0],i[1],'Try')=='success':
                    if rotatePiece(canvas,'Try')=='success':
                        movePiece(canvas,i[0],i[1],'Do')
                        rotatePiece(canvas,'Do')
                        return 'success'
                    else:
                        if canvas.data['currentPieceNum']==2:
                            if movePiece(canvas,2*i[0],2*i[1],'Try')=='success':
                                if rotatePiece(canvas,'Try')=='success':
                                    movePiece(canvas,2*i[0],2*i[1],'Do')
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
        if set(board[row][0:3]+board[row][-1:-4:-1])!=set([0]):
            return True
    if set(board[rows-3])!=set([0]):
        return True
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
    if lowestblock=={}:
        return True                         # after creating the boss
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
    if canvas.data['generatingBoss']==False:
        placePiece(canvas)

'''Boss Start'''
        
def moveBoss(canvas,boss,i):
    cols=canvas.data['cols']
    rows=canvas.data['rows']
    tetrisBoard=canvas.data['tetrisBoard']
    newBoard=tetrisBoard[0:rows-3][-1::-1]
    newBoard=newBoard[0:rows-4][-1::-1]+boss[i-1:i]
    for x in range(3):
        newBoard += [[0]*cols]
    tetrisBoard=newBoard
    canvas.data['tetrisBoard']=tetrisBoard
    redrawAll(canvas)
    if i==len(boss):
        canvas.data['generatingBoss']=False
        placePiece(canvas)
        timerFired(canvas)
        return
    i += 1
    delay=300
    canvas.after(delay, moveBoss, canvas, boss,i)
    
def generateBosses(canvas):
    bossNum=random.choice(canvas.data['bosses'].keys())
    boss=canvas.data['bosses'][bossNum]
    i=1                                       # just a counter
    moveBoss(canvas,boss,i)

def changeBossStatus(canvas):
    tetrisBoard=canvas.data['tetrisBoard']
    cols=canvas.data['cols']
    rows=canvas.data['rows']
    canvas.data['previousBossStatus']=canvas.data['currentBossStatus']
    for row in range(rows):
        for col in range(cols):
            if tetrisBoard[row][col]<-10:
                canvas.data['currentBossStatus']=True
                return
    canvas.data['currentBossStatus']=False
    
def resetLinesTillNextBoss(canvas):
    if canvas.data['previousBossStatus']==False and canvas.data['currentBossStatus']==True:
        canvas.data['linesTillNextBoss']=5
 
'''Boss End'''
               
def checkIfClear(canvas):
    cols=canvas.data['cols']
    rows=canvas.data['rows']
    tetrisBoard=canvas.data['tetrisBoard']
    newtetrisBoard=[]
    for x in range(rows-1,0-1,-1):
        if 0 in tetrisBoard[x][3:-3]:
            newtetrisBoard+=[tetrisBoard[x]]
    i=0
    while len(newtetrisBoard)<len(tetrisBoard):
        newtetrisBoard+=[[0]*cols]
        i +=1
    canvas.data['linecleared'] += i
    if i>=3: canvas.data['3/4linesBonus'] += i/2 
    newtetrisBoard=newtetrisBoard[-1::-1]
    canvas.data['tetrisBoard']=newtetrisBoard
    changeBossStatus(canvas)
    if canvas.data['previousBossStatus']==False and canvas.data['currentBossStatus']==False:
        canvas.data['linesTillNextBoss'] = canvas.data['linesTillNextBoss'] - i
    resetLinesTillNextBoss(canvas)
    if canvas.data['linesTillNextBoss']<=0:
        canvas.data['generatingBoss']=True
        generateBosses(canvas)
    
def placePiece(canvas):
    tetrisBoard=canvas.data['tetrisBoard']
    cols=canvas.data['cols']
    previousAllNegBoard=copy.deepcopy(tetrisBoard)
    canvas.data['previousAllNegBoard']=previousAllNegBoard           # store the all negative board for later comparison
    pcol=(cols-1)/2-2                                                # find the coordinates of the top left corner to place the piece
    prow=0
    if set(tetrisBoard[3])!=set([0]):
        gameOver(canvas)
        return
    canvas.data['totalPieces'] += 1
    if canvas.data['totalPieces']==0:
        nextPieceNum=random.choice(canvas.data['pieces'].keys())
        canvas.data['nextPieceNum']=nextPieceNum
        canvas.data['nextPiece']=canvas.data['pieces'][nextPieceNum]
        canvas.data['nextPieceSize']=len(canvas.data['nextPiece'])
    canvas.data['currentPieceNum']=canvas.data['nextPieceNum']
    canvas.data['currentPiece']=canvas.data['nextPiece']
    canvas.data['currentPieceSize']=canvas.data['nextPieceSize']
    for row in range(prow, prow+canvas.data['currentPieceSize']):
        for col in range(pcol, pcol+canvas.data['currentPieceSize']):
            tetrisBoard[row][col]=canvas.data['currentPiece'][row-prow][col-pcol]
    canvas.data['tlrow']=prow                                        # reset top left corner position for tracking
    canvas.data['tlcol']=pcol
    nextPieceNum=random.choice(canvas.data['pieces'].keys())
    canvas.data['nextPieceNum']=nextPieceNum
    canvas.data['nextPiece']=canvas.data['pieces'][nextPieceNum]
    canvas.data['nextPieceSize']=len(canvas.data['nextPiece'])
    
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
    if canvas.data['debug']==True:          # debug
        cols=range(canvas.data['cols'])
        rows=range(canvas.data['rows'])
    for row in rows:
        for col in cols:
            drawCell(canvas,row,col)
    if canvas.data['debug']==False:
        drawSidebar(canvas)
        drawCover(canvas)

def drawCell(canvas,row,col):
    cols=canvas.data['cols']    # Leave out the hidden cols and rows
    rows=canvas.data['rows']
    tetrisBoard=canvas.data['tetrisBoard']
    margin=canvas.data['margin']
    cellSize=canvas.data['cellSize']
    left=margin+(col-3)*cellSize
    top=margin+(row-4)*cellSize
    if canvas.data['debug']==True:          # debug
        left=margin+col*cellSize    
        top=margin+row*cellSize
    right=left+cellSize
    bottom=top+cellSize
    if tetrisBoard[row][col]==0:                
        color='black'
        if canvas.data['debug']==True:
            color='white'
            if col in [0,1,2,cols-3,cols-2,cols-1] or row in [rows-3,rows-2,rows-1]:
                color='gray'
        canvas.create_rectangle(left,top,right,bottom,fill=color)
    else:
        color=canvas.data['color'][tetrisBoard[row][col]]
        if canvas.data["isGameOver"]==True:
            color=random.choice(canvas.data['color'].values())
        if canvas.data['enhancedGraphics']==False:
            canvas.create_rectangle(left,top,right,bottom,fill=color[0])
        else:
            canvas.create_rectangle(left,top,right,bottom,fill=color[1])
            canvas.create_polygon([left,top,left,bottom,right,top,right,bottom],fill=color[2]) 
            canvas.create_polygon([left,top,(left+right)/2,(top+bottom)/2,right,top],fill=color[3])
    if canvas.data['debug']==True:
        canvas.create_text((left+right)/2,(top+bottom)/2,text=tetrisBoard[row][col],font=("Helvetica", 13, "bold"),fill='black')
                
def drawCover(canvas):
    margin=canvas.data['margin']
    canvasWidth=canvas.data['canvasWidth']
    left=margin
    top=canvas.data['canvasHeight']-margin
    right=canvasWidth
    bottom=top+canvas.data['statusbarSize']+margin
    canvas.create_rectangle(left,top,right+4,top+4,outline='light gray', fill='light gray')
    canvas.create_text(65,top+14, text="Move - Arrow keys", font=("Helvetica", 13, "bold"),fill='white')
    canvas.create_text(62,top+28, text="Rotate - Up Arrow", font=("Helvetica", 13, "bold"),fill='white')
    canvas.create_text(58,top+42, text="Drop - Spacebar", font=("Helvetica", 13, "bold"),fill='white')
    canvas.create_text(54,top+56, text="Start/Pause - S", font=("Helvetica", 13, "bold"),fill='white')
    canvas.create_text(35,top+70, text="Reset - R", font=("Helvetica", 13, "bold"),fill='white')
    canvas.create_text(95,top+70, text="Quit - Q", font=("Helvetica", 13, "bold"),fill='white')
    # score
    score=(canvas.data['linecleared']+canvas.data['3/4linesBonus'])*10 + (canvas.data['totalPieces'])
    canvas.data['score']=score
    if canvas.data['highscore']<canvas.data['score']:
        canvas.data['highscore']=canvas.data['score']
    highscore=canvas.data['highscore']
    canvas.create_text(right-27,top+24, text="Score:", font=("Helvetica", 18, "bold"),fill='white')
    canvas.create_text(right+36,top+24, text="%d" % (score), font=("Helvetica", 18, "bold"),fill='white')
    canvas.create_text(right-50,top+54, text="High Score:", font=("Helvetica", 18, "bold"),fill='white')
    canvas.create_text(right+36,top+54, text="%d" % (highscore), font=("Helvetica", 18, "bold"),fill='white')
    # Game Over
    if canvas.data["isGameOver"] == True:
        drawGameOver(canvas)
    # the pause sign
    if canvas.data['start/pause']==False and canvas.data["isGameOver"] == False and canvas.data['gameStarted']==True:
        canvas.create_text((left+right)/2, top-350, text="||", font=("Helvetica", 72, "bold"),fill='white')

def drawSidebar(canvas):
    sidebarSize=canvas.data['sidebarSize']
    margin=canvas.data['margin']
    stop=margin
    sbottom=canvas.data['canvasHeight']+canvas.data['statusbarSize']
    sleft=canvas.data['canvasWidth']-margin
    sright=sleft+sidebarSize+margin
    canvas.create_rectangle(sleft,stop,sleft+4,sbottom-canvas.data['statusbarSize'],outline='light gray', fill='light gray')
    canvas.create_text((sleft+sright)/2-2,stop+sbottom*1/10,font=("Helvetica", 20, "bold"),text='Next', fill='white')
    nextPiece=canvas.data['nextPiece']
    if canvas.data['gameStarted']==False and canvas.data['currentPiece']!=[]:
        nextPiece=canvas.data['currentPiece']
    for row in range(len(nextPiece)):
        for col in range(len(nextPiece)):
            if nextPiece[row][col]!=0:
                color=canvas.data['color'][nextPiece[row][col]]
                top=85+15*row
                bottom=top+15
                left=sleft+15*col
                if nextPiece[row][col]==1:
                    left=sleft-7+15*col 
                right=left+15
                canvas.create_rectangle(left,top,right,bottom,fill=color[1])
                canvas.create_polygon([left,top,left,bottom,right,top,right,bottom],fill=color[2]) 
                canvas.create_polygon([left,top,(left+right)/2,(top+bottom)/2,right,top],fill=color[3])                
                    
def drawGameOver(canvas):
    cols=canvas.data['cols']
    margin=canvas.data['margin']
    cellsize=canvas.data['cellSize']
    left=margin
    right=left+(cols-6)*cellsize
    position=canvas.data['initGameOverPos']
    if position<canvas.data['finaGameOverPos']:
        position+=3
        canvas.data['initGameOverPos']+=3
    canvas.create_text((left+right)/2, position, text="Game Over", font=("Helvetica", 31, "bold"),fill='white')
    delay=30
    canvas.after(delay,redrawAll,canvas)
    
'''redraw end'''

def timerFired(canvas):                         # Confirm the block is landed only after checkIfLanded(canvas)==True twice in a row
    if checkIfLanded(canvas)==True:             # which means that you have one delay interval (<350ms) to move the block 
        canvas.data['nextMove']=0               # left or right before the landing is confirmed and a new piece is generated
        canvas.data['landedtwiceinarow'] +=1
    else:
        canvas.data['nextMove']=1
        canvas.data['landedtwiceinarow']=0
    nextMove=canvas.data['nextMove']
    if canvas.data['landedtwiceinarow']>=2:     # landing is confirmed here
        canvas.data['landedtwiceinarow']=0
        landed(canvas)
    if canvas.data['start/pause']==True and canvas.data["isGameOver"]==False and canvas.data['generatingBoss']==False:    
        movePiece(canvas,nextMove,0,'Do')
        redrawAll(canvas)
        linecleared=canvas.data['linecleared']
        delay=400-3*linecleared                 # change difficulty here~~~~~~~~~
        if canvas.data['nextMove']==0: delay=350
        canvas.after(delay, timerFired, canvas)
        
def gameOver(canvas):
    canvas.data["isGameOver"] = True
    redrawAll(canvas)
    saveHighscore(canvas)

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
    
def init(canvas,bgm):
    canvas.data['totalPieces'] =-1
    canvas.data['linecleared']=0
    canvas.data['3/4linesBonus']=0
    canvas.data['initGameOverPos']=0
    canvas.data["isGameOver"]=False
    canvas.data['start/pause']=False
    canvas.data['generatingBoss']=False
    canvas.data['previousBossStatus']=False
    canvas.data['currentBossStatus']=False
    canvas.data['linesTillNextBoss']=5
    canvas.data['currentPiece']=[]
    bgm.play(-1)
    bgm.pause()
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
    canvas.data['pieces'][8]=[[0,0,2,0],[0,0,2,0],[0,0,2,0],[0,0,2,0]]      # I      4*4

def storebosses(canvas):
    canvas.data['bosses']={}
    creeper=[]
    for row in range(8):
        creeper += [[0]*4+[-18]*8+[0]*4]
    for row in range(2,8):
        for col in range(5,11):
            if [row,col] in [[2,5],[2,6],[2,9],[2,10],[3,5],[3,6],[3,9],[3,10],[4,7],[4,8],[7,6],[7,9]] or (row in [5,6] and col in [6,7,8,9]):
                creeper[row][col]=0
    canvas.data['bosses'][0]=creeper
    
    skeleton=[]
    for row in range(8):
        skeleton += [[0]*4+[-11]*8+[0]*4]
    for row in range(4,7):
        for col in range(5,11):
            if [row,col] in [[4,5],[4,6],[4,9],[4,10]] or (row==6 and col in range(5,11)):
                skeleton[row][col]=0
    for col in [7,8]:
        skeleton[5][col]= -12
    canvas.data['bosses'][1]=skeleton
    
    zombie=[]
    for row in range(8):
        zombie += [[0]*4+[-14]*8+[0]*4]
    for col in [5,6,9,10]:
        zombie[4][col]=0
    for col in [7,8]:
        zombie[5][col]=-13
    canvas.data['bosses'][2]=zombie
        
    pig=[]
    for row in range(8):
        pig += [[0]*4+[-15]*8+[0]*4]
    for col in [4,11]:
        pig[3][col]=0
    for col in [5,10]:
        pig[3][col]=-11
    for row in [4,6]:
        for col in [6,7,8,9]:
            pig[row][col]=-17
    for col in [6,9]:
        pig[5][col]=-16
    canvas.data['bosses'][3]=pig
    
    mario=[]
    for row in range(2):
        mario += [[0]*3+[-19]*9+[0]*4]
    for row in range(5):
        mario += [[0]*3+[-20]*10+[0]*3]
    for [row,col] in [[0,3],[0,9],[0,10],[0,11],[2,8],[2,10],[2,11],[2,12],[3,8],[3,12],[5,12]]:
        mario[row][col]=0
    for [row,col] in [[4,9]]:
        mario[row][col]=0
    for col in range(8,12):
        mario[5][col]=0
    for col in [3,11,12]:
        mario[6][col]=0
    for [row,col] in [[2,3],[2,4],[2,5],[3,4],[4,4],[4,5],[5,3]]:
        mario[row][col]=-16
    canvas.data['bosses'][4]=mario
    
def makeMenus(root):
    menubar = Menu(root)
    root.config(menu=menubar)
    
def run(cols, rows):
    root=Tk()
    root.title('Tetris')
    makeMenus(root)
    margin=0
    cellSize=26
    statusbarSize=76
    sidebarSize=70
    canvasWidth=2*margin+cols*cellSize
    canvasHeight=2*margin+rows*cellSize
    canvas=Canvas(root, width=canvasWidth+sidebarSize, height=canvasHeight+statusbarSize)
    canvas.config(background='black',highlightthickness=0)       # black and borderless
    canvas.pack()
    sw = root.winfo_screenwidth()
    sh = root.winfo_screenheight()
    x = (sw/2) - (canvasWidth+sidebarSize)/2
    y = (sh/2) - (canvasHeight+statusbarSize)/2-60
    root.geometry('+%d+%d' % (x, y))
    root.resizable(width=FALSE, height=FALSE)
    root.canvas=canvas.canvas=canvas    #!! for callbacks
    canvas.data={}
                                    
    canvas.data['highscore']=0          # get high score from save file. The file might be empty, so high score is initialized as 0
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
    canvas.data['sidebarSize']=sidebarSize
    canvas.data['debugWidth']=canvasWidth+6*cellSize
    canvas.data['debugHeight']=canvasHeight+7*cellSize
    canvas.data['initGameOverPos']=0
    canvas.data['finaGameOverPos']=cellSize*rows/2
    canvas.data['currentPiece']=[]
    
    #various counters
    canvas.data['totalPieces'] =-1
    canvas.data['linecleared']=0
    canvas.data['3/4linesBonus']=0
    canvas.data['score']=0
    
    # indicators
    canvas.data['nextMove']=1           # can only be changed by timerFired(canvas)
    canvas.data['landedtwiceinarow']=0  
    canvas.data['1*4']=1                # track the rotation of the 1*4 piece    
    
    # True/False
    canvas.data['enhancedGraphics']=True
    canvas.data['gameStarted']=False     # only used for not showing the 'pause' sign after the game has been reset
    canvas.data['start/pause']=False            
    canvas.data["isGameOver"]=False
    canvas.data['debug']=False
    canvas.data['generatingBoss']=False
    canvas.data['previousBossStatus']=False
    canvas.data['currentBossStatus']=False
    
    #color dictionary
    canvas.data['color']={}
    canvas.data['color'][1]=['blue','#000079','#0000BF','#8383FF']
    canvas.data['color'][2]=['violet','#790079','#BF00BF','#FF83FF']
    canvas.data['color'][3]=['red','#790000','#BF0000','#FF8383']       # Lightred: 255,130,130   red:190,0,0   Darkred: 121,0,0 
    canvas.data['color'][4]=['orange','#793D00','#BF5F00','#FFC183']
    canvas.data['color'][5]=['yellow','#797900','#BFBF00','#FFFF83']
    canvas.data['color'][6]=['green','#007900','#00BF00','#83FF83']
    canvas.data['color'][7]=['cyan','#007979','#00BFBF','#83FFFF']
    for x in range(1,8):
        canvas.data['color'][-x]=canvas.data['color'][x]
    canvas.data['color'][-11]=['white','#BEBEBE','#DCDCDC','#FFFFFF']       # 190, 220, 255
    canvas.data['color'][-12]=['gray','#5A5A5A','#787878','#969696']        # 90, 120, 150
    canvas.data['color'][-13]=['darkgreen','#003200','#004600','#006400']   # 50, 70, 100 g
    canvas.data['color'][-14]=['darkgreen','#004600','#006400','#007900']   # 70, 100, 121 g
    canvas.data['color'][-15]=['#DCA0A0','#C88C8C','#DCA0A0','#F0B4B4']     # pig pink
    canvas.data['color'][-16]=['#9B5050','#873C3C','#9B5050','#AF6464']     # pig nose dark also mario hair
    canvas.data['color'][-17]=['#EFC4C4','#D7B0B0','#EFC4C4','#FFD8D8']     # pig nose bright
    canvas.data['color'][-18]=canvas.data['color'][6]                       # creeper green
    canvas.data['color'][-19]=canvas.data['color'][3]                       # mario red
    canvas.data['color'][-20]=['#F0BE8C','#DCAA78','#F0BE8C','#FFD2A0']     # mario skin
    storePieces(canvas)
    storebosses(canvas)
    #key event
    root.bind('<Key>', keyPress)
    mixer.init(44100, -16, 2, 1024)
    bgm=mixer.music
    bgm.load('Tetris.ogg')
    root.bgm=bgm
    init(canvas,bgm)
    root.mainloop()
    
run(10,22)