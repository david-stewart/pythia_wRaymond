#include <TSystem>
#include <TString>

void pyth6(string argv) {
#ifdef __CINT__
  gSystem->Load("libEG");
  gSystem -> Load("$FASTJET/lib/libfastjet");
  gSystem -> Load("$FASTJET/lib/libfastjettools");
  gSystem->Load("libEGPythia6");
#endif
    gSystem->Load("Pyth6");
    Pyth6* py = new Pyth6(argv);
    delete py;
};
