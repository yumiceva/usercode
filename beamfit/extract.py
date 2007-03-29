#!/usr/bin/python


#num_of_tracks = ["50","500","1000","2000","5000","10000","20000"]
num_of_tracks = ["50","100","200","500","1000","2000","5000","10000","20000"]

list_of_files = ["lhc_2008_1_1001","lhc_2008_1_1003","lhc_2008_1_1004","lhc_2008_1_1007","lhc_2008_1_1008"]


#def remove_whitespaces(thestring):

#    char = list(thestring)
#    for i in range(0,len(char)-1):
#
#        if char[i]==char[i+1] and char[i]==' ':
#            char[i]=''
#
#    return ''.join(char)

# class to hold records for a row
#________________________________________________________________
class RowElement:
        pass


#________________________________________________________________
if __name__ == "__main__":


    table = []
    
    # loop over files:
    for ifile in list_of_files:
	    subtable = []

	    for isuffix in num_of_tracks:
		    filename = ifile+"_"+isuffix+".log"
		    
		    row = RowElement()
		    thefile = open(filename)
		    nlinez = 0
		    nlinec= 0
		    pass1 = 0
		    pass2 = 0
		    passdphi = 0
		    for iline in thefile:
			    if iline.find("Sum of all weights")!=-1:
				    passdphi = 1
			    if passdphi == 1 and iline.find("x0                :")!=-1:
				    tmpl = iline.split()
				    row.x0dphi = tmpl[1].strip(":")
				    row.x0errdphi = tmpl[3]
			    if passdphi == 1 and iline.find("y0                :")!=-1:
				    tmpl = iline.split()
				    row.y0dphi = tmpl[1].strip(":")
				    row.y0errdphi = tmpl[3]
			    if passdphi == 1 and iline.find("x slope           :")!=-1:
				    tmpl = iline.split()
				    row.dxdphi = tmpl[2].strip(":")
				    row.dxerrdphi = tmpl[4]
			    if passdphi == 1 and iline.find("y slope           :")!=-1:
				    tmpl = iline.split()
				    row.dydphi = tmpl[2].strip(":")
				    row.dyerrdphi = tmpl[4]
			    
			    if iline.find("z0")!=-1:
				    nlinez = nlinez +1

			    if iline.find("c0")!=-1:
				    nlinec = nlinec +1
				    
			    if nlinez==12:
				    pass1 = 1
			    if nlinez==13:
				    pass1 = 0
			    if nlinez==14:
				    pass2 = 1
			    
			    if pass1==1:
				    if iline.find("z0")!=-1:
					    tmpl = iline.split()
					    row.z0 = tmpl[2]
					    row.z0err = tmpl[3]
				    if iline.find("sigma ")!=-1:
					    tmpl = iline.split()
					    row.zsigma = tmpl[2]
					    row.zsigmaerr = tmpl[3]
				    if iline.find("x0")!=-1:
					    tmpl = iline.split()
					    row.x0 = tmpl[2]
					    row.x0err = tmpl[3]
				    if iline.find("y0")!=-1:
					    tmpl = iline.split()
					    row.y0 = tmpl[2]
					    row.y0err = tmpl[3]
				    if iline.find("dxdz")!=-1:
					    tmpl = iline.split()
					    row.dxdz = tmpl[2]
					    row.dxdzerr = tmpl[3]
				    if iline.find("dydz")!=-1:
					    tmpl = iline.split()
					    row.dydz = tmpl[2]
					    row.dydzerr = tmpl[3]
				    if iline.find("sigmaBeam")!=-1:
					    tmpl = iline.split()
					    row.sigBeam = tmpl[2]
					    row.sigBeamerr = tmpl[3]
			    if nlinec==3:
				    if iline.find("c0")!=-1:
					    tmpl = iline.split()
					    row.c0 = tmpl[2]
					    row.c0err = tmpl[3]
				    if iline.find("c1")!=-1:
					    tmpl = iline.split()
					    row.c1 = tmpl[2]
					    row.c1err = tmpl[3]
					    
		    subtable.append(row)
	    table.append(subtable)

    #get values
    i=0
    arrlen = str(len(num_of_tracks))
    for isubtable in table:
	    print "//"+50*"_"+"\n"

	    print "// begin: dphi fit results"

	    print "Double_t "+list_of_files[i]+"_x0dphi["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.x0dphi + ", "
		    else:
			    print irow.x0dphi + "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_x0errdphi["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.x0errdphi+", "
		    else:
			    print irow.x0errdphi+ "};\n"
		    n = n +1

	    print "Double_t "+list_of_files[i]+"_y0dphi["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.y0dphi + ", "
		    else:
			    print irow.y0dphi + "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_y0errdphi["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.y0errdphi+", "
		    else:
			    print irow.y0errdphi+ "};\n"
		    n = n +1

	    print "Double_t "+list_of_files[i]+"_dxdphi["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.dxdphi + ", "
		    else:
			    print irow.dxdphi + "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_dxerrdphi["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.dxerrdphi+", "
		    else:
			    print irow.dxerrdphi+ "};\n"
		    n = n +1

	    print "Double_t "+list_of_files[i]+"_dydphi["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.dydphi + ", "
		    else:
			    print irow.dydphi + "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_dyerrdphi["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.dyerrdphi+", "
		    else:
			    print irow.dyerrdphi+ "};\n"
		    n = n +1

	    print "// end  : dphi fit results"

	    
	    print "Double_t "+list_of_files[i]+"_z0["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.z0 + ", "
		    else:
			    print irow.z0 + "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_z0err["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.z0err+", "
		    else:
			    print irow.z0err+ "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_zsig["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.zsigma+", "
		    else:
			    print irow.zsigma+ "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_zsigerr["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.zsigmaerr+", "
		    else:
			    print irow.zsigmaerr+ "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_x0["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.x0+", "
		    else:
			    print irow.x0+ "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_x0err["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.x0err+", "
		    else:
			    print irow.x0err+ "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_y0["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.y0+", "
		    else:
			    print irow.y0+ "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_y0err["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.y0err+", "
		    else:
			    print irow.y0err+ "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_dxdz["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.dxdz+", "
		    else:
			    print irow.dxdz+ "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_dxdzerr["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.dxdzerr+", "
		    else:
			    print irow.dxdzerr+ "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_dydz["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.dydz+", "
		    else:
			    print irow.dydz+ "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_dydzerr["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.dydzerr+", "
		    else:
			    print irow.dydzerr+ "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_sigBeam["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.sigBeam+", "
		    else:
			    print irow.sigBeam+ "};\n"
		    n = n +1
	    
	    print "Double_t "+list_of_files[i]+"_sigBeamerr["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.sigBeamerr+", "
		    else:
			    print irow.sigBeamerr+ "};\n"
		    n = n +1

	    print "Double_t "+list_of_files[i]+"_c0["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.c0+", "
		    else:
			    print irow.c0+ "};\n"
		    n = n +1
	    print "Double_t "+list_of_files[i]+"_c0err["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.c0err+", "
		    else:
			    print irow.c0err+ "};\n"
		    n = n +1

	    print "Double_t "+list_of_files[i]+"_c1["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.c1+", "
		    else:
			    print irow.c1+ "};\n"
		    n = n +1

	    print "Double_t "+list_of_files[i]+"_c1err["+arrlen+"]={"
	    n =0
	    for irow in isubtable:
		    
		    if n!= len(isubtable)-1:
			    print irow.c1err+", "
		    else:
			    print irow.c1err+ "};\n"
		    n = n +1

	    print "\n"
	    #print "//"+50*"_"+"\n"
	    i = i +1

