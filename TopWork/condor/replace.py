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

    dict = {'btag':"process.TopAnalyzer.UsebTagging  = cms.bool(True)",'JPT':"process.TopAnalyzer.jetSource      = cms.InputTag('selectedLayer1JetsJPT')",'MET':"process.TopAnalyzer.METCuts.Recalculate  = cms.bool(True)",'JESUp':"process.TopAnalyzer.jetCuts.JES = cms.double(1.1)",'JESDown':"process.TopAnalyzer.jetCuts.JES = cms.double(0.9)",'JetCuts':"process.TopAnalyzer.jetCuts.ApplyAsymmetricCuts = cms.bool(True);process.TopAnalyzer.muonCuts.MinPt = cms.double(30.)",'mtop':"process.TopAnalyzer.UseMtopConstraint  = cms.bool(False)",'lyon':"process.TopAnalyzer.jetCuts.MinJetPt = cms.double(35.);process.TopAnalyzer.muonCuts.MinPt = cms.double(30.)"}
    
    if len(sys.argv) < 2:
        print" [usage] replace.py <flag=JetCuts,JESUp,JESDown,btag,JPT,MET,mtop> <on/off> <pattern>"
	print " flags: "
	print dict
        sys.exit()


    flag = sys.argv[1]
    switch = sys.argv[2]
    pattern = sys.argv[3]

    
    files = get_list_files(".", pattern)
    #print "files found:"
    #print files
    
    for f in files:

        newf = file("replace.tmp", 'w')

        infile = file(f)
        
        for line in infile:

		if switch=='off':
			
			if line.find('UsebTagging')!=-1 and flag=='btag': line = ''

			if line.find('UseMtopConstraint')!=-1 and flag=='mtop': line = ''
			
			if line.find('jetSource')!=-1 and flag=='JPT': line = ''
                    
			if line.find('METCuts')!=-1 and flag=='MET': line = ''
			    
			if line.find('JES')!= -1 and flag == "JESUp": line= ''

			if line.find('JES')!= -1 and flag == "JESDown": line= ''
														
			if line.find('ApplyAsymmetricCuts')!=-1 and flag=='JetCuts': line=''
									    
			if line.find('muonCuts')!=-1 and flag=='JetAsymmetric': line = ''
													
                                                                        
		newf.write(line)
	
	if switch=='on':
		anewline = ''
		for ikey in dict.keys():
			if flag==ikey:
				anewline=dict[ikey]
		newf.write(anewline)
		newf.write("\n")
		
        newf.close()
        os.rename('replace.tmp',f)
        
if __name__ =='__main__':
    sys.exit(main())
