
#include "global.h"

#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TFile.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TStyle.h"
#include "TLine.h"
#include "TPolyLine.h"

Double_t zdis(Double_t *x, Double_t *parms) { 

	Double_t z = x[0];
	Double_t sigma = 0.5;
	
	Double_t sig = sqrt(sigma*sigma+parms[1]*parms[1]);
	Double_t result = (exp(-((z-parms[0])*(z-parms[0]))/(2.0*sig*sig)))/(sig*sqrt2pi);
	
	return result;
} 


void plot_results(TString in_filename) {

	TFile* input_ro_file = TFile::Open(in_filename);
		

	TCanvas *cv1 = new TCanvas("cv1","cv1",700,700);

	TH1F *h_z = (TH1F*) gDirectory->Get("z0");
	h_z->Sumw2();
	h_z->SetMarkerStyle(20);
	h_z->SetMarkerSize(0.9);
	h_z->Scale(1./(h_z->Integral("width")));
	h_z->SetAxisRange(-40,40,"X");
	h_z->SetXTitle("dz [cm]");
	h_z->SetYTitle("Events/2 [cm]");

	TF1 *f_z = new TF1("f_z",zdis,-40.,40.,2);
	
	f_z->SetParameter(0,3.59139e-02);
	f_z->SetParameter(1,7.26);

	h_z->Draw("HISTPE1");
	f_z->SetLineColor(kBlue);
	f_z->Draw("same");

	
	Int_t npts = 7;
	Double_t x_trk[7] = {50, 500, 1000, 2000, 5000, 10000, 20000};//*e3
	Double_t x_trk_plus[9] = {50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000};//*e3
	//__________________________________________________
	Double_t lhc_2008_1_1001_x0dphi[9]={
0.0301776, 
0.0300989, 
0.0302077, 
0.029628, 
0.0299589, 
0.0298758, 
0.0300209, 
0.0299976, 
0.0300353};

Double_t lhc_2008_1_1001_x0errdphi[9]={
0.000787758, 
0.000475081, 
0.000354413, 
0.000231079, 
0.00016383, 
0.000116783, 
7.53663e-05, 
5.3527e-05, 
3.78893e-05};

Double_t lhc_2008_1_1001_y0dphi[9]={
0.060934, 
0.0601743, 
0.0600345, 
0.0598941, 
0.0601825, 
0.0603034, 
0.0601233, 
0.0601352, 
0.0600568};

Double_t lhc_2008_1_1001_y0errdphi[9]={
0.000824019, 
0.000603849, 
0.00042162, 
0.00025092, 
0.000173625, 
0.000122056, 
7.61124e-05, 
5.3519e-05, 
3.77872e-05};


Double_t lhc_2008_1_1001_dxdphi[9]={
-2.27821e-05, 
1.16662e-05, 
-1.97504e-05, 
-1.05698e-05, 
-3.93395e-06, 
5.65599e-06, 
-7.46506e-06, 
-1.65741e-05, 
-2.00388e-06};

Double_t lhc_2008_1_1001_dxerrdphi[9]={
6.46619e-05, 
5.14295e-05, 
4.42875e-05, 
3.0804e-05, 
2.23229e-05, 
1.56622e-05, 
1.03448e-05, 
7.0777e-06, 
4.98017e-06};

Double_t lhc_2008_1_1001_dydphi[9]={
9.76289e-05, 
2.44053e-05, 
5.34769e-05, 
3.7225e-06, 
-2.85195e-05, 
-7.75199e-06, 
6.86786e-06, 
3.35682e-06, 
-5.39443e-06};

Double_t lhc_2008_1_1001_dyerrdphi[9]={
9.37346e-05, 
8.19275e-05, 
5.80704e-05, 
3.74796e-05, 
2.57561e-05, 
1.73725e-05, 
1.03913e-05, 
7.2514e-06, 
5.08667e-06};

// end  : dphi fit results

	
	Double_t lhc_2008_1_1001_z0[7]={
	  4.46198e-01,
	  1.43948e-01,
	  -1.46190e-02,
	  -1.35883e-01,
	  -8.20779e-02,
	  -6.15279e-02,
	  -1.95931e-03};

	Double_t lhc_2008_1_1001_z0err[7]={
	  3.17359e+00,
	  6.65825e-01,
	  4.57645e-01,
	  3.25905e-01,
	  2.05976e-01,
	  1.49300e-01,
	  1.06201e-01};

	Double_t lhc_2008_1_1001_zsig[7]={
	  1.12093e+01,
	  7.42884e+00,
	  7.21035e+00,
	  7.27000e+00,
	  7.28512e+00,
	  7.44803e+00,
	  7.48782e+00};

	Double_t lhc_2008_1_1001_zsigerr[7]={
	  2.24632e+00,
	  4.71998e-01,
	  3.23725e-01,
	  2.30963e-01,
	  1.46387e-01,
	  1.05827e-01,
	  7.52113e-02};

	Double_t lhc_2008_1_1001_x0[7]={
	  3.01421e-02,
	  2.96275e-02,
	  2.99452e-02,
	  2.98700e-02,
	  3.00157e-02,
	  2.99939e-02,
	  3.00352e-02};

	Double_t lhc_2008_1_1001_x0err[7]={
	  1.36655e-03,
	  4.42389e-04,
	  3.00805e-04,
	  2.19132e-04,
	  1.40953e-04,
	  1.01790e-04,
	  6.95447e-05};

	Double_t lhc_2008_1_1001_y0[7]={
	  6.07408e-02,
	  5.99006e-02,
	  6.02011e-02,
	  6.03121e-02,
	  6.01254e-02,
	  6.01364e-02,
	  6.00596e-02};

	Double_t lhc_2008_1_1001_y0err[7]={
	  1.49862e-03,
	  4.81654e-04,
	  3.19446e-04,
	  2.28948e-04,
	  1.41592e-04,
	  9.93352e-05,
	  6.95884e-05};

	Double_t lhc_2008_1_1001_dxdz[7]={
	  -2.63600e-05,
	  -9.78394e-06,
	  -4.03666e-06,
	  5.06248e-06,
	  -7.67880e-06,
	  -1.64561e-05,
	  -2.08437e-06};

	Double_t lhc_2008_1_1001_dxdzerr[7]={
	  1.16223e-04,
	  5.92192e-05,
	  4.11418e-05,
	  2.94099e-05,
	  1.92613e-05,
	  1.31642e-05,
	  9.14071e-06};

	Double_t lhc_2008_1_1001_dydz[7]={
	  9.55736e-05,
	  3.67045e-06,
	  -2.98862e-05,
	  -7.54247e-06,
	  6.51068e-06,
	  3.30856e-06,
	  -5.64313e-06};

	Double_t lhc_2008_1_1001_dydzerr[7]={
	  1.68228e-04,
	  7.21177e-05,
	  4.75264e-05,
	  3.25773e-05,
	  1.93073e-05,
	  1.34731e-05,
	  9.37475e-06};

	Double_t lhc_2008_1_1001_sigBeam[7]={
	  1.09143e-03,
	  1.75735e-03,
	  1.51316e-03,
	  1.63512e-03,
	  1.58102e-03,
	  1.56662e-03,
	  1.49457e-03};

	Double_t lhc_2008_1_1001_sigBeamerr[7]={
	  1.74488e-03,
	  3.96044e-04,
	  2.96700e-04,
	  2.10479e-04,
	  1.36616e-04,
	  9.15259e-05,
	  4.77858e-05};

	Double_t lhc_2008_1_1001_c0[7]={
	  1.44090e-03,
	  1.29399e-03,
	  1.13265e-03,
	  1.06390e-03,
	  9.74014e-04,
	  9.21312e-04,
	  9.96789e-04};

	Double_t lhc_2008_1_1001_c0err[7]={
	  1.89395e-03,
	  6.19626e-04,
	  3.92915e-04,
	  2.96908e-04,
	  1.86014e-04,
	  1.27672e-04,
	  8.77168e-05};

	Double_t lhc_2008_1_1001_c1[7]={
	  6.96690e-03,
	  7.52376e-03,
	  8.32113e-03,
	  8.73127e-03,
	  9.17350e-03,
	  9.39216e-03,
	  9.01543e-03};

	Double_t lhc_2008_1_1001_c1err[7]={
	  7.10338e-03,
	  2.25684e-03,
	  1.48412e-03,
	  1.11391e-03,
	  7.03880e-04,
	  4.83239e-04,
	  3.33057e-04};



	//__________________________________________________

	Double_t lhc_2008_1_1003_z0[7]={
	  -3.81850e-02,
	  8.83430e-01,
	  8.94222e-02,
	  -1.02633e-01,
	  -3.56374e-02,
	  -3.32773e-02,
	  -2.21584e-02};

	Double_t lhc_2008_1_1003_z0err[7]={
	  2.41218e+00,
	  6.91502e-01,
	  5.07471e-01,
	  3.43619e-01,
	  2.19480e-01,
	  1.53280e-01,
	  1.07664e-01};

	Double_t lhc_2008_1_1003_zsig[7]={
	  8.50142e+00,
	  7.72243e+00,
	  7.94968e+00,
	  7.70648e+00,
	  7.69169e+00,
	  7.62947e+00,
	  7.60138e+00};

	Double_t lhc_2008_1_1003_zsigerr[7]={
	  1.70625e+00,
	  4.90414e-01,
	  3.56946e-01,
	  2.44689e-01,
	  1.54490e-01,
	  1.08477e-01,
	  7.61536e-02};

	Double_t lhc_2008_1_1003_x0[7]={
	  2.99711e-02,
	  3.00812e-02,
	  3.00532e-02,
	  3.01916e-02,
	  3.01399e-02,
	  3.00397e-02,
	  3.00608e-02};

	Double_t lhc_2008_1_1003_x0err[7]={
	  1.24830e-03,
	  3.34656e-04,
	  2.20169e-04,
	  1.56885e-04,
	  1.02191e-04,
	  7.07516e-05,
	  5.00015e-05};

	Double_t lhc_2008_1_1003_y0[7]={
	  6.01859e-02,
	  6.01956e-02,
	  6.00781e-02,
	  6.00686e-02,
	  6.00368e-02,
	  6.00110e-02,
	  5.99570e-02};

	Double_t lhc_2008_1_1003_y0err[7]={
	  1.10840e-03,
	  3.47791e-04,
	  2.26224e-04,
	  1.57836e-04,
	  1.01323e-04,
	  7.12554e-05,
	  5.07699e-05};

	Double_t lhc_2008_1_1003_dxdz[7]={
	  -2.77095e-05,
	  4.14031e-06,
	  -1.25760e-05,
	  -4.96450e-06,
	  2.01941e-06,
	  -3.57700e-06,
	  3.97319e-06};

	Double_t lhc_2008_1_1003_dxdzerr[7]={
	  1.62165e-04,
	  4.14604e-05,
	  2.76353e-05,
	  2.01611e-05,
	  1.33494e-05,
	  9.22468e-06,
	  6.55716e-06};

	Double_t lhc_2008_1_1003_dydz[7]={
	  -1.24066e-04,
	  -4.55320e-06,
	  -7.79913e-06,
	  1.45041e-05,
	  8.49816e-06,
	  1.05034e-07,
	  -1.82344e-06};

	Double_t lhc_2008_1_1003_dydzerr[7]={
	  1.65932e-04,
	  4.74154e-05,
	  2.82002e-05,
	  2.05059e-05,
	  1.32050e-05,
	  9.38152e-06,
	  6.79274e-06};

	Double_t lhc_2008_1_1003_sigBeam[7]={
	  1.42811e-03,
	  1.69244e-03,
	  1.40994e-03,
	  1.41239e-03,
	  1.50195e-03,
	  1.45192e-03,
	  1.43356e-03};

	Double_t lhc_2008_1_1003_sigBeamerr[7]={
	  8.73158e-04,
	  2.53021e-04,
	  1.81848e-04,
	  1.28337e-04,
	  8.10654e-05,
	  5.78309e-05,
	  3.69205e-05};

	Double_t lhc_2008_1_1003_c0[7]={
	  1.18969e-03,
	  1.14236e-03,
	  1.17696e-03,
	  1.21008e-03,
	  1.09253e-03,
	  1.03730e-03,
	  1.03612e-03};

	Double_t lhc_2008_1_1003_c0err[7]={
	  1.77001e-03,
	  5.61174e-04,
	  3.54293e-04,
	  2.48902e-04,
	  1.54610e-04,
	  1.14864e-04,
	  7.58306e-05};

	Double_t lhc_2008_1_1003_c1[7]={
	  3.25687e-03,
	  3.41229e-03,
	  3.22907e-03,
	  3.08922e-03,
	  3.60591e-03,
	  3.84293e-03,
	  3.84799e-03};

	Double_t lhc_2008_1_1003_c1err[7]={
	  5.45669e-03,
	  1.82915e-03,
	  1.11669e-03,
	  7.87738e-04,
	  5.16700e-04,
	  3.69976e-04,
	  2.56918e-04};



	//__________________________________________________

	Double_t lhc_2008_1_1004_z0[7]={
	  1.04292e+00,
	  4.61511e-02,
	  -1.21651e-01,
	  1.72103e-02,
	  1.12569e-01,
	  -3.60590e-02,
	  -1.59184e-01};

	Double_t lhc_2008_1_1004_z0err[7]={
	  2.92416e+00,
	  6.96780e-01,
	  4.92354e-01,
	  3.34006e-01,
	  2.11092e-01,
	  1.51121e-01,
	  1.07289e-01};

	Double_t lhc_2008_1_1004_zsig[7]={
	  1.03277e+01,
	  7.77416e+00,
	  7.67084e+00,
	  7.45173e+00,
	  7.49435e+00,
	  7.55125e+00,
	  7.61751e+00};

	Double_t lhc_2008_1_1004_zsigerr[7]={
	  2.07016e+00,
	  4.93721e-01,
	  3.44699e-01,
	  2.35111e-01,
	  1.50482e-01,
	  1.07263e-01,
	  7.64401e-02};

	Double_t lhc_2008_1_1004_x0[7]={
	  2.39246e-02,
	  2.90375e-02,
	  3.09667e-02,
	  2.90064e-02,
	  2.81920e-02,
	  2.73793e-02,
	  2.76307e-02};

	Double_t lhc_2008_1_1004_x0err[7]={
	  1.23663e-02,
	  3.60500e-03,
	  2.60359e-03,
	  1.84857e-03,
	  1.15499e-03,
	  8.07471e-04,
	  5.73459e-04};

	Double_t lhc_2008_1_1004_y0[7]={
	  5.17957e-02,
	  5.44708e-02,
	  5.56301e-02,
	  5.66486e-02,
	  5.56768e-02,
	  5.62055e-02,
	  5.57259e-02};

	Double_t lhc_2008_1_1004_y0err[7]={
	  1.18936e-02,
	  3.69485e-03,
	  2.59594e-03,
	  1.81899e-03,
	  1.16709e-03,
	  8.30521e-04,
	  5.85581e-04};

	Double_t lhc_2008_1_1004_dxdz[7]={
	  -4.29059e-04,
	  -5.31471e-04,
	  -4.05556e-04,
	  -1.06438e-04,
	  -1.50900e-04,
	  -1.17318e-04,
	  -6.30831e-05};

	Double_t lhc_2008_1_1004_dxdzerr[7]={
	  1.11384e-03,
	  5.01250e-04,
	  3.49512e-04,
	  2.50664e-04,
	  1.54675e-04,
	  1.05532e-04,
	  7.54195e-05};

	Double_t lhc_2008_1_1004_dydz[7]={
	  -6.65346e-04,
	  1.47233e-07,
	  -2.33696e-04,
	  -1.12378e-04,
	  -4.88903e-05,
	  -9.12338e-05,
	  -5.22661e-05};

	Double_t lhc_2008_1_1004_dydzerr[7]={
	  1.38271e-03,
	  4.63962e-04,
	  3.39681e-04,
	  2.42971e-04,
	  1.52970e-04,
	  1.09265e-04,
	  7.55249e-05};

	Double_t lhc_2008_1_1004_sigBeam[7]={
	  -1.13662e-11,
	  -7.98729e-11,
	  -2.03878e-11,
	  -1.76441e-13,
	  1.07401e-12,
	  -1.95835e-12,
	  -1.96799e-13};

	Double_t lhc_2008_1_1004_sigBeamerr[7]={
	  1.41986e-02,
	  8.70636e-03,
	  5.85862e-03,
	  3.99839e-03,
	  2.45859e-03,
	  1.61573e-03,
	  1.15132e-03};

	Double_t lhc_2008_1_1004_c0[7]={
	  3.54455e-03,
	  1.13423e-02,
	  1.12373e-02,
	  1.19620e-02,
	  1.17372e-02,
	  1.19802e-02,
	  1.00000e-03};

	Double_t lhc_2008_1_1004_c0err[7]={
	  4.95296e-03,
	  3.10371e-03,
	  2.37698e-03,
	  1.72683e-03,
	  1.12129e-03,
	  8.08848e-04,
	  0};

	Double_t lhc_2008_1_1004_c1[7]={
	  1.00000e-01,
	  6.96523e-02,
	  7.22813e-02,
	  6.75410e-02,
	  6.94909e-02,
	  6.72493e-02,
	  9.00000e-03};

	Double_t lhc_2008_1_1004_c1err[7]={
	  7.63984e-02,
	  1.31604e-02,
	  1.02088e-02,
	  7.33441e-03,
	  4.76283e-03,
	  3.41798e-03,
	  0};



	//__________________________________________________

	Double_t lhc_2008_1_1007_z0[7]={
	  -9.95393e-01,
	  -7.17162e-01,
	  -6.41580e-01,
	  -3.06169e-01,
	  -1.65292e-01,
	  -1.80706e-01,
	  8.80749e-02};

	Double_t lhc_2008_1_1007_z0err[7]={
	  2.59540e+00,
	  6.82056e-01,
	  4.72756e-01,
	  3.40386e-01,
	  2.15714e-01,
	  1.57142e-01,
	  1.07771e-01};

	Double_t lhc_2008_1_1007_zsig[7]={
	  9.16185e+00,
	  7.60888e+00,
	  7.45869e+00,
	  7.59708e+00,
	  7.50377e+00,
	  7.59437e+00,
	  7.59322e+00};

	Double_t lhc_2008_1_1007_zsigerr[7]={
	  1.83783e+00,
	  4.83307e-01,
	  3.35296e-01,
	  2.41289e-01,
	  1.50786e-01,
	  1.07867e-01,
	  7.62480e-02};

	Double_t lhc_2008_1_1007_x0[7]={
	  3.46045e-02,
	  2.97274e-02,
	  2.99435e-02,
	  2.97548e-02,
	  2.93903e-02,
	  2.97234e-02,
	  2.99661e-02};

	Double_t lhc_2008_1_1007_x0err[7]={
	  5.48986e-03,
	  1.57609e-03,
	  1.08829e-03,
	  7.96735e-04,
	  5.09797e-04,
	  3.58597e-04,
	  2.52520e-04};

	Double_t lhc_2008_1_1007_y0[7]={
	  6.04002e-02,
	  6.09749e-02,
	  6.04378e-02,
	  5.97476e-02,
	  5.97658e-02,
	  5.98569e-02,
	  5.98681e-02};

	Double_t lhc_2008_1_1007_y0err[7]={
	  5.12339e-03,
	  1.66074e-03,
	  1.19572e-03,
	  8.20141e-04,
	  5.63852e-04,
	  3.60689e-04,
	  2.54039e-04};

	Double_t lhc_2008_1_1007_dxdz[7]={
	  -3.24900e-04,
	  -2.40130e-04,
	  -1.65906e-04,
	  -9.79174e-05,
	  -5.58210e-05,
	  -1.46326e-05,
	  -3.22470e-05};

	Double_t lhc_2008_1_1007_dxdzerr[7]={
	  9.08545e-04,
	  1.99893e-04,
	  1.48371e-04,
	  1.05941e-04,
	  7.16049e-05,
	  4.74365e-05,
	  3.36236e-05};

	Double_t lhc_2008_1_1007_dydz[7]={
	  1.84017e-04,
	  1.47367e-04,
	  9.38653e-05,
	  5.57650e-05,
	  3.99455e-06,
	  -1.54842e-05,
	  -9.25240e-06};

	Double_t lhc_2008_1_1007_dydzerr[7]={
	  4.78702e-04,
	  2.20369e-04,
	  1.57173e-04,
	  1.06417e-04,
	  6.66248e-05,
	  4.71040e-05,
	  3.35744e-05};

	Double_t lhc_2008_1_1007_sigBeam[7]={
	  -1.65184e-11,
	  -2.24250e-10,
	  -3.19461e-11,
	  1.79406e-03,
	  1.28260e-03,
	  1.36457e-03,
	  8.07916e-04};

	Double_t lhc_2008_1_1007_sigBeamerr[7]={
	  6.89676e-03,
	  1.51509e-02,
	  1.94623e-02,
	  2.81442e-03,
	  1.84759e-03,
	  1.56168e-03,
	  1.90421e-03};

	Double_t lhc_2008_1_1007_c0[7]={
	  1.06540e-02,
	  9.42378e-03,
	  9.99524e-03,
	  1.00690e-02,
	  1.00269e-02,
	  9.99201e-03,
	  1.00510e-02};

	Double_t lhc_2008_1_1007_c0err[7]={
	  2.36637e-03,
	  1.95477e-03,
	  1.29900e-03,
	  9.53090e-04,
	  5.91833e-04,
	  4.24378e-04,
	  2.96507e-04};

	Double_t lhc_2008_1_1007_c1[7]={
	  1.12938e-05,
	  1.21302e-02,
	  9.97098e-03,
	  9.72182e-03,
	  9.89049e-03,
	  1.00340e-02,
	  9.79132e-03};

	Double_t lhc_2008_1_1007_c1err[7]={
	  7.71087e-02,
	  6.83442e-03,
	  4.44569e-03,
	  3.32248e-03,
	  2.08632e-03,
	  1.50837e-03,
	  1.05031e-03};



	//__________________________________________________

	Double_t lhc_2008_1_1008_z0[7]={
	  1.08044e+00,
	  1.08044e+00,
	  7.35526e-01,
	  8.05133e-01,
	  2.33395e-01,
	  -2.71752e-02,
	  -1.33425e-03};

	Double_t lhc_2008_1_1008_z0err[7]={
	  6.99356e-01,
	  6.99356e-01,
	  4.80171e-01,
	  3.26040e-01,
	  2.13885e-01,
	  1.54206e-01,
	  1.07662e-01};

	Double_t lhc_2008_1_1008_zsig[7]={
	  7.80332e+00,
	  7.80332e+00,
	  7.57547e+00,
	  7.27083e+00,
	  7.49105e+00,
	  7.69230e+00,
	  7.59876e+00};

	Double_t lhc_2008_1_1008_zsigerr[7]={
	  4.95640e-01,
	  4.95640e-01,
	  3.40277e-01,
	  2.29226e-01,
	  1.50474e-01,
	  1.09246e-01,
	  7.63120e-02};

	Double_t lhc_2008_1_1008_x0[7]={
	  3.06592e-02,
	  3.06592e-02,
	  3.01239e-02,
	  2.99494e-02,
	  2.99361e-02,
	  2.99940e-02,
	  3.00608e-02};

	Double_t lhc_2008_1_1008_x0err[7]={
	  8.26298e-04,
	  8.26298e-04,
	  5.77175e-04,
	  4.09223e-04,
	  2.71498e-04,
	  1.86995e-04,
	  1.31586e-04};

	Double_t lhc_2008_1_1008_y0[7]={
	  6.00086e-02,
	  6.00086e-02,
	  5.98369e-02,
	  6.01561e-02,
	  5.99427e-02,
	  6.01033e-02,
	  6.00331e-02};

	Double_t lhc_2008_1_1008_y0err[7]={
	  8.17955e-04,
	  8.17955e-04,
	  5.85042e-04,
	  4.13271e-04,
	  3.44408e-04,
	  1.87468e-04,
	  1.29637e-04};

	Double_t lhc_2008_1_1008_dxdz[7]={
	  2.72134e-05,
	  2.72134e-05,
	  2.53379e-05,
	  4.00584e-05,
	  4.07495e-06,
	  5.35110e-06,
	  -5.57273e-06};

	Double_t lhc_2008_1_1008_dxdzerr[7]={
	  1.09185e-04,
	  1.09185e-04,
	  7.84185e-05,
	  5.72856e-05,
	  4.23771e-05,
	  2.53940e-05,
	  1.79466e-05};

	Double_t lhc_2008_1_1008_dydz[7]={
	  1.78324e-05,
	  1.78324e-05,
	  3.47907e-05,
	  2.34930e-05,
	  1.24817e-05,
	  -2.40878e-06,
	  -2.14876e-06};

	Double_t lhc_2008_1_1008_dydzerr[7]={
	  1.01599e-04,
	  1.01599e-04,
	  7.46749e-05,
	  5.55876e-05,
	  3.65372e-05,
	  2.39532e-05,
	  1.72110e-05};

	Double_t lhc_2008_1_1008_sigBeam[7]={
	  1.08162e-03,
	  1.08162e-03,
	  1.14457e-03,
	  1.24108e-03,
	  1.98567e-03,
	  1.66901e-03,
	  1.53577e-03};

	Double_t lhc_2008_1_1008_sigBeamerr[7]={
	  2.31137e-03,
	  2.31137e-03,
	  1.55370e-03,
	  1.06429e-03,
	  2.71229e-04,
	  3.20572e-04,
	  1.00687e-04};

	Double_t lhc_2008_1_1008_c0[7]={
	  4.83119e-03,
	  4.83119e-03,
	  5.04564e-03,
	  4.83072e-03,
	  4.81412e-03,
	  4.90749e-03,
	  4.91447e-03};

	Double_t lhc_2008_1_1008_c0err[7]={
	  8.56206e-04,
	  8.56206e-04,
	  6.63851e-04,
	  4.72601e-04,
	  3.26041e-04,
	  2.25482e-04,
	  1.56104e-04};

	Double_t lhc_2008_1_1008_c1[7]={
	  5.67291e-03,
	  5.67291e-03,
	  4.81618e-03,
	  5.68028e-03,
	  5.73320e-03,
	  5.36838e-03,
	  5.34094e-03};

	Double_t lhc_2008_1_1008_c1err[7]={
	  2.94739e-03,
	  2.94739e-03,
	  2.28777e-03,
	  1.65217e-03,
	  1.12530e-03,
	  7.80171e-04,
	  5.40683e-04};



// change cm to micron
	
	for (int i=0; i< 9; i++) {
		lhc_2008_1_1001_x0dphi[i]=lhc_2008_1_1001_x0dphi[i]*1.e4;
		lhc_2008_1_1001_x0errdphi[i]=lhc_2008_1_1001_x0errdphi[i]*1.e4;
		lhc_2008_1_1001_y0dphi[i]=lhc_2008_1_1001_y0dphi[i]*1.e4;
		lhc_2008_1_1001_y0errdphi[i]=lhc_2008_1_1001_y0errdphi[i]*1.e4;
		lhc_2008_1_1001_dxdphi[i]=lhc_2008_1_1001_dxdphi[i]*1.e4;
		lhc_2008_1_1001_dxerrdphi[i]=lhc_2008_1_1001_dxerrdphi[i]*1.e4;
		lhc_2008_1_1001_dydphi[i]=lhc_2008_1_1001_dydphi[i]*1.e4;
		lhc_2008_1_1001_dyerrdphi[i]=lhc_2008_1_1001_dyerrdphi[i]*1.e4;
	}
		
 for (int i=0; i< 7; i++) {
	 lhc_2008_1_1001_x0[i]=lhc_2008_1_1001_x0[i]*1.e4;
	 lhc_2008_1_1001_x0err[i]=lhc_2008_1_1001_x0err[i]*1.e4;
	 lhc_2008_1_1001_y0[i]=lhc_2008_1_1001_y0[i]*1.e4;
	 lhc_2008_1_1001_y0err[i]=lhc_2008_1_1001_y0err[i]*1.e4;
	 lhc_2008_1_1001_sigBeam[i]=lhc_2008_1_1001_sigBeam[i]*1.e4;
	 lhc_2008_1_1001_sigBeamerr[i]=lhc_2008_1_1001_sigBeamerr[i]*1.e4;
	 lhc_2008_1_1001_c0[i]=lhc_2008_1_1001_c0[i]*1.e4;
	 lhc_2008_1_1001_c0err[i]=lhc_2008_1_1001_c0err[i]*1.e4;
	 lhc_2008_1_1001_c1[i]=lhc_2008_1_1001_c1[i]*1.e4;
	 lhc_2008_1_1001_c1err[i]=lhc_2008_1_1001_c1err[i]*1.e4;
	 lhc_2008_1_1001_dxdz[i]=lhc_2008_1_1001_dxdz[i]*1.e4;
	 lhc_2008_1_1001_dxdzerr[i]=lhc_2008_1_1001_dxdzerr[i]*1.e4;
	 lhc_2008_1_1001_dydz[i]=lhc_2008_1_1001_dydz[i]*1.e4;
	 lhc_2008_1_1001_dydzerr[i]=lhc_2008_1_1001_dydzerr[i]*1.e4;
	 
         lhc_2008_1_1003_x0[i]=lhc_2008_1_1003_x0[i]*1.e4;
         lhc_2008_1_1003_x0err[i]=lhc_2008_1_1003_x0err[i]*1.e4;
         lhc_2008_1_1003_y0[i]=lhc_2008_1_1003_y0[i]*1.e4;
         lhc_2008_1_1003_y0err[i]=lhc_2008_1_1003_y0err[i]*1.e4;
         lhc_2008_1_1003_sigBeam[i]=lhc_2008_1_1003_sigBeam[i]*1.e4;
         lhc_2008_1_1003_sigBeamerr[i]=lhc_2008_1_1003_sigBeamerr[i]*1.e4;

         lhc_2008_1_1008_x0[i]=lhc_2008_1_1008_x0[i]*1.e4;
         lhc_2008_1_1008_x0err[i]=lhc_2008_1_1008_x0err[i]*1.e4;
         lhc_2008_1_1008_y0[i]=lhc_2008_1_1008_y0[i]*1.e4;
         lhc_2008_1_1008_y0err[i]=lhc_2008_1_1008_y0err[i]*1.e4;
         lhc_2008_1_1008_sigBeam[i]=lhc_2008_1_1008_sigBeam[i]*1.e4;
         lhc_2008_1_1008_sigBeamerr[i]=lhc_2008_1_1008_sigBeamerr[i]*1.e4;

         lhc_2008_1_1007_x0[i]=lhc_2008_1_1007_x0[i]*1.e4;
         lhc_2008_1_1007_x0err[i]=lhc_2008_1_1007_x0err[i]*1.e4;
         lhc_2008_1_1007_y0[i]=lhc_2008_1_1007_y0[i]*1.e4;
         lhc_2008_1_1007_y0err[i]=lhc_2008_1_1007_y0err[i]*1.e4;
         lhc_2008_1_1007_sigBeam[i]=lhc_2008_1_1007_sigBeam[i]*1.e4;
         lhc_2008_1_1007_sigBeamerr[i]=lhc_2008_1_1007_sigBeamerr[i]*1.e4;



 }

 TMultiGraph *multi_z0 = new TMultiGraph(); 
 TGraphErrors* graerr[3];
 Double_t zeros[7] = {0};
 Double_t zeros_plus[9] = {0};
 
 graerr[0] = new TGraphErrors(7,x_trk,lhc_2008_1_1001_z0,zeros,lhc_2008_1_1001_z0err);
 //graerr[1] = new TGraphErrors(7,x_trk,lhc_2008_1_1002_z0,zeros,lhc_2008_1_1002_z0err);
 //graerr[2] = new TGraphErrors(7,x_trk,lhc_2008_1_1003_z0,zeros,lhc_2008_1_1003_z0err);

 graerr[0]->SetMarkerStyle(20);
 
 // for (int i=0; i<3; i++) {
	 
 //	 graerr[i]->SetMarkerSize(0.9);
 //	 multi_z0->Add(graerr[i]);
 //}

 

 
 TCanvas *cv2 = new TCanvas("cv2","cv2",700,700);

 cv2->SetLogx();
 graerr[0]->Draw("ap");
 graerr[0]->GetXaxis()->SetTitle("Tracks");
 graerr[0]->GetYaxis()->SetTitle("z0 [cm]");

 //_______
TCanvas *cv31 = new TCanvas("cv31","cv31",700,700);

 TGraphErrors *graerr_x0dphi = new TGraphErrors(9,x_trk_plus,lhc_2008_1_1001_x0dphi,zeros_plus,lhc_2008_1_1001_x0errdphi);
	
 cv31->SetLogx();
 graerr_x0dphi->SetMarkerSize(0.9);
 graerr_x0dphi->SetMarkerStyle(20);
 graerr_x0dphi->Draw("ap");
 graerr_x0dphi->GetXaxis()->SetTitle("Tracks");
 graerr_x0dphi->GetYaxis()->SetTitle("x0 [#mum]");

 TLine *line_x0dphi = new TLine(0.,0.03*1.e4,20000,0.03*1.e4);
 line_x0dphi->SetLineStyle(2);
 line_x0dphi->SetLineWidth(2);
 line_x0dphi->Draw();
//___________
 
  //_______
TCanvas *cv32 = new TCanvas("cv32","cv32",700,700);

 TGraphErrors *graerr_y0dphi = new TGraphErrors(9,x_trk_plus,lhc_2008_1_1001_y0dphi,zeros_plus,lhc_2008_1_1001_y0errdphi);
	
 cv32->SetLogx();
 graerr_y0dphi->SetMarkerSize(0.9);
 graerr_y0dphi->SetMarkerStyle(20);
 graerr_y0dphi->Draw("ap");
 graerr_y0dphi->GetXaxis()->SetTitle("Tracks");
 graerr_y0dphi->GetYaxis()->SetTitle("y0 [#mum]");

 TLine *line_y0dphi = new TLine(0.,0.06*1.e4,20000,0.06*1.e4);
 line_y0dphi->SetLineStyle(2);
 line_y0dphi->SetLineWidth(2);
 line_y0dphi->Draw();
//___________
 
 //_______
TCanvas *cv33 = new TCanvas("cv33","cv33",700,700);

 TGraphErrors *graerr_dxdphi = new TGraphErrors(9,x_trk_plus,lhc_2008_1_1001_dxdphi,zeros_plus,lhc_2008_1_1001_dxerrdphi);
	
 cv33->SetLogx();
 graerr_dxdphi->SetMarkerSize(0.9);
 graerr_dxdphi->SetMarkerStyle(20);
 graerr_dxdphi->Draw("ap");
 graerr_dxdphi->GetXaxis()->SetTitle("Tracks");
 graerr_dxdphi->GetYaxis()->SetTitle("#frac{dx}{dz} [#mum]");

 TLine *line_dxdphi = new TLine(0.,0.,20000,0.);
 line_dxdphi->SetLineStyle(2);
 line_dxdphi->SetLineWidth(2);
 line_dxdphi->Draw();
//___________
 
  //_______
TCanvas *cv34 = new TCanvas("cv34","cv34",700,700);

 TGraphErrors *graerr_dydphi = new TGraphErrors(9,x_trk_plus,lhc_2008_1_1001_dydphi,zeros_plus,lhc_2008_1_1001_dyerrdphi);
	
 cv34->SetLogx();
 graerr_dydphi->SetMarkerSize(0.9);
 graerr_dydphi->SetMarkerStyle(20);
 graerr_dydphi->Draw("ap");
 graerr_dydphi->GetXaxis()->SetTitle("Tracks");
 graerr_dydphi->GetYaxis()->SetTitle("#frac{dy}{dz} [#mum]");

 TLine *line_dydphi = new TLine(0.,0.,20000,0.);
 line_dydphi->SetLineStyle(2);
 line_dydphi->SetLineWidth(2);
 line_dydphi->Draw();
//___________
 

 
 TCanvas *cv3 = new TCanvas("cv3","cv3",700,700);

 TGraphErrors *graerr_zsig = new TGraphErrors(7,x_trk,lhc_2008_1_1001_zsig,zeros,lhc_2008_1_1001_zsigerr);
	
 cv3->SetLogx();
 graerr_zsig->SetMarkerSize(0.9);
 graerr_zsig->SetMarkerStyle(20);
 graerr_zsig->Draw("ap");
 graerr_zsig->GetXaxis()->SetTitle("Tracks");
 graerr_zsig->GetYaxis()->SetTitle("#sigma_{z0} [cm]");

 TLine *line_zsig = new TLine(0.,7.55,20000,7.55);
 line_zsig->SetLineStyle(2);
 line_zsig->SetLineWidth(2);
 line_zsig->Draw();

 TCanvas *cv4 = new TCanvas("cv4","cv4",700,700);

 TGraphErrors *graerr_x0 = new TGraphErrors(7,x_trk,lhc_2008_1_1001_x0,zeros,lhc_2008_1_1001_x0err);
	
 cv4->SetLogx();
 graerr_x0->SetMarkerSize(0.9);
 graerr_x0->SetMarkerStyle(20);
 graerr_x0->Draw("ap");
 graerr_x0->GetXaxis()->SetTitle("Tracks");
 graerr_x0->GetYaxis()->SetTitle("x0 [#mum]");

 TLine *line_x0 = new TLine(0.,0.03*1.e4,20000,0.03*1.e4);
 line_x0->SetLineStyle(2);
 line_x0->SetLineWidth(2);
 line_x0->Draw();

 TCanvas *cv5 = new TCanvas("cv5","cv5",700,700);

 TGraphErrors *graerr_y0 = new TGraphErrors(7,x_trk,lhc_2008_1_1001_y0,zeros,lhc_2008_1_1001_y0err);
	
 cv5->SetLogx();
 graerr_y0->SetMarkerSize(0.9);
 graerr_y0->SetMarkerStyle(20);
 graerr_y0->Draw("ap");
 graerr_y0->GetXaxis()->SetTitle("Tracks");
 graerr_y0->GetYaxis()->SetTitle("y0 [#mum]");

 TLine *line_y0 = new TLine(0.,0.06*1.e4,20000,0.06*1.e4);
 line_y0->SetLineStyle(2);
 line_y0->SetLineWidth(2);
 line_y0->Draw();

 TCanvas *cv6 = new TCanvas("cv6","cv6",700,700);

 
 TGraphErrors *graerr_sigBeam = new TGraphErrors(7,x_trk,lhc_2008_1_1001_sigBeam,zeros,lhc_2008_1_1001_sigBeamerr);
	
 cv6->SetLogx();
 graerr_sigBeam->SetMarkerSize(0.9);
 graerr_sigBeam->SetMarkerStyle(20);
 graerr_sigBeam->Draw("ap");
 graerr_sigBeam->GetXaxis()->SetTitle("Tracks");
 graerr_sigBeam->GetYaxis()->SetTitle("beam width [#mum]");

 double beam0=sqrt(55.*3.75e-8)*1.e4;
 TLine *line_sigBeam = new TLine(0.,beam0,20000,beam0);
 line_sigBeam->SetLineStyle(2);
 line_sigBeam->SetLineWidth(2);
 line_sigBeam->Draw();

 TCanvas *cv7 = new TCanvas("cv7","cv7",700,700);

 Double_t polyarrx[14] = {50, 500, 1000, 2000, 5000, 10000, 20000,
						  20000, 10000, 5000, 2000, 1000, 500, 50 };

 Double_t polyarr7[14];				  
 for (int i=0; i<7; i++) {
	 polyarr7[i] = lhc_2008_1_1001_c0[i]+lhc_2008_1_1001_c0err[i];
	 polyarr7[i+7] = lhc_2008_1_1001_c0[6-i]-lhc_2008_1_1001_c0err[6-i];
 }
 TPolyLine *poly7t = new TPolyLine(14,polyarrx,polyarr7);
 poly7t->SetFillColor(kYellow);

 TGraphErrors *graerr_c0 = new TGraphErrors(7,x_trk,lhc_2008_1_1001_c0,zeros,lhc_2008_1_1001_c0err);

 cv7->SetLogx();
 graerr_c0->SetFillStyle(4000);
 graerr_c0->SetFillColor(0);
 
 graerr_c0->SetMarkerSize(0.9);
 graerr_c0->SetMarkerStyle(20);
 graerr_c0->Draw("ap");
 graerr_c0->GetXaxis()->SetTitle("Tracks");
 graerr_c0->GetYaxis()->SetTitle("c_{0} [#mum]");

 poly7t->Draw("f");
 graerr_c0->Draw("p same");

 TLine *line_c0 = new TLine(0.,0.001*1.e4,20000,0.001*1.e4);
 line_c0->SetLineStyle(2);
 line_c0->SetLineWidth(2);
 line_c0->Draw();

 TCanvas *cv8 = new TCanvas("cv8","cv8",700,700);

 TGraphErrors *graerr_c1 = new TGraphErrors(7,x_trk,lhc_2008_1_1001_c1,zeros,lhc_2008_1_1001_c1err);

 
 Double_t polyarr8[14];				  
 for (int i=0; i<7; i++) {
	 polyarr8[i] = lhc_2008_1_1001_c1[i]+lhc_2008_1_1001_c1err[i];
	 polyarr8[i+7] = lhc_2008_1_1001_c1[6-i]-lhc_2008_1_1001_c1err[6-i];
 }
 
 TPolyLine *poly8t = new TPolyLine(14,polyarrx,polyarr8);
 poly8t->SetFillColor(kYellow);
 //poly8t->Draw("f");
 
 
 cv8->SetLogx();
 graerr_c1->SetFillStyle(4000);
 graerr_c1->SetFillColor(0);
 
 graerr_c1->SetMarkerSize(0.9);
 graerr_c1->SetMarkerStyle(20);
 graerr_c1->Draw("ap");
 graerr_c1->GetXaxis()->SetTitle("Tracks");
 graerr_c1->GetYaxis()->SetTitle("c_{1} [#mumGeV/c]");

 poly8t->Draw("f");
 graerr_c1->Draw("p same");
 
 TLine *line_c1 = new TLine(0.,0.009*1.e4,20000,0.009*1.e4);
 line_c1->SetLineStyle(2);
 line_c1->SetLineWidth(2);
 line_c1->Draw();

 
 TCanvas *cv9 = new TCanvas("cv9","cv9",700,700);
 Double_t x_res[3] = {1,2,5};
 Double_t x_res_err[3] = {0};
 Double_t sig_res[3] = {lhc_2008_1_1001_sigBeam[6],
			lhc_2008_1_1008_sigBeam[6],
			lhc_2008_1_1007_sigBeam[6]};

 Double_t sig_res_err[3] = {lhc_2008_1_1001_sigBeamerr[6],
			    lhc_2008_1_1008_sigBeamerr[6],
			    lhc_2008_1_1007_sigBeamerr[6]};


 TGraphErrors *graerr_resSig = new TGraphErrors(3,x_res,sig_res,x_res_err,sig_res_err);

 graerr_resSig->SetMarkerSize(0.9);
 graerr_resSig->SetMarkerStyle(20);
 graerr_resSig->Draw("ap");
 graerr_resSig->GetXaxis()->SetTitle("samples");
 graerr_resSig->GetYaxis()->SetTitle("beam width [#mum]");

 TLine *line_sigBeam2 = new TLine(0.6,beam0,5.4,beam0);
 line_sigBeam2->SetLineStyle(2);
 line_sigBeam2->SetLineWidth(2);
 line_sigBeam2->Draw();


 TCanvas *cv10 = new TCanvas("cv10","cv10",700,700);

 TGraphErrors *graerr_dxdz = new TGraphErrors(7,x_trk,lhc_2008_1_1001_dxdz,zeros,lhc_2008_1_1001_dxdzerr);
	
 cv10->SetLogx();
 graerr_dxdz->SetMarkerSize(0.9);
 graerr_dxdz->SetMarkerStyle(20);
 graerr_dxdz->Draw("ap");
 graerr_dxdz->GetXaxis()->SetTitle("Tracks");
 graerr_dxdz->GetYaxis()->SetTitle("#frac{dx}{dz} [#muradian]");

 TLine *line_dxdz = new TLine(0.,0.0,20000,0.0);
 line_dxdz->SetLineStyle(2);
 line_dxdz->SetLineWidth(2);
 line_dxdz->Draw();

 TCanvas *cv11 = new TCanvas("cv11","cv11",700,700);

 TGraphErrors *graerr_dydz = new TGraphErrors(7,x_trk,lhc_2008_1_1001_dydz,zeros,lhc_2008_1_1001_dydzerr);
	
 cv11->SetLogx();
 graerr_dydz->SetMarkerSize(0.9);
 graerr_dydz->SetMarkerStyle(20);
 graerr_dydz->Draw("ap");
 graerr_dydz->GetXaxis()->SetTitle("Tracks");
 graerr_dydz->GetYaxis()->SetTitle("#frac{dy}{dz} [#muradian]");

 TLine *line_dydz = new TLine(0.,0.0,20000,0.0);
 line_dydz->SetLineStyle(2);
 line_dydz->SetLineWidth(2);
 line_dydz->Draw();

 

							
}
