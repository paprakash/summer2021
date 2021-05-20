void format_line(TAttLine* line,int col,int sty){
    line->SetLineWidth(5); line->SetLineColor(col);
    line->SetLineStyle(sty);}

double the_gausppar(double* vars, double* pars){
    return pars[0]*TMath::Gaus(vars[0],pars[1],pars[2])+
        pars[5]*TMath::Exp(pars[3]*(vars[0]-pars[4]));}

int macro8(){
    gStyle->SetOptTitle(0); gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111); gStyle->SetStatBorderSize(0);
    gStyle->SetStatX(.89); gStyle->SetStatY(.89);

    TF1 expo("expo","[2]*TMath::Exp([0]*(x-[1]))",100,160);
    format_line(&expo,kBlue,2);

    TF1 gaussian("gaussian","[0]*TMath::Gaus(x,[1],[2])",100,160);
    format_line(&gaussian,kRed,2);

    TF1 gausppar("gausppar",the_gausppar,100,160,6);
    double a=-0.06; double b=41; double c=800;
    double norm=3; double mean=125; double sigma=1.6;
    gausppar.SetParameters(norm,mean,sigma,a,b,c);
    gausppar.SetParNames("Norm","Mean","Sigma","a","b","c");
    format_line(&gausppar,kBlue,1);

    TH1F histo("histo","Signal plus background;X vals;Y Vals",50,100,160);
    histo.SetMarkerStyle(8);

    // Fake the dat
//    gRandom->SetSeed(0);
    for (int i=1;i<=5000;++i) histo.Fill(gausppar.GetRandom());

    // Reset the parameters before the fit and set
    // by eye a peak at 6 with an area of more or less 50
    gausppar.SetParameter(0,4);
    gausppar.SetParLimits(1,120,130);
    gausppar.SetParLimits(2,0,5);
    gausppar.SetParameter(3,-0.1);
    gausppar.FixParameter(4,41);
    gausppar.SetParameter(5,800);

//    int npar=gausppar.GetNpar();
//    for (int ipar=2;ipar<npar;++ipar) gausppar.SetParameter(ipar,1);

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
        expo.SetParameter(ipar,gausppar.GetParameter(ipar+3));
    }

    histo.GetYaxis()->SetRangeUser(0,250);
    histo.DrawClone("PE");
    expo.DrawClone("Same"); gaussian.DrawClone("Same");
    TLatex latex(2,220,"#splitline{Signal Peak over}{background}");
    latex.DrawClone("Same");
    return 0;
}
