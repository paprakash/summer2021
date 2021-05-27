// defining a function with variable parameters(parameters have to be variable because we eventually be fitting our histogram with this function
double the_gaussian(double* var, double* par) {
	return par[0]*TMath::Gaus(var[0],par[1],par[2]);}
// double* x doesn't work, double_t *x is supposed to work


void test(){
	TF1 gaussian("gaussian",the_gaussian,0,20,3);
	gaussian.SetParameters(100,10,1.6);
	TH1F histo("histo","Histogram; X; Y",100,-0,20);
//TH1F is not a pointer here, so we have to use .DrawClone, .Draw doesn't work, histo's data ends with the lifetime of object.

// detail explaination of pointers in context to this example.

	for ( int i=0; i<5000;i++)
		histo.Fill(gaussian.GetRandom());


	auto c= new TCanvas("test","Test",600,400);

	histo.Fit(&gaussian);
	histo.DrawClone("PE");
  	gaussian.DrawClone("Same");
	
}	
