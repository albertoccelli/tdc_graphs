
//--------------------------------------------------------------------------------------------------
//                               University of Torino - Department of Physics
//                                   via Giuria 1 10125, Torino, Italy
//--------------------------------------------------------------------------------------------------
// [Filename]       histoFromFile.cxx
// [Project]        Advanced Electronics Laboratory course
// [Author]         Luca Pacher - pacher@to.infn.it
// [Language]       ROOT/C++
// [Created]        Jun 01, 2016
// [Modified]       Jun 01, 2016
// [Description]    Simple un-named ROOT script to histogram and 
//					fit a set of data stored on an external
//                  plain text files
// [Notes]          Execute the script at the ROOT command prompt with
//
//                     root[] .x histoFromFile.cxx
//
// [Version]        1.0
// [Revisions]      01.06.2016 - Created
//-----------------------------------------------------------------------------------------------------




void td130450()
{
   	gROOT->Reset() ;
   	gStyle->SetOptStat("emr") ;   // show into the statistics box the number of Entries, the Mean and the RMS  
    
    int N = 8;
    
    string filename[41] = {
   "data200219/datatd_13_0000.csv", "data200219/datatd_13_0050.csv", 
   "data200219/datatd_13_0100.csv", "data200219/datatd_13_0150.csv", 
   "data200219/datatd_13_0200.csv", "data200219/datatd_13_0250.csv", 
   "data200219/datatd_13_0300.csv", "data200219/datatd_13_0350.csv", 
   "data200219/datatd_13_0400.csv", "data200219/datatd_13_0450.csv", 
   "data200219/datatd_13_0500.csv", "data200219/datatd_13_0550.csv", 
   "data200219/datatd_13_0600.csv", "data200219/datatd_13_0650.csv", 
   "data200219/datatd_13_0700.csv", "data200219/datatd_13_0750.csv", 
   "data200219/datatd_13_0800.csv", "data200219/datatd_13_0850.csv", 
   "data200219/datatd_13_0900.csv", "data200219/datatd_13_0950.csv", 
   "data200219/datatd_13_1000.csv", "data200219/datatd_13_1050.csv", 
   "data200219/datatd_13_1100.csv", "data200219/datatd_13_1150.csv", 
   "data200219/datatd_13_1200.csv", "data200219/datatd_13_1250.csv", 
   "data200219/datatd_13_1300.csv", "data200219/datatd_13_1350.csv", 
   "data200219/datatd_13_1400.csv", "data200219/datatd_13_1450.csv", 
   "data200219/datatd_13_1500.csv", "data200219/datatd_13_1550.csv", 
   "data200219/datatd_13_1600.csv", "data200219/datatd_13_1650.csv", 
   "data200219/datatd_13_1700.csv", "data200219/datatd_13_1750.csv", 
   "data200219/datatd_13_1800.csv", "data200219/datatd_13_1850.csv", 
   "data200219/datatd_13_1900.csv", "data200219/datatd_13_1950.csv", 
   "data200219/datatd_13_2000.csv", 
    }; 


    int xMin[8] = {25, 28, 31, 34, 37, 40, 43, 46};
    int xMax[8] = {27, 30, 33, 36, 39, 42, 45, 48};
    
    int Nbins = 2;

    double x[41]={0,50,100,150,200,250,300,350,400,450,500,
        550,600,650,700,750,800,850,900,950,1000,
        1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,
        1550,1600,1650,1700,1750,1800,1850,1900,1950,2000};
    double y[41];

TCanvas *c1 = new TCanvas("c1", "Time difference");

for(int i=0; i<N; i++){   
       
   	// Histograms
   	TH1F *h1 = new TH1F("h1", "TD (del. 13)", Nbins, xMin[i], xMax[i]) ;   
   
   
   	// input files
   	ifstream f1(filename[i]) ; 

   	// fill histograms
   	double x1;
   	while(f1 >> x1){
      	h1->Fill(abs(x1)) ;}
   	f1.close();
  
   	//gPad->SetGridx();
   	//gPad->SetGridy();
 
   /*
   	// Normalize histogram
   	int Nb = h1->GetNbinsX();
   	int effEntries = h1->GetEntries();
   	double norm = (h1->GetEntries())/(h1->GetNbinsX());
   	h1->Scale(1/norm);
   	for(int j=1; j<=Nb; j++){
   		h1->SetBinContent(j, (h1->GetBinContent(j))-1);
   	}
   	
	h1->SetEntries(effEntries);
 */  	h1->SetFillStyle(1001);
   	h1->SetFillColor(kRed - 3);
   	h1->SetLineColor(kRed);
   	h1->Draw("HIST");
  
 /*  	// Print DNL
   	double dnl[Nb];
   	double maxDnl=0;
   	double meanDnl=0;
   	for(int n=1; n<=Nb; n++){
   		dnl[n-1] = abs(h1->GetBinContent(n));
        if(dnl[n-1]>maxDnl)
        	maxDnl=dnl[n-1];	  	
   //   printf("DNL = %.3f\n", dnl[n-1]);
   		meanDnl = meanDnl + dnl[n-1];
   	}	
   	meanDnl = meanDnl/Nb;   
   	printf("\nMaximum deviation is: %.3f \n", maxDnl);
   	printf("\nMean deviation is: %.3f \n\n", meanDnl);
   */
    
   	// x-axis
   	h1->GetXaxis()->SetTitle("Time");
   	//histo->GetXaxis()->SetTitleFont(132);
   	//histo->GetXaxis()->SetTitleSize(0.055);
   	//histo->GetXaxis()->SetTitleOffset(1.1);
   	//histo->GetXaxis()->SetLabelFont(132);
   	//histo->GetXaxis()->SetLabelSize(0.04);


   	// y-axis
   	h1->GetYaxis()->SetTitle("Hits");
   	h1->GetYaxis()->CenterTitle();
  
   	//histo->GetYaxis()->SetTitleOffset(0.9);
   	//histo->GetYaxis()->SetTitleFont(132);
   	//histo->GetYaxis()->SetTitleSize(0.055);
   	//histo->GetYaxis()->SetLabelFont(132);
   	//histo->GetYaxis()->SetLabelSize(0.04);

   	gPad->Modified();
   	gPad->Update();
    
    y[i]=h1->GetMean();

}
printf("Plotting.......\n");
printf("  x  |  y  \n");
printf("-----|-----");
 
    for(int k=0; k<N; k++){
        printf("\n%5.0f|%2.3f", x[k], y[k]);
    }
    printf("\n-----|-----\n");
    cout<<endl<<endl;
     
TGraph *gr = new TGraph(N, x, y);
 
gr->SetMarkerStyle(21);
gr->SetMarkerSize(0.8);
 
gr->SetTitle("Time Difference") ;
 
//x-axis setup
gr->GetXaxis()->SetTitle("Distance [ns]") ;
gr->GetXaxis()->CenterTitle() ;
     
// y-axis setup
gr->GetYaxis()->SetTitle("TD") ;
gr->GetYaxis()->CenterTitle() ;

gr->Draw("AP");


}


