'''Started: Jan 23, 2013'''
import string,random,math

def game():
    wordlist=open('wordlist.txt')
    wordlist=[x.strip() for x in wordlist]
    abc=string.lowercase
    word=''
    possible_result=wordlist
    '''flip a coin'''
    print 'Flip a coin.....\n'
    coin=math.floor(random.random()*2)
    if coin==0:
        print "It's head. You choose the first letter."
    else:
        print "It's tail. Computer choose the first letter."
        com_letter=random.choice(abc)
        print 'Computer says letter %s.' % (com_letter)
        word=com_letter
        print word+'...\n'
        possible_result=[x for x in wordlist if x.startswith(com_letter)]
    while True:
        d={}
        player_letter=str(raw_input("Your choice: "))
        word=word+player_letter
        print word+'...\n'
        possible_result=[x for x in possible_result if x.startswith(word)]
        if word in possible_result:
            print '%s is a word. You lose.' % (word)
            return
        if len(possible_result)==0:
            print "There is no word starting with %s. You lose." % (word)
            return
        for x in abc:
            possible_move=word+x
            consequence=[m for m in possible_result if m.startswith(possible_move) and len(m[len(possible_move):]) % 2 ==1]
            d[possible_move]=len(consequence)
        best_move=d.keys()[d.values().index(max(d.values()))]
        while True:
            if best_move in possible_result:
                d[best_move]=0
                best_move=d.keys()[d.values().index(max(d.values()))]
            else:
                break
        com_letter=best_move[-1]
        if d.values()==[0]*26:
            p={}
            for x in abc:
                possible_move=word+x
                badconsequence=[m for m in possible_result if m.startswith(possible_move)]
                p[possible_move]=len(badconsequence)
            best_move=p.keys()[p.values().index(max(p.values()))]
            com_letter=best_move[-1]
            if set([len(x)-len(word) for x in possible_result])==set([1]):
                com_letter=random.choice(possible_result)[-1]
                word=word+com_letter
                print 'Computer says letter %s.' % (com_letter)
                print "%s is a word. You win!" % (word)
                return
        print "Computer says letter %s." % (com_letter)
        if best_move in possible_result:
            print "%s is a word. You win!" % (best_move)
            return
        word=best_move
        print word+'...\n'
        possible_result=[x for x in possible_result if x.startswith(best_move)]
        
if __name__=='__main__':
    again='y'
    while again=='y':
        game()
        again=str(raw_input('Want to play again? (y/n) : '))
        