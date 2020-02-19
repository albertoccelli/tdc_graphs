
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

void td32_12()
{
   	gROOT->Reset() ;
   	gStyle->SetOptStat("e") ;   // show into the statistics box the number of Entries, the Mean and the RMS  
  
    TCanvas *c2 = new TCanvas("c2", "DNL 32/12");
    c2->Divide(1,2);
   
   	double  xMin  = 0.0 ;
   	double  xMax  = 32 ;
   	int     Nbins = 32 ;
    
    
    

   	// Histograms
   	TH1F *h1 = new TH1F("h1", "DNL (channel 32, del. 12, EVENS)", Nbins, xMin, xMax) ;   
   	TH1F *h2 = new TH1F("h2", "DNL (channel 32, del. 12, ODDS)", Nbins, xMin, xMax) ;   
   	// input files
   	ifstream f1("data/data32_12.csv") ; 

   	// fill histograms
   	int x1;
   	while(f1 >> x1){
      	if(x1%2!=0)
            h1->Fill(x1) ;
        else
            h2->Fill(x1);
    }
   	f1.close();
  
   	//gPad->SetGridx();
   	//gPad->SetGridy();
 
   c2->cd(1);
   

   h1->Scale(16.0/h1->GetEntries());
   h2->Scale(16.0/h2->GetEntries());
   // Normalize histogram
  	for(int j=1; j<=Nbins; j++){
   		if(j%2!=0)
            h2->SetBinContent(j, (h2->GetBinContent(j)-1)  );
   	    else
            h1->SetBinContent(j, (h1->GetBinContent(j)-1)  );
    }
   	h1->SetEntries(h1->GetEntries() - 16);
   	h1->SetFillStyle(1001);
   	h1->SetFillColor(kRed - 3);
   	h1->SetLineColor(kRed);
   	h1->Draw("HIST");
   	
    c2->cd(2);
   	
    h2->SetFillStyle(1001);
   	h2->SetEntries(h2->GetEntries() - 16);
   	h2->SetFillColor(kBlue - 3);
   	h2->SetLineColor(kBlue);
   	h2->Draw("HIST");
/*
    for (int n = 1; n<=Nbins; n++){
        printf("\n---o---|---e---\n");
        printf(  "%f, %f", h1->GetBinContent(n), h2->GetBinContent(n)); 
    }
*/
    /*
   	// Print DNL
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

   	gPad->Modified();
   	gPad->Update();
   
   
}
