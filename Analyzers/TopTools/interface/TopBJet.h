#ifndef TopBJet_h
#define TopBJet_h

/**_________________________________________________________________
   class:   TopBJet.h
   package: Analyzers/TopTools


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: TopBJet.h,v 1.2 2007/05/30 15:06:53 yumiceva Exp $

________________________________________________________________**/


class TopBJet {

  public:
	TopBJet() {};
	~TopBJet() {};

	void SetDiscriminator(double discriminator) { fdiscriminator = discriminator; };
	void SetId(int id) { fjetid = id; };

	double GetDiscriminator() { return fdiscriminator; };
	int    GetId() { return fjetid; };
	
  private:
	double fdiscriminator;
	int    fjetid;
	
};

#endif


