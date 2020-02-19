void dnl() {
    gROOT -> LoadMacro("dnl00_11.cxx");
    gROOT -> LoadMacro("dnl00_12.cxx");
    gROOT -> LoadMacro("dnl00_13.cxx");
    gROOT -> LoadMacro("dnl00_14.cxx");
    gROOT -> LoadMacro("dnl00_15.cxx");
    gROOT -> LoadMacro("dnl32_11.cxx");
    gROOT -> LoadMacro("dnl32_12.cxx");
    gROOT -> LoadMacro("dnl32_13.cxx");
    gROOT -> LoadMacro("dnl32_14.cxx");
    gROOT -> LoadMacro("dnl32_15.cxx");
    gROOT -> LoadMacro("td32_11.cxx");
    gROOT -> LoadMacro("td32_12.cxx");
    gROOT -> LoadMacro("td32_13.cxx");
    gROOT -> LoadMacro("td32_14.cxx");
    
    TCanvas *cc1 = new TCanvas("cc1", "DNL (00)");
   cc1 -> Divide(2, 2);
    
    TCanvas *cc2 = new TCanvas("cc2", "DNL (32)");
    cc2 -> Divide(2, 2);
    
    
    cc1 -> cd(1);
    gROOT->ProcessLine("dnl00_11()");
    cc1 -> cd(2);
    gROOT->ProcessLine("dnl00_12()");
    cc1 -> cd(3);
    gROOT->ProcessLine("dnl00_13()");
    cc1 -> cd(4);
    gROOT->ProcessLine("dnl00_14()");
    cc1 -> cd(5);
    
    gROOT->ProcessLine("dnl00_15()");
    cc2 -> cd(1);
    gROOT->ProcessLine("dnl32_11()");
    cc2 -> cd(2);
    gROOT->ProcessLine("dnl32_12()");
    cc2 -> cd(3);
    gROOT->ProcessLine("dnl32_13()");
    cc2 -> cd(4);
    gROOT->ProcessLine("dnl32_14()");
    cc2 -> cd(5);
    gROOT->ProcessLine("dnl32_15()");


}
