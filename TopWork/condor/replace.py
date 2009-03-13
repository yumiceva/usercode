import sys,os

#________________________________________________________________
def get_list_files(directory,pattern = ""):

        dir = []

        dir = os.listdir(directory)
        
        lfiles = []
        for f in dir:

            if f.find(pattern) != -1 and f.endswith("py"):

                lfiles.append(directory+"/"+f)

        lfiles.sort()
        return lfiles

                                                        
def main():

    if len(sys.argv) < 2:
        print" [usage] replace.py <flag=btag,JPT> <on/off> <pattern>"
        sys.exit()


    flag = sys.argv[1]
    switch = sys.argv[2]
    pattern = sys.argv[3]

    
    files = get_list_files(".", pattern)

    print files
    
    for f in files:

        newf = file("replace.tmp", 'w')

        infile = file(f)
        
        for line in infile:

            if line.find('UsebTagging')!=-1 and flag=='btag':

                if switch=='on':
                    line = line.strip('\#')
                else:
                    line = '#'+line

                print "line replaced"

            if line.find('jetSource')!=-1 and flag=='JPT':

                if switch=='on':
                    line = line.strip('\#')
                else:
                    line = '#'+line
                    
                print "line replaced"

                                                                        
            newf.write(line)

        newf.close()
        os.rename('replace.tmp',f)
        
if __name__ =='__main__':
    sys.exit(main())
