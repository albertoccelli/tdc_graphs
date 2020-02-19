
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

void dnl_sweep32()
{
    gROOT->Reset() ;
  
    TCanvas *c1 = new TCanvas("c1", "DNL integrals (ch32)");

    double  xMin  = 0.0 ;
    double  xMax  = 32 ;
    int     Nbins = 32 ;
    int entries = 0;
    int pEntries = 0;
    int N = 21;

    string filename [21] = {"data200219/data32_13_0000.csv",
                            "data200219/data32_13_0050.csv",
                            "data200219/data32_13_0100.csv",
                            "data200219/data32_13_0150.csv",
                            "data200219/data32_13_0200.csv",
                            "data200219/data32_13_0250.csv",
                            "data200219/data32_13_0300.csv",
                            "data200219/data32_13_0350.csv",
                            "data200219/data32_13_0400.csv",
                            "data200219/data32_13_0450.csv",
                            "data200219/data32_13_0500.csv",
                            "data200219/data32_13_0550.csv",
                            "data200219/data32_13_0600.csv",
                            "data200219/data32_13_0650.csv",
                            "data200219/data32_13_0700.csv",
                            "data200219/data32_13_0750.csv",
                            "data200219/data32_13_0800.csv",
                            "data200219/data32_13_0850.csv",
                            "data200219/data32_13_0900.csv",
                            "data200219/data32_13_0950.csv",
                            "data200219/data32_13_1000.csv",
    };

// x axis
    double x[N];
    for(int j=0; j<N; j++)
        x[j]=j*50.0;

// y axis
    double y[N];

// Loop through histograms
    for(int i=0; i<N; i++){

// Histograms
   	TH1F *h1 = new TH1F("h1", "DNL (channel 00, del. 13)", Nbins, xMin, xMax) ;   

   	// input files
   	ifstream f1(filename[i]) ; 

   	// fill histograms
   	double x1;
   	while(f1 >> x1){
      	h1->Fill(x1) ;}
   	f1.close();
  
   
   	// Normalize histogram
   	int Nb = h1->GetNbinsX();
   	int effEntries = h1->GetEntries();
   	double norm = (h1->GetEntries())/(h1->GetNbinsX());
   	h1->Scale(1/norm);
   	for(int j=1; j<=Nb; j++){
   		h1->SetBinContent(j, (h1->GetBinContent(j))-1);
   	}
   /*	
	h1->SetEntries(effEntries);
   	h1->SetFillStyle(1001);
   	h1->SetFillColor(kRed - 3);
   	h1->SetLineColor(kRed);
   	h1->Draw("HIST");
  */
   	// Print DNL
   	double dnl[Nb];
   	double maxDnl=0;
   	double meanDnl=0;
   	for(int n=1; n<=Nb; n++){
   		dnl[n-1] = (h1->GetBinContent(n));
        if(dnl[n-1]>maxDnl)
        	maxDnl=abs(dnl[n-1]);	  	
      printf("DNL = %.3f\n", dnl[n-1]);
   		meanDnl = meanDnl + abs(dnl[n-1]);
   	}	
   	meanDnl = meanDnl/Nb;

    double integral = h1->Integral(xMax, xMin);
   	y[i]=integral;

    printf("\nMaximum deviation is: %.3f \n", maxDnl);
   	printf("\nMean deviation is: %.3f \n", meanDnl);
   	printf("\nIntegral: %.3f \n\n", integral);
   
    
   	// x-axis
   	h1->GetXaxis()->SetTitle("Bins");
   	//histo->GetXaxis()->SetTitleFont(132);
   	//histo->GetXaxis()->SetTitleSize(0.055);
   	//histo->GetXaxis()->SetTitleOffset(1.1);
   	//histo->GetXaxis()->SetLabelFont(132);
   	//histo->GetXaxis()->SetLabelSize(0.04);


   	// y-axis
   	h1->GetYaxis()->SetTitle("DNL");
   	h1->GetYaxis()->CenterTitle();
  
   	//histo->GetYaxis()->SetTitleOffset(0.9);
   	//histo->GetYaxis()->SetTitleFont(132);
   	//histo->GetYaxis()->SetTitleSize(0.055);
   	//histo->GetYaxis()->SetLabelFont(132);
   	//histo->GetYaxis()->SetLabelSize(0.04);
}
    printf("\nPlotting.......\n"); 
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


gr->SetTitle("DNL integrals (ch 32)") ;

    c1->SetGridx();
    c1->SetGridy();

    //x-axis setup
    gr->GetXaxis()->SetTitle("Distance [ns]") ;
    gr->GetXaxis()->CenterTitle() ;
   //gr->GetXaxis()->SetRangeUser(...) ;
   //gr->GetXaxis()->SetNdivisions(...) ;
   //gr->GetXaxis()->SetTickLength(...) ;
   //gr->GetXaxis()->SetLabelOffset(...) ;
   //gr->GetXaxis()->SetLabelSize(...) ;  
   //gr->GetXaxis()->SetLabelFont(...) ; 
   //gr->GetXaxis()->SetTitleOffset(...) ;
   //gr->GetXaxis()->SetTitleSize(...) ;   
   //gr->GetXaxis()->SetTitleFont(...) ; 
   
   // y-axis setup
   gr->GetYaxis()->SetTitle("DNL") ;
   gr->GetYaxis()->CenterTitle() ;
   //gr->GetYaxis()->SetRangeUser(...) ;
   //gr->GetYaxis()->SetNdivisions(...) ;
   //gr->GetYaxis()->SetTickLength(...) ;
   //gr->GetYaxis()->SetLabelOffset(...) ;
   //gr->GetYaxis()->SetLabelSize(...) ;  
   //gr->GetYaxis()->SetLabelFont(...) ; 
   //gr->GetYaxis()->SetTitleOffset(...) ;
   //gr->GetYaxis()->SetTitleSize(...) ;   
   //gr->GetYaxis()->SetTitleFont(...) ; 

gr->Draw("APL");

c1->Modified();
c1->Update();
}
