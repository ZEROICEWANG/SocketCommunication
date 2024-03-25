from functools import partial

import os
os.chdir('E://file//pdata')
floders=os.listdir()
for floder in floders:
    with open(floder, 'rb+') as file:
        data_b=file.read()
        print(data_b)
        '''records=iter(partial(data_b,2),b'')
        i=0
        for r in records:
            j=0
            r_int=int.from_bytes(r,bytecorder='big')
            i+=1
            print('i={0}:{1}'.format(i,r_int))
        '''
        with open(floder+'_t.txt','w+') as file:
            file.write(str(data_b))


