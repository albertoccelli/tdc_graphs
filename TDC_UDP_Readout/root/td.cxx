void td() {
    gROOT -> LoadMacro("td11.cxx");
    gROOT -> LoadMacro("td12.cxx");
    gROOT -> LoadMacro("td13.cxx");
    gROOT -> LoadMacro("td14.cxx");
    
    TCanvas *td1 = new TCanvas("td1", "Time difference");
    td1 -> Divide(2, 2);
    
    
    td1 -> cd(1);
    gROOT->ProcessLine("td11()");
    td1 -> cd(2);
    gROOT->ProcessLine("td12()");
    td1 -> cd(3);
    gROOT->ProcessLine("td13()");
    td1 -> cd(4);
    gROOT->ProcessLine("td14()");
}
