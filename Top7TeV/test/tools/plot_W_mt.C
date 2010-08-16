{

    gROOT->ProcessLine(".L tdrstyle.C");
    setTDRStyle();

    gROOT->ProcessLine(".L CMSTopStyle.cc+");
    CMSTopStyle();

    TCanvas *cv = new TCanvas("cv","cv",800,600);
    
    W_mt_WMC->Draw("hist");
    W_mt_QCDdata = (TH1F*) gDirectory->Get("W_mt_QCDMC");
//(TH1F*) gDirectory->Get("W_mt_QCDdata");

    W_mt_QCDdata->Draw();
    h_mt0->Add(W_mt_QCDdata);

    TGraphErrors* tgraphs;

    double err_x[30];
    double err_y[30];
    double x[30];
    double y[30];
    
    for(int j=0; j < 30; ++j){
        err_x[j]       = 0.0;
        y[j]           = W_mt_WMC->GetBinContent( j );
        x[j] = W_mt_WMC->GetBinCenter( j );
        err_y[j] = W_mt_QCDdata->GetBinContent( j );
    }
    
    tgraphs = new TGraphErrors(30, x, y, err_x, err_y);
    tgraphs->SetFillColor(kGray+3);
    tgraphs->SetFillStyle(3354);
    
    W_mt_WMC->Draw("hist");
   
    h_mt0->Draw("hist same");
    W_mt_QCDdata->Draw("hist same");
    W_mt->Draw("same");
    tgraphs->Draw("E3 same");
    
    TLegend *legend = new TLegend(0.4,0.5,0.8,0.7);
    legend->SetMargin(0.12);
    legend->SetTextSize(0.035);
    legend->SetFillColor(10);
    legend->SetBorderSize(0);
    legend->SetHeader("CMS Preliminary #sqrt{s}=7 TeV");
    legend->AddEntry(W_mt,"Data L = 0.25 pb^{-1}","lp");
    legend->AddEntry(W_mt_WMC,"W#rightarrowl#nu","f");
    legend->AddEntry(h_mt0,"Z/#gamma*#rightarrowl^{+}l^{-}","f");
    legend->AddEntry(W_mt_QCDdata,"QCD data-driven","f");
    legend->AddEntry(tgraphs,"QCD uncertainty","f");
    legend->Draw();
    
    gPad->RedrawAxis();    

}
