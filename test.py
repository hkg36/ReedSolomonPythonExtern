import sys
sys.path.append('build')

import reedsolomon
import random
print reedsolomon.NPAR
while True:
    tMesg = ""
    for i in xrange(30):
        tMesg+=random.choice('1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ')
    tSize = 6
    result= reedsolomon.RSEncode(tMesg)
    reslist=list(result)
    errorcount=random.randint(1,3)
    errseted=set()
    for i in xrange(errorcount):
        while True:
            pos=random.randint(0,len(result)-1)
            if pos not in errseted:
                errseted.add(pos)
                break
        print pos,',',
        reslist[pos]='\x00'
    print ''
    result1=''.join(reslist)
    print result1
    result2= reedsolomon.RSDecode(result1)
    print result2
    for i in xrange(len(result2)):
        if result2[i]!=result1[i]:
            print 'error'
            exit()