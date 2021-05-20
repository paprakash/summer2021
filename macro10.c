void format_line(TAttLine* line,int col,int sty){
    line->SetLineWidth(5); line->SetLineColor(col);
    line->SetLineStyle(sty);}

double the_gausppar(double* vars, double* pars){
    return pars[0]*TMath::BreitWigner(vars[0],pars[1],pars[2]) +       
    pars[3]*(ROOT::Math::crystalball_function(vars[0],pars[4],pars[5],pars[6],pars[7]));}
int macro10(){
    gStyle->SetOptTitle(0); gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111); gStyle->SetStatBorderSize(0);
    gStyle->SetStatX(.89); gStyle->SetStatY(.89);

    TF1 cb("cb","[0]*ROOT::Math::crystalball_function(x,[1],[2],[3],[4])",80,160);
    format_line(&cb,kBlue,2);

    TF1 bw("bw","[0]*TMath::BreitWigner(x,[1],[2])",80,160);
    format_line(&bw,kRed,2);

    TF1 gausppar("gausppar",the_gausppar,80,160,8);
    double a=2; double b=-122; double c=1; double d=1.6; double e=125; 
    double norm=200; double mean=91; double sigma=26;
    gausppar.SetParameters(norm,mean,sigma,a,b,c,d,e);
    gausppar.SetParNames("Norm","Mean","Sigma","a","b","c","d","e");
    format_line(&gausppar,kBlue,1);

    TH1F histo("histo","Signal plus background;X vals;Y Vals",200,80,160);
    histo.SetMarkerStyle(8);

    // Fake the data
    for (int i=1;i<=35000;++i) histo.Fill(gausppar.GetRandom());

    // Reset the parameters before the fit and set
    // by eye a peak at 6 with an area of more or less 50
    gausppar.SetParameter(0,150);
    gausppar.SetParLimits(1,85,95);
    gausppar.SetParameter(2,30);
    gausppar.SetParameter(3,1);
    gausppar.SetParLimits(4,-116,-124);
    gausppar.SetParLimits(5,1,4);
    gausppar.SetParLimits(6,0,10);
    gausppar.SetParLimits(7,120,130);

//   int npar=gausppar.GetNpar();
//   for (int ipar=2;ipar<npar;++ipar) gausppar.SetParameter(ipar,1);

    // perform fit ...
    auto fitResPtr = histo.Fit(&gausppar, "S");
    // ... and retrieve fit results
    fitResPtr->Print(); // print fit results
    // get covariance Matrix an print it
    TMatrixDSym covMatrix (fitResPtr->GetCovarianceMatrix());
    covMatrix.Print();

    // Set the values of the gaussian and parabola
    for (int ipar=0;ipar<5;ipar++){
        bw.SetParameter(ipar,gausppar.GetParameter(ipar));
        cb.SetParameter(ipar,gausppar.GetParameter(ipar+3));
    }

    histo.GetYaxis()->SetRangeUser(0,750);
    histo.DrawClone("PE");
    cb.DrawClone("Same"); bw.DrawClone("Same");
    TLatex latex(2,220,"#splitline{Signal Peak over}{background}");
    latex.DrawClone("Same");
    return 0;
}
