void format_line(TAttLine* line,int col,int sty){
    line->SetLineWidth(5); line->SetLineColor(col);
    line->SetLineStyle(sty);}

double the_gausppar(double* vars, double* pars){
    return pars[0]*TMath::Gaus(vars[0],pars[1],pars[2])+
    pars[3]*TMath::BreitWigner(vars[0],pars[4],pars[5]);}

int macro9(){
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111); gStyle->SetStatBorderSize(0);
    gStyle->SetStatX(.89); gStyle->SetStatY(.89);

    TF1 britw("britw","[0]*TMath::BreitWigner(x,[1],[2])",80,160); 
    format_line(&britw,kBlue,2);

    TF1 gaussian("gaussian","[0]*TMath::Gaus(x,[1],[2])",80,160);
    format_line(&gaussian,kRed,2);

    TF1 gausppar("gausppar",the_gausppar,80,160,6);
    double a=15; double b=90; double c=35;
    double norm=0.06; double mean=125; double sigma=1.6;
    gausppar.SetParameters(norm,mean,sigma,a,b,c);
    gausppar.SetParNames("Norm","Mean","Sigma","a","b","c");
    format_line(&gausppar,kBlue,1);

    TH1F histo("histo","Signal plus background;X vals;Y Vals",500,80,160);
    histo.SetMarkerStyle(8);

    // Fake the data
    for (int i=1;i<=50000;++i) histo.Fill(gausppar.GetRandom());

    // Reset the parameters before the fit and set
    // by eye a peak at 6 with an area of more or less 50
    gausppar.SetParameter(0,1);
    gausppar.SetParLimits(1,120,130);
    gausppar.SetParLimits(2,0,10);
    gausppar.SetParameter(3,17);
    gausppar.SetParameter(4,91);
    gausppar.SetParLimits(5,20,55);
//  int npar=gausppar.GetNpar();
//  for (int ipar=2;ipar<npar;++ipar) gausppar.SetParameter(ipar,1);

    // perform fit ...
    auto fitResPtr = histo.Fit(&gausppar, "S");
    // ... and retrieve fit results
    fitResPtr->Print(); // print fit results
    // get covariance Matrix an print it
    TMatrixDSym covMatrix (fitResPtr->GetCovarianceMatrix());
    covMatrix.Print();

    // Set the values of the gaussian and parabola
    for (int ipar=0;ipar<3;ipar++){
        gaussian.SetParameter(ipar,gausppar.GetParameter(ipar));
        britw.SetParameter(ipar,gausppar.GetParameter(ipar+3));
    }

    histo.GetYaxis()->SetRangeUser(0,300);
    histo.DrawClone("PE");
    britw.DrawClone("Same"); gaussian.DrawClone("Same");
    TLatex latex(2,220,"#splitline{Signal Peak over}{background}");
    latex.DrawClone("Same");
    return 0;
}
