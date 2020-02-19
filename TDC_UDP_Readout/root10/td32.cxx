void td32() {
    gROOT -> LoadMacro("td32_11.cxx");
    gROOT -> LoadMacro("td32_12.cxx");
    gROOT -> LoadMacro("td32_13.cxx");
    gROOT -> LoadMacro("td32_14.cxx");
    
    gROOT->ProcessLine("td32_11()");
    gROOT->ProcessLine("td32_12()");
    gROOT->ProcessLine("td32_13()");
    gROOT->ProcessLine("td32_14()");


}
